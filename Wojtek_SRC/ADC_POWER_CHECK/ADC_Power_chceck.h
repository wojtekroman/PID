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

}PowerCheck_t;

#define ADC_NO_DATA	0xFFFF
#define ADC_TIMEOUT_VALUE	0xFFFF
// *************** SREG bits **********************
#define POWER_NEW_VOLTAGE	0x01
#define ADC_POINT_POSITION	10000
#define POWER_FAIL_BORDER	(ADC_POINT_POSITION*2)



#ifdef STM32F103C8
#include "stm32f10x_gpio.h"
#include "stm32f10x_rcc.h"
#include "stm32f10x_adc.h"
#include "misc.h"
#endif


	#define ADC_REF_VOLTAGE		33000
	#define ADC_MAX_VALIE		4096
	#define ADC_DELTA_VOLTAGE	((ADC_REF_VOLTAGE/ADC_MAX_VALIE)*10)

	#define ADC_FILTER_VALUE	800



	#define ADC_TIME_OUT				0xFFFF
	#define CFGR_ADCPRE_Reset_Mask    ((uint32_t)0xFFFF3FFF)

	#define SUPPLY_CHECK_PIN			GPIO_Pin_0
	#define SUPPLY_CHECK_PORT			GPIOA			// Power --- 1MOhm ---- A3 -----100k ---GND

	#define SUPPLY_CHECK_CLOCK			RCC_APB2Periph_GPIOA
	#define SUPPLY_CHECK_RCC_ACTIVE		RCC_APB2PeriphClockCmd(SUPPLY_CHECK_CLOCK, ENABLE)

	#define PowerCheckADCChanel			ADC_Channel_3

uint16_t PowerCheckHardwareInit(PowerCheck_t *Power);
void PowerCheckADCHardwareInit(ADC_TypeDef *ADCx, uint16_t *Calibration_value);
void PowerCheckVoltageCoutn (PowerCheck_t *Power);
volatile PowerCheck_t PowerSupply;
#endif
/*#else
extern void PowerCheckVoltageCoutn (PowerCheck_t *Power);
extern PowerCheckVoltageToString (PowerCheck_t *Power, uint8_t *string);
#endif
*/
