/*
 * Software timers
 *
 */


#define TIMERS_C
#ifdef STM32F030F4P6
#include "stm32f0xx_gpio.h"

#endif

#ifdef STM32F103C8
#include "stm32f10x_gpio.h"
//#include "CAN_Volvo.h"
#endif

#include "timers.h"

#undef TIMERS_C

/*
 * 		HARDWARE INIT FOR TIMERS
 */
void TIMERS_HARDWARE_INIT(void)
{
	  // ***************** stop timers in debugging mode ******************
uint32_t localSystemClock;

#ifdef STM32F030F4P6
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_DBGMCU, ENABLE);

	DBGMCU->APB2FZ |= (uint32_t)(DBGMCU_APB2_FZ_DBG_TIM17_STOP);
	DBGMCU->APB1FZ |= (uint32_t)(DBGMCU_APB1_FZ_DBG_TIM3_STOP);
	DBGMCU->APB2FZ |= (uint32_t)(DBGMCU_APB2_FZ_DBG_TIM1_STOP);
#endif
	/* system clock is 48 MHz*/
#ifdef STM32F103C8
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_DBGMCU, ENABLE);
//	DBGMCU->CR |= (uint32_t)(DBGMCU_TIM3_STOP);
	DBGMCU->CR |= (uint32_t)(DBGMCU_TIM4_STOP);

#endif

	if ((RCC->CFGR & RCC_CFGR_PPRE1_DIV2) && (uC_TIMER_100ms == TIM4))
	{
		localSystemClock = SystemCoreClock / 2;			// Peripheria in APB1 have system clock / 2 (see user manual page 125)  in system_stm32F10x.c line 1028 RCC->CFGR |= (uint32_t)RCC_CFGR_PPRE1_DIV2;

	}
	NVIC_InitTypeDef NVIC_InitStruct;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;

	/* timer switch ON */
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_TIM17, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	//SystemCoreClock
	/* 1ms == 48000 */
	TIM_TimeBaseStructure.TIM_Prescaler = (uint16_t) ((localSystemClock / 1000)-1 );	// 1000 => 1ms
	TIM_TimeBaseStructure.TIM_Period = (uint16_t)100;
 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;			// counter counts from 0 up to Certain value which is auto reloaded (ARR reg) after Update Event V
 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
 	TIM_TimeBaseInit(uC_TIMER_100ms, &TIM_TimeBaseStructure);

	//uC_TIMER_1ms->CR1 |= 0x04; przerwanie co okolo 5s


    // kofiguracja przerwania dla licznika 17

#ifdef STM32F030F4P6
 	NVIC_InitStruct.NVIC_IRQChannel =  TIM17_IRQn ;
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
#endif
    NVIC_InitStruct.NVIC_IRQChannel =  TIM4_IRQn ;
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);

 	/* enable interrupts */
 	TIM_ITConfig(uC_TIMER_100ms,TIM_IT_Update, ENABLE );
 	TIM_Cmd(uC_TIMER_100ms, ENABLE);



 	// ******************** timer 3 ********************************
#ifdef TIMER_1S_AS_HARDWARE

 	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);


	/* 1s == 8000000 */
	TIM_TimeBaseStructure.TIM_Prescaler =  (uint16_t)((SystemCoreClock / 1000) -1);	// 1000 => 1ms
	TIM_TimeBaseStructure.TIM_Period = (uint16_t)1000;//(uint16_t)35000;
 	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;			// counter counts from 0 up to Certain value which is auto reloaded (ARR reg) after Update Event V
 	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
 	TIM_TimeBaseInit(uC_TIMER_1s, &TIM_TimeBaseStructure);


    // kofiguracja przerwania dla licznika 3
    NVIC_InitStruct.NVIC_IRQChannel =  TIM3_IRQn ;
#ifdef STM32F030F4P6
    NVIC_InitStruct.NVIC_IRQChannelPriority = 0;
#endif
    NVIC_InitStruct.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStruct);




 	/* enable interrupts */

 	TIM_ITConfig(uC_TIMER_1s, TIM_IT_Update, ENABLE );
 	TIM_Cmd(uC_TIMER_1s, ENABLE);


#endif




}


/*
 * 	****************************************	SOFTWARE INIT FOR TIMERS	******************************
 */
void TIMERS_SOFTWARE_INIT(void)
{
	uint16_t count=0;
	while(count < TIMER_MAX_NUMBER_s)
	{
		Timers_s[count]=TIMER_STOP;
		count++;

	}

count =0;
	while(count < TIMER_MAX_NUMBER_100ms)
	{
		Timers_100ms[count]=TIMER_STOP;
		count++;

	}
	TimersMsIntCount=0;
	TimersSIntCount=0;
	Timers_100ms[TIMER_1S]=DELAY_1S_VALUE;
	Timers_s[TIMER_1H_DALAY] = DELAY_1H_VALUE;
}




// *************** timers interrupt handling **********************************
void TimersMsInterupt(void)
{
	uint16_t TempInteruptCounter = (uint16_t)TimersMsIntCount;//(uint16_t)CAN_loger_timer_ms;
	uint16_t index=0;


	//if (CAN_loger_timer_ms)//TimersMsIntCount)
	{
		TimersMsIntCount=0;//TimersMsIntCount=0;
		do {
			if (Timers_100ms[index] != TIMER_STOP)
			{
				if (Timers_100ms[index] >= TempInteruptCounter)
				{
					Timers_100ms[index] -= TempInteruptCounter;
				}
				else
				{
					Timers_100ms[index] = TIMER_END;
				}
			}

			index++;
		} while(index < TIMER_MAX_NUMBER_100ms);
	}

}
// *************** timers interrupt handling **********************************
void TimersSInterupt(void)
{
	uint16_t index=0;

#ifdef TIMER_1S_AS_HARDWARE
	uint16_t TempInteruptCounter = TimersSIntCount;
	TimersSIntCount=0;
#else

	if (Timers_100ms[TIMER_1S] == TIMER_END)
	{


#endif

	do {

		if (Timers_s[index] != TIMER_STOP)
		{
#ifdef TIMER_1S_AS_HARDWARE

			if (Timers_s[index] >= TempInteruptCounter)
			{
				Timers_s[index] -= TempInteruptCounter;
			}
			else
			{
				Timers_s[index] =0;
			}
#else
			Timers_s[index]--;
#endif

		}



		index++;
	} while(index < TIMER_MAX_NUMBER_s);

#ifdef TIMER_1S_AS_HARDWARE
#else
	Timers_100ms[TIMER_1S]=DELAY_1S_VALUE;
	}

#endif


}
// *************** timers interrupt handling **********************************
void TimersHUpdate(void)
{
	uint16_t index=0;
	if (!Timers_s[TIMER_1H_DALAY])
	{
		do {
			if (Timers_h[index] != TIMER_STOP)
			{
				if (Timers_h[index] > 0)
				{
					Timers_h[index] --;
				}

			}

			index++;
		} while(index < TIMER_MAX_NUMBER_h);

		Timers_s[TIMER_1H_DALAY]=DELAY_1H_VALUE;
	}
}
// ****************** Interupts form timer 3 ***************************


// ****************** Interupts form timer 17 ***************************

//void TIM17_IRQHandler(void)
void TIM4_IRQHandler(void)

{

	if (uC_TIMER_100ms->SR & TIM_IT_Update)			//TIM_GetITStatus(uC_TIMER_100ms, TIM_IT_Update))
 	{
		TimersMsIntCount=1;
		uC_TIMER_100ms->SR &= ~TIM_IT_Update;
 	}

}

// ****************** Interupts form timer 3 ***************************
#ifdef TIMER_1S_AS_HARDWARE
void TIM3_IRQHandler(void)

{
	//BUZER_TOGLE;

	 if (uC_TIMER_1s->SR & TIM_IT_Update)//TIM_GetITStatus(uC_TIMER_1s, TIM_IT_Update))
	 	{

		 TimersSIntCount=1;
 	 	 uC_TIMER_1s->SR &= ~TIM_IT_Update;

	 	}

	 CAN_loger_timer_ms++;
}
#endif
