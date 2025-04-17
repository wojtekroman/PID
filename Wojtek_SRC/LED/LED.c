
#define LED_C
#include "LED.h"


#undef LED_C





void LED_HARDWARE_INIT(void)
{
	GPIO_InitTypeDef GPIO_InitStruct;
	//*********************  CLOCK ACTIVATION **********************

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	// ******************** GPIO CONFIGURATION *********************
	GPIO_InitStruct.GPIO_Pin = LED_PIN;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_10MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_Out_PP;


	GPIO_Init(LED_PORT, &GPIO_InitStruct);



}
