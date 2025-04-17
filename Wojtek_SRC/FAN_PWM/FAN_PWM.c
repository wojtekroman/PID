#define FAN_PWM_C
	#include "FAN_PWM.h"
#undef FAN_PWM_C

// *********************************************************
// **************** LAMP HW INIT ***************************
// *********************************************************

void FAN_HardwareInit(void)
{
	FAN_SoftwareInit();
	FAN_GPIOHardwareInit();
#ifdef FAN_BY_PWM
	FAN_TimerForPWMHardwareInit(&FAN_drawer);
#endif
}




// **************** TIMER INIT FOR LED LAMP *************************
#ifdef FAN_BY_PWM
void FAN_TimerForPWMHardwareInit(FANType *FANx)
{

	uint32_t localSystemClock;
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef	TIM_OCInitStructure;

	// ****************** Stop timer if uC in in debugging mode ************************
#ifdef STM32F030F4P6

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_DBGMCU, ENABLE);
	if (LedLampX->TIMx == TIM17)
	{
		DBGMCU->APB2FZ |= (uint32_t)(DBGMCU_APB2_FZ_DBG_TIM17_STOP);
	}
	else if (LedLampX->TIMx == TIM14)
	{
//		DBGMCU->APB1FZ |= (uint32_t)(DBGMCU_APB1_FZ_DBG_TIM14_STOP);
	}
#endif


	if ((RCC->CFGR & RCC_CFGR_PPRE1_DIV2) && (FAN_PWM_TIMER == TIM3))
	//if ((RCC->CFGR & RCC_CFGR_PPRE_DIV2) && (LED_LAMP_PWM_TIMER == TIM14))
	{
		localSystemClock = SystemCoreClock / 2;			// Peripheria in APB1 have system clock / 2 (see user manual page 125)  in system_stm32F10x.c line 1028 RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

	}
	else
	{
		localSystemClock = 8000000;//SystemCoreClock;
	}


	/* timer switch ON */

	if (FANx->TIMx == TIM3)
	{
		RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	}
	else if (FANx->TIMx == TIM17)
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);//#error "Undefined RCC for spcifyied timer."
	}
	else if ((FANx->TIMx == TIM1))
	{
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM1, ENABLE);//#error "Undefined RCC for spcifyied timer."
	}

	//SystemCoreClock
	/* 1ms == 48000 */
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) 0x00;					//((localSystemClock / 200)-1 );	// 1000 => 1ms
	TIM_TimeBaseStructure.TIM_Period = (uint16_t)(FANx->Freq -1);					// to have 200Hz frequency
 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;				// counter counts from 0 up to Certain value which is auto reloaded (ARR reg) after Update Event V
 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
 	TIM_TimeBaseInit(FANx->TIMx, &TIM_TimeBaseStructure);

 	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
 	TIM_OCInitStructure.TIM_OutputNState= TIM_OutputState_Enable;
 	TIM_OCInitStructure.TIM_Pulse = (uint16_t)(FANx->Freq /2);
 	TIM_OC3Init(FANx->TIMx, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(FANx->TIMx, TIM_OCPreload_Enable);
	//uC_TIMER_1ms->CR1 |= 0x04; przerwanie co okolo 5s




 	/* enable interrupts */

 	TIM_Cmd(FANx->TIMx, ENABLE);




}
#endif
// **************** GPIO INIT FOR LED LAMP *************************

void FAN_GPIOHardwareInit(void)

{

	GPIO_InitTypeDef GPIO_InitStruct;
	//*********************  CLOCK ACTIVATION **********************

	FAN_PORT_RCC_ON;		//
#ifdef STM32F103C8
	// Enable clock for alternative function			different command for different uControlers
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
#endif

	//RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOB, ENABLE);
#ifdef FAN_BY_PWM

#endif

	// ******************** GPIO CONFIGURATION *********************

	GPIO_InitStruct.GPIO_Pin = FAN_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;

#ifdef FAN_BY_PWM
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	#ifdef STM32F103C8
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	#endif

#else

	#ifdef STM32F030X6
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_OUT;//GPIO_Mode_Out_PP;
		GPIO_InitStruct.GPIO_OType = GPIO_OType_PP;
		GPIO_InitStruct.GPIO_PuPd = GPIO_PuPd_DOWN;
	#endif

	#ifdef STM32F10X_MD
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;
	#endif

#endif

		GPIO_Init(FAN_PORT, &GPIO_InitStruct);

#ifdef FAN_BY_PWM

	// activate alternative function for pin1 GPIOB as TIM14_CH1
//	GPIO_PinAFConfig(LED_LAMP_PORT, GPIO_PinSource1,  GPIO_AF_0);
	// activate alternative function for pin4 GPIOA as TIM14_CH1
#ifdef STM32F103C8
#else

		GPIO_PinAFConfig(FAN_PORT, GPIO_PinSource0,  GPIO_AF_4);
#endif

#endif
}


// **********************************************************************************
// ************************** LED_LAMP_SOFTWARE_INIT ********************************
// **********************************************************************************
void FAN_SoftwareInit(void)
{
	FAN_drawer.Sreg = FAN_DEF_SREG;
#ifdef FAN_BY_PWM
	FAN_drawer.IntensivityCurrent = FAN_DEF_OFF_SPEED;
	FAN_drawer.IntensivityON = FAN_MIN_SPEED;
	FAN_drawer.IntensivityOFF = FAN_DEF_OFF_SPEED;
	FAN_drawer.Freq = FAN_DEF_FREQ;
	FAN_drawer.TIMx = FAN_PWM_TIMER;
	FAN_drawer.IntensivityGradient = DEFAULT_FAN_SPEED_GRADIENT;
	FAN_drawer.FanTempActivationLevel = FAN_TEMP_ACT_DEAFULD;
	FAN_drawer.FanSpeedChangeDelayValue = DEF_FAN_SPPED_CHANGE_DELAY_VALUE;
#else
	FAN_drawer.IntensivityCurrent = 0;
	FAN_drawer.IntensivityON = 1000;
	FAN_drawer.IntensivityOFF = 0;
#endif

}
