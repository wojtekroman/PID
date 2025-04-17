
#define LED_TIMER_DELAY_S  2

#ifdef STM32F10X_MD
	#include "stm32f10x_gpio.h"
	#include "stm32f10x_rcc.h"
#endif

#define LED_PORT 	GPIOC
#define LED_PIN	 	GPIO_Pin_13
#define LED_ON		(LED_PORT->ODR &= (~LED_PIN))
#define LED_OFF		(LED_PORT->ODR |= LED_PIN)
#define LED_TOGLE	(LED_PORT->ODR ^= LED_PIN)


#ifdef LED_C






#else
	extern void LED_HARDWARE_INIT(void);

#endif
