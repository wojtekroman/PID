
#ifdef STM32F10X_MD
	#include "stm32f10x_rcc.h"
	#include "stm32f10x_gpio.h"
	#include "stm32f10x_tim.h"


	#define FAN_DEF_ON_SPEED 			((FAN_MAX_SPED + FAN_MIN_SPEED)/2)	//800






#define PWM_TIMER			TIM1


#define PWM_PORT 			GPIOA
#define PWM_PIN	 			GPIO_Pin_7
#define PWM_NPIN	 		GPIO_Pin_8


typedef struct
{
	TIM_TypeDef* TIMx;
	uint16_t	Freq;
	uint16_t	Filling;
	uint8_t		sreg;

}PWMType;


// **********************************************
//	**************** sreg definition ************
// **********************************************



//	**************** end sreg definition ************
#define GET_INT_FOR_FILLING(MAX, PERC)		((uint16_t)((uint64_t)MAX -	((uint64_t)MAX * (uint64_t)PERC) / 1000))



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

#define FAN_PORT_RCC_ON				RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE)

#ifdef FAN_PWM_C



	#define FAN_DEF_FREQ				4
	#define FAN_DEF_INTENSIVITY			(FAN_DEF_FREQ/2)			// RANGE FROM 0 UP TO 1000
	#define FAN_DEF_SREG				0			// RANGE FROM 0 UP TO 1000




	FANType FAN_drawer;

	void FAN_GPIOHardwareInit(void);
	void FAN_TimerForPWMHardwareInit(FANType *LedLampX);


#else
	extern void FAN_HardwareInit(void);
	extern void FAN_SoftwareInit(void);
	extern FANType FAN_drawer;

#endif
