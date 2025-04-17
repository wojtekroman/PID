/*
 * Timers h
 *	Used hardware timer is TIM4 , TIM3 will be used for PWM for motor fan
 */



#ifdef TIM15_BUSY
	#ifdef	STM32F030F4P6
		#error "TIM15 is not pressent in STM32F030F4P6"
	#endif
#endif


#ifndef TIM4_BUSY
	#define	uC_TIMER_100ms		TIM4
	#define	TIM4_BUSY

#else
	#error	"Double allocation for TIM4"
#endif


#ifdef TIMER_1S_AS_HARDWARE
	#ifndef TIM3_BUSY
		#define	uC_TIMER_1s			TIM3
		#define	TIM3_BUSY
	#else
		#error "Double allocation for TIM3"
	#endif
#endif



 #ifdef TIM7_BUSY
  #ifdef STM32F030F4P6
	#error "Device don't exist. Tim7 exist only at STM32F030xC."
  #endif
 #endif

 #ifdef TIM15_BUSY
  #ifdef STM32F030F4P6
   #error "Device don't exist. Tim7 exist only at STM32F030xC."
  #endif
 #endif

 #ifdef TIM6_BUSY
  #ifdef STM32F030F4P6
   #error "Device don't exist. Tim6 exist only at STM32FO30x8 and STM32F030xC."
  #endif
 #endif

// **************** All Timers with 1S delay ***********************
enum{
	TIMER_BUZZER_s=0,
	TIMER_1H_DALAY,
	TIMER_LED,
	TIMER_LUGGAGE_CHECK,
	TIMER_HC595_LATCH,
	TIMER_MAX_NUMBER_s
};

// all timers with 100ms delay
enum{
	TIMER_1S=0,
	GYRO_CHECK_DELAY,
	TIMER_POWER_VOLTAGE_DISPLAY,
	TIMER_TEMPERATURE_DISPLAY,
	TIMER_FAN_SPEED_CHANGE,
	TIMER_MAX_NUMBER_100ms
};


// Timers with 1h delay
enum {
	TIMER_H_IRRIGATION_DELAY=0,
	TIMER_MAX_NUMBER_h
};
#define TIMER_STOP		0xFFFF
#define TIMER_END   	0
#define DELAY_1H_VALUE	3600
#define DELAY_1S_VALUE	10
#ifdef TIMERS_C

 #ifdef STM32F030F4P6				//STM32F0XX
	#include "stm32f0xx.h"
	#include "stm32f0xx_rcc.h"
	#include "stm32f0xx_tim.h"
	#include "stm32f0xx_misc.h"

 #endif



#ifdef STM32F103C8
#include "stm32f10x.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_tim.h"
#include "misc.h"
#include "stm32f10x_dbgmcu.h"
#endif

uint16_t	Timers_h[TIMER_MAX_NUMBER_h];
uint16_t	Timers_s[TIMER_MAX_NUMBER_s];
uint16_t	Timers_100ms[TIMER_MAX_NUMBER_100ms];

volatile uint16_t TimersMsIntCount=0;
volatile uint16_t TimersSIntCount=0;


#ifdef STM32F103C8
 #warning "Available timers are TIM1 to TIM4".
#endif

#else
 extern void TIMERS_HARDWARE_INIT(void);
 extern void TIMERS_SOFTWARE_INIT(void);
 extern void TimersHUpdate(void);
 extern void TimersMsInterupt(void);
  extern void TimersSInterupt(void);
 extern volatile uint16_t TimersMsIntCount;
 extern volatile uint16_t TimersSIntCount;
 extern uint16_t	Timers_h[];
 extern uint16_t	Timers_s[];
 extern uint16_t	Timers_100ms[];

#endif
