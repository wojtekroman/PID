
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
// ***************** To be removed ? ******************************

	if ((RCC->CFGR & RCC_CFGR_PPRE1_DIV2) && (PWM_TIMER == TIM3))
	{
		localSystemClock = SystemCoreClock / 2;			// Peripheria in APB1 have system clock / 2 (see user manual page 125)  in system_stm32F10x.c line 1028 RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

	}
	else if (PWM_dev->TIMx == TIM3)
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



	/* timer switch ON */

	if (PWM_dev->TIMx == TIM3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	}
	else if (PWM_dev->TIMx == TIM17)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);//#error "Undefined RCC for spcifyied timer."
	}
	else if (PWM_dev->TIMx == TIM1)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//#error "Undefined RCC for spcifyied timer."
	}

	TIM_Cmd(PWM_dev->TIMx, DISABLE);
	//SystemCoreClock
	/* 1ms == 48000 */
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) 0x00;					//	to get MAX F speed
	TIM_TimeBaseStructure.TIM_Period = (uint16_t)(PWM_dev->Freq );					// to have 200Hz frequency
 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;				// counter counts from 0 up to Certain value which is auto reloaded (ARR reg) after Update Event V
 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
 	TIM_TimeBaseInit(PWM_dev->TIMx, &TIM_TimeBaseStructure);

 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2;
 	TIM_OCInitStructure.TIM_OutputNState= TIM_OutputState_Enable;
 	TIM_OCInitStructure.TIM_Pulse = (uint16_t)(PWM_dev->Freq /2);
 	TIM_OC1Init(PWM_dev->TIMx, &TIM_OCInitStructure);
	TIM_OC1PreloadConfig(PWM_dev->TIMx, TIM_OCPreload_Enable);
	TIM_OC2Init(PWM_dev->TIMx, &TIM_OCInitStructure);
	TIM_OC2PreloadConfig(PWM_dev->TIMx, TIM_OCPreload_Enable);



 	/* enable interrupts */

 	TIM_Cmd(PWM_dev->TIMx, ENABLE);
 	TIM_CtrlPWMOutputs(PWM_dev->TIMx, ENABLE);

	PWM_dev->TIMx->CCER |= TIM_CCER_CC1E+TIM_CCER_CC2NE+TIM_CCER_CC1NP;
	PWM_dev->TIMx->CCER |= TIM_CCER_CC2E+TIM_CCER_CC2NE+TIM_CCER_CC2NP;
	PWM_dev->TIMx->CCER &= ~(TIM_CCER_CC1P+TIM_CCER_CC2P);

}

// **************** GPIO INIT FOR LED LAMP *************************

void PWM_GPIOHardwareInit(void)

{

	GPIO_InitTypeDef GPIO_InitStruct;
	//*********************  CLOCK ACTIVATION **********************

	PWM_PORT1_RCC_ON;
	//PWM_PORT2_RCC_ON;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	// ******************** GPIO CONFIGURATION *********************
	// PWM TIM 4 _ CH 1			PA8			<- no remap
	// PWM TIM 1 _ N CH 1		PB13		<- no remap
	// PWM TIM 1 _ CH 2			PA9			<- no remap
	// PWM TIM 1 _ N CH 2		PB14		<- no remap
	// *************************************************************

	GPIO_InitStruct.GPIO_Pin = PWM_PIN1;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;


	#ifdef STM32F103C8
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	#endif

	GPIO_Init(PWM_PORT1, &GPIO_InitStruct);

}


// **********************************************************************************
// ************************** LED_LAMP_SOFTWARE_INIT ********************************
// **********************************************************************************
void PWM_SoftwareInit(void)
{
	PWM_Unit.sreg = PWM_DEF_SREG;
	PWM_Unit.TIMx = PWM_TIMER;
	PWM_Unit.Freq = PWM_SET_FREQ(1000000)	;
	PWM_Unit.Filling = PWM_Unit.Freq/2;




}
