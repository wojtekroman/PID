#ifndef PWM_H
#define PWM_H
#ifdef STM32F10X_MD
	#include "stm32f10x_rcc.h"
	#include "stm32f10x_gpio.h"
	#include "stm32f10x_tim.h"

#endif
	#define PWM_MAIN_FREQ			1000
	#define PWM_DEF_ON_SPEED 		((FAN_MAX_SPED + FAN_MIN_SPEED)/2)	//800
	#define PWM_NEW_VALUE			0x01
	#define	PWM_DEF_FREQ			SystemCoreClock
	#define PWM_TOTAL_MAX_SPEED		72000000
	#define PWM_SET_FREQ(freq)		freq<=0 ? 1 : ((SystemCoreClock/freq)-1)


	#define PWM_TIMER			TIM1


#define PWM_PORT1 			GPIOB
#define PWM_PIN1	 		GPIO_Pin_13			// PB13 - CN3
#define PWM_NPIN1	 		GPIO_Pin_13			// PA9 - ch4

#define PWM_PORT2 			GPIOA
#define PWM_PIN2	 		GPIO_Pin_8
#define PWM_NPIN2	 		GPIO_Pin_14


typedef struct
{
	TIM_TypeDef* TIMx;
	uint16_t	Freq;
	uint16_t	Fulfillment;
	uint8_t		sreg;

}PWM_t;


// **********************************************
//	**************** sreg definition ************
// **********************************************



//	**************** end sreg definition ************
#define GET_INT_FOR_FULFILLMENT(MAX, PERC)		((uint16_t)((uint64_t)MAX -	((uint64_t)MAX * (uint64_t)PERC) / 1000))



#ifdef STM32F030X6
	#include "stm32f0xx_rcc.h"
	#include "stm32f0xx_tim.h"
#endif

#ifdef STM32F10X_MD

	#include "stm32f10x_rcc.h"
	#include "stm32f10x_tim.h"
	#include "misc.h"
#endif
// ******************* define default value for FAN *************************




	#define PWM_PORT1_RCC_ON				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE)
	#define PWM_PORT2_RCC_ON				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)


	#define PWM_DEF_INTENSIVITY			(FAN_DEF_FREQ/2)			// RANGE FROM 0 UP TO 1000
	#define PWM_DEF_SREG				PWM_NEW_VALUE			// RANGE FROM 0 UP TO 1000




	PWM_t PWM_Unit;

	void PWM_GPIOHardwareInit(void);
	void PWM_TimerForPWMHardwareInit(PWM_t *PWM_dev);
	void PWM_FillerUpdate (PWM_t *PWM_dev);

#endif
