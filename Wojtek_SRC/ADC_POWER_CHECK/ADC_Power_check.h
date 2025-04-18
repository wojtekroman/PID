#ifndef ADC_POWER_CHECK_H
#define ADC_POWER_CHECK_H

#ifdef STM32F103C8
#include  "stm32f10x.h"
#include "stm32f10x_adc.h"
#endif

typedef enum{
	ADC_OK=0,
	ADC_ERROR
}ADC_InitStatusType ;

typedef struct {
	uint8_t sreg;
	uint16_t PowerValue;
	uint16_t ADC_Calibration;
	ADC_TypeDef *ADCx;
	uint32_t Voltage;
	uint32_t MAX_Voltage;

}PowerCheck_t;

#define ADC_NO_DATA	0xFFFF
#define ADC_TIMEOUT_VALUE	0xFFFF
// *************** SREG bits **********************
#define POWER_NEW_VOLTAGE	0x01
#define ADC_POINT_POSITION	1500
#define POWER_FAIL_BORDER	(ADC_POINT_POSITION/2)		// input has a V divider



#ifdef STM32F103C8
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "misc.h"
#endif


	#define ADC_REF_VOLTAGE		33000
	#define ADC_MAX_VALIE		4096
	#define ADC_DELTA_VOLTAGE	((ADC_REF_VOLTAGE/ADC_MAX_VALIE))

	#define ADC_FILTER_VALUE	100
	#define MAX_VOLTAGE			ADC_REF_VOLTAGE			// Remember about voltage devider

	#define ADC_TIME_OUT				0xFFFF
	#define CFGR_ADCPRE_Reset_Mask    ((uint32_t)0xFFFF3FFF)

	#define SUPPLY_CHECK_PIN			GPIO_Pin_0		// Power --- 21kMOhm ---- A0 -----21k ---GND
	#define SUPPLY_CHECK_PORT			GPIOA

	#define SUPPLY_CHECK_CLOCK			RCC_APB2Periph_GPIOA
	#define SUPPLY_CHECK_RCC_ACTIVE		RCC_APB2PeriphClockCmd(SUPPLY_CHECK_CLOCK, ENABLE)

	#define PowerCheckADCChanel			ADC_Channel_0


	/*#if SUPPLY_CHECK_PIN == GPIO_Pin_0					error (GPIO_Pin_0 is (uint16_t) 0x00001 #if don't understand it
		#define PowerCheckADCChanel			ADC_Channel_0

	#elif SUPPLY_CHECK_PIN == GPIO_Pin_1
		#define PowerCheckADCChanel			ADC_Channel_1

	#elif SUPPLY_CHECK_PIN == GPIO_Pin_2
		#define PowerCheckADCChanel			ADC_Channel_2

	#elif SUPPLY_CHECK_PIN == GPIO_Pin_3
		#define PowerCheckADCChanel			ADC_Channel_3

	#elif SUPPLY_CHECK_PIN == GPIO_Pin_4
		#define PowerCheckADCChanel			ADC_Channel_4
	#endif
*/



uint16_t PowerCheckHardwareInit(PowerCheck_t *Power);
void PowerCheckADCHardwareInit(ADC_TypeDef *ADCx, uint16_t *Calibration_value);
void PowerCheckVoltageCount (PowerCheck_t *Power);
volatile PowerCheck_t PowerSupply;
#endif

