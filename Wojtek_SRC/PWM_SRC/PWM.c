
#include "PWM.h"


// *********************************************************
// **************** LAMP HW INIT ***************************
// *********************************************************

void PWM_HardwareInit(void)
{
	PWM_SoftwareInit();
	PWM_GPIOHardwareInit();

	TimerForPWMHardwareInit(&PWM_Unit);

}




// **************** TIMER INIT FOR LED LAMP *************************

void TimerForPWMHardwareInit(PWM_t *PWM_dev)
{

	uint32_t localSystemClock;
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef	TIM_OCInitStructure;
	TIM_BDTRInitTypeDef bdtr;

	// ****************** Stop timer if uC in in debugging mode ************************
#ifdef STM32F030F4P6

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_DBGMCU, ENABLE);
	if (PWM_dev->TIMx == TIM17)
	{
		DBGMCU->APB2FZ |= (uint32_t)(DBGMCU_APB2_FZ_DBG_TIM17_STOP);
	}
	else if (PWM_dev->TIMx == TIM14)
	{
//		DBGMCU->APB1FZ |= (uint32_t)(DBGMCU_APB1_FZ_DBG_TIM14_STOP);
	}
#endif
/*/ ***************** To be removed ? ******************************

	if ((RCC->CFGR & RCC_CFGR_PPRE1_DIV2) && ((PWM_TIMER == TIM3)||(PWM_TIMER == TIM4)))
	{
		localSystemClock = SystemCoreClock / 2;			// Peripheria in APB1 have system clock / 2 (see user manual page 125)  in system_stm32F10x.c line 1028 RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

	}
	else if ((PWM_dev->TIMx == TIM3) || (PWM_TIMER == TIM4))
	{
		localSystemClock = 8000000;//SystemCoreClock;
	}

	// ***************** To be removed ? END ******************************


	// TIM 1 is from system clock, check do we have prescaler for APB2 set
#ifdef STM32F103C8
	if (!(RCC->CFGR & RCC_CFGR_PPRE2_2) )
	{
		localSystemClock = SystemCoreClock;
	}
	else
	{
		localSystemClock = SystemCoreClock / (1<< (RCC->CFGR & (~RCC_CFGR_PPRE2_2)) );
	}


	localSystemClock = SystemCoreClock / (1<< ((RCC->CFGR & (~RCC_CFGR_PPRE2_2))>>10) );
#endif

*/

	/* timer switch ON */

	if (PWM_dev->TIMx == TIM3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	}
	else if (PWM_dev->TIMx == TIM4)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);//#error "Undefined RCC for spcifyied timer."
	}

	else if (PWM_dev->TIMx == TIM17)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);//#error "Undefined RCC for spcifyied timer."
	}
	else if (PWM_dev->TIMx == TIM1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);
	}

		TIM_Cmd(PWM_dev->TIMx, DISABLE);
	//SystemCoreClock
	/* 1ms == 72000 */

	    TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) 0x00;					//	to get MAX F speed
		TIM_TimeBaseStructure.TIM_Period = (uint16_t)(PWM_dev->Freq );					// to have 200Hz frequency
	 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;				// counter counts from 0 up to Certain value which is auto reloaded (ARR reg) after Update Event V
	 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	 	TIM_TimeBaseInit(PWM_dev->TIMx, &TIM_TimeBaseStructure);

	 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	 	TIM_OCInitStructure.TIM_OutputNState= TIM_OutputNState_Enable;
	 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	 	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	 	TIM_OCInitStructure.TIM_OCNPolarity = TIM_OCNPolarity_High;
	 	TIM_OCInitStructure.TIM_Pulse = (uint16_t)(PWM_dev->Filling);
	 	TIM_OC1Init(PWM_dev->TIMx, &TIM_OCInitStructure);
		TIM_OC1PreloadConfig(PWM_dev->TIMx, TIM_OCPreload_Enable);
		//TIM_OC2Init(PWM_dev->TIMx, &TIM_OCInitStructure);
		//TIM_OC2PreloadConfig(PWM_dev->TIMx, TIM_OCPreload_Enable);

		// *************** BDTR register configuration ********************
		bdtr.TIM_OSSRState = TIM_OSSRState_Enable;
		bdtr.TIM_OSSIState = TIM_OSSIState_Enable;
		bdtr.TIM_LOCKLevel = TIM_LOCKLevel_OFF;
		bdtr.TIM_DeadTime = 5;
		bdtr.TIM_Break = TIM_Break_Disable;
		bdtr.TIM_BreakPolarity = TIM_BreakPolarity_High;
		bdtr.TIM_AutomaticOutput = TIM_AutomaticOutput_Enable;

		TIM_BDTRConfig(PWM_dev->TIMx, &bdtr);

	 	/* enable interrupts */


		PWM_dev->TIMx->CR1 |=TIM_CR1_ARPE; 			// auto reload enable



 	/* enable interrupts */

	TIM_CtrlPWMOutputs(PWM_dev->TIMx, ENABLE);			// PWM output enable
 	TIM_Cmd(PWM_dev->TIMx, ENABLE);						// timx enable


}

// **************** GPIO INIT FOR LED LAMP *************************

void PWM_GPIOHardwareInit(void)

{

	GPIO_InitTypeDef GPIO_InitStruct;
	//*********************  CLOCK ACTIVATION **********************

	PWM_PORT1_RCC_ON;
	PWM_PORT2_RCC_ON;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	// ******************** GPIO CONFIGURATION *********************
	// PWM TIM 1 _ N	CH 1N			PB13		<- no remap
	// PWM TIM 1 _ 		CH 1			PA8			<- no remap
	// *************************************************************

	GPIO_InitStruct.GPIO_Pin = PWM_PIN1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;


	#ifdef STM32F103C8
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	#endif

	GPIO_Init(PWM_PORT1, &GPIO_InitStruct);
	/// TIM1CH1 activation
	GPIO_InitStruct.GPIO_Pin = PWM_PIN2;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;


	#ifdef STM32F103C8
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	#endif

	GPIO_Init(PWM_PORT2, &GPIO_InitStruct);


}

void PWM_FillerUpdate (PWM_t *PWM_dev)
{
	PWM_dev->TIMx->CCR1 = PWM_dev->Fulfillment;


}

// **********************************************************************************
// ************************** LED_LAMP_SOFTWARE_INIT ********************************
// **********************************************************************************
void PWM_SoftwareInit(void)
{
	PWM_Unit.sreg = PWM_DEF_SREG;
	PWM_Unit.TIMx = PWM_TIMER;
	PWM_Unit.Freq = PWM_SET_FREQ(1000);
	PWM_Unit.Fulfillment = PWM_Unit.Freq/10;			//przy Fill rownym 0x00000193f jest prawie max napiecie = 6450


}
