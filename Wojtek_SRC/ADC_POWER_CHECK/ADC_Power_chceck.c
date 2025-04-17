#define ADC_POWER_CHECK_C
#include "ADC_Power_chceck.h"
//#include "ntc4k7.h"
#undef ADC_POWER_CHECK_C




// *************** ADC Clock activation ****************************
void ADC_ClockActive(void)
{

// uint16_t timeout= ADC_TIME_OUT;


 // ADC shall be connected with clock of MAX 14MHz

 	 //RCC->CR2 |= RCC_CR2_HSI14ON;/* (2) */

 // reset ADC Prescaller
 	 RCC->CFGR &= CFGR_ADCPRE_Reset_Mask;
 	 // set ADC prescaller for 6
 	 if (SystemCoreClock == 72000000 )
 	 {
 		 RCC->CFGR |=  0x02 << 14;
 	 }
 	 else
 	 {
 		RCC->CFGR |=  0x03 << 14;
 	 }


}

// **********************************************************************
// ****************HARDWARE INIT FOR POWER SUPPLY CHECK *****************
// **********************************************************************
uint16_t PowerCheckHardwareInit(PowerCheckType *Power)
{
	uint16_t calibration=0xFFFF;
	PowerCheckGPIOHardwareInit();
	ADC_ClockActive();
	PowerCheckADCHardwareInit((Power->ADCx), &(Power->ADC_Calibration));

	Power->ADCx->CR2 |= ADC_CR2_ADON;
	 /* Wait the ADCEN falg */
	// while(!ADC_GetFlagStatus((Power->ADCx), ADC_CR2_ADON));

	return calibration;
}

void PowerCheckSoftwareInit(PowerCheckType *Power)
{
	Power->ADCx = ADC1;
	Power->PowerValue=0;
	Power->ADC_Calibration=ADC_NO_DATA;
	Power->Voltage = 0;
}



void PowerCheckVoltageCoutn (PowerCheckType *Power)
{
 uint32_t TempVoltage=0;
 TempVoltage = (uint32_t)((uint32_t)(Power->PowerValue) * (uint32_t)ADC_DELTA_VOLTAGE);
 TempVoltage= TempVoltage+ (TempVoltage/10);
 if (Power->Voltage > TempVoltage ? Power->Voltage - TempVoltage > ADC_FILTER_VALUE : TempVoltage - Power->Voltage  > ADC_FILTER_VALUE)
 {
	 if (Power->Voltage > TempVoltage ? Power->Voltage - TempVoltage > ADC_POINT_POSITION : TempVoltage - Power->Voltage  > ADC_POINT_POSITION )
	 {
		 Power->Voltage = TempVoltage;
	 }
	 else
	 {
		  Power->Voltage += TempVoltage;
		  Power->Voltage /= 2;
		//  Power->Voltage += (Power->Voltage / 10);	// Voltage correction
	 }
	 Power->sreg |= POWER_NEW_VOLTAGE;
 }
}

void PowerCheckVoltageToString (PowerCheckType *Power, uint8_t *string)
{
	uint32_t VoltageGradient =1000000;
	uint32_t TempVoltage;
	uint32_t TempVoltageValue=Power->Voltage;
	uint8_t i=0, DotPassed=0;
	do
	{
		TempVoltage = TempVoltageValue / VoltageGradient;

		if (TempVoltage > 0)
		{
			string[i] = TempVoltage + 0x30;
			i++;
		}
		else if (TempVoltage == 0 && DotPassed)
			{
					string[i] = 0 + 0x30;
					i++;

			}

		if (VoltageGradient == ADC_POINT_POSITION)
		{
			if (TempVoltage == 0)
			{
					string[i] = 0 + 0x30;
					i++;
					DotPassed=1;
			}
			string[i] = '.';
			i++;
		}
		TempVoltageValue =TempVoltageValue - (TempVoltage*VoltageGradient);
		VoltageGradient=VoltageGradient/10;
	}
	while (VoltageGradient);

	// *********** end of string  marker ********************************
	string[i] = 0;

}
// ******************************************************************
// ********************** GPIO INIT *********************************
// ******************************************************************
void PowerCheckGPIOHardwareInit(void)
{
	GPIO_InitTypeDef GPIOInitStructure;
	// ****************** RCC CLOCK ACTIVE *******************************


	SUPPLY_CHECK_RCC_ACTIVE;
	GPIOInitStructure.GPIO_Speed =GPIO_Speed_2MHz;
	GPIOInitStructure.GPIO_Pin = SUPPLY_CHECK_PIN;			//GPIO_Pin_3;
	GPIOInitStructure.GPIO_Mode = GPIO_Mode_AIN;
	GPIO_Init(SUPPLY_CHECK_PORT, &GPIOInitStructure);		// GPIOA




	//GPIO_Init(GPIOA, &GPIOInitStructure);		// GPIOA

}



// ********************* ADC INTERRUPT FUNCTION *****************
void ADCIntConfiguration (void)
{
	NVIC_InitTypeDef ADC_IntConfigurationStructure;


	ADC_IntConfigurationStructure.NVIC_IRQChannel = ADC1_2_IRQn;
	ADC_IntConfigurationStructure.NVIC_IRQChannelCmd = ENABLE;
	ADC_IntConfigurationStructure.NVIC_IRQChannelSubPriority  = 0;
	ADC_IntConfigurationStructure.NVIC_IRQChannelPreemptionPriority  = 0;

	NVIC_Init(&ADC_IntConfigurationStructure);
}

// ******************************************************************************

void PowerCheckADCHardwareInit(ADC_TypeDef *ADCx, uint16_t *Calibration_value)
{
	ADC_InitTypeDef ADC_InitStruct;
	uint32_t	timeout = ADC_TIMEOUT_VALUE;

	// ************* clock for adc enable *******************
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);


	 ADC_InitStruct.ADC_Mode = ADC_Mode_Independent;
	 ADC_InitStruct.ADC_ScanConvMode = DISABLE;
	 ADC_InitStruct.ADC_ExternalTrigConv= ADC_ExternalTrigConv_None;
	 ADC_InitStruct.ADC_ContinuousConvMode = ENABLE;
	 ADC_InitStruct.ADC_DataAlign = ADC_DataAlign_Right;
	 ADC_InitStruct.ADC_NbrOfChannel= 1 ;

	 // ************* ADC initialization	**********************
	 ADC_Init(ADCx, &ADC_InitStruct);

	 // *************** ADC chanell configuration ****************************


	 ADC_RegularChannelConfig(ADCx, PowerCheckADCChanel,1, ADC_SampleTime_239Cycles5);

	  // **************** ADC interrupt ***********************

	  ADCIntConfiguration();

	  // *Calibration_value=(uint16_t)ADC_GetCalibrationFactor(ADCx);

	  ADC_Cmd(ADCx, ENABLE);// ADCx->CR2 |= ADC_CR2_ADON;
	  /* Wait the ADCEN falg */
	  do{
		  timeout --;
		  if (!timeout )
		  {
			  break;
		  }
	  }
  	  while(!(ADCx->CR2 & ADC_CR2_ADON));

	  ADC_ResetCalibration(ADCx);
	  timeout = ADC_TIMEOUT_VALUE;
	    /* Check the end of ADC1 reset calibration register */
	  do{
		  timeout --;
		  if (!timeout )
		  {
			  break;
		  }
	  }
	  while (ADC_GetResetCalibrationStatus(ADC1));

	    /* Start ADC1 calibaration */
	  ADC_StartCalibration(ADCx);
	    /* Check the end of ADC1 calibration */
	  timeout = ADC_TIMEOUT_VALUE;
	  do{
		  timeout --;
		  if (!timeout )
		  {
			  break;
		  }
	  }
	  while (ADC_GetCalibrationStatus(ADCx));

	  //  ****************** End of conversion interrupt enable bit **************************
	  ADCx->CR1 |= (0x01 << 5);


	  //ADC_ResetCalibration(ADCx);
	  //

	  ADC_SoftwareStartConvCmd(ADCx, ENABLE);


}




// ********************* ADC INTERRUPT FUNCTION *****************
void ADC1_2_IRQHandler(void)
{
uint32_t test32_t;
	if ((PowerSupply.ADCx->SR & ADC_FLAG_EOC) )
	{
		if (PowerSupply.ADCx->DR > PowerSupply.PowerValue ? PowerSupply.ADCx->DR - PowerSupply.PowerValue > 400 :  PowerSupply.PowerValue - PowerSupply.ADCx->DR >400 )
		{
			PowerSupply.PowerValue = PowerSupply.ADCx->DR;
		}
		else{
			PowerSupply.PowerValue +=PowerSupply.ADCx->DR;
			PowerSupply.PowerValue /=2;
		}

		PowerSupply.ADCx->SR = (uint32_t)(ADC_FLAG_EOC ) ;
	}

	if ((ADC2->SR & ADC_FLAG_EOC) )
	{
		if (ADC2->DR > Temperature.ADC_Value ? ADC2->DR - Temperature.ADC_Value > 200 :  Temperature.ADC_Value - ADC2->DR >200 )
		{
			Temperature.ADC_Value= ADC2->DR;
		}else
		{
			Temperature.ADC_Value+=ADC2->DR;
			Temperature.ADC_Value/=2;
		}
		Temperature.sreg |= NTC4K7_ADC_NEW_VALUE;
		ADC2->SR = (uint32_t)(ADC_FLAG_EOC);
	}
}
