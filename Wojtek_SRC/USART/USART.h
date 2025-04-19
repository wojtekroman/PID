#ifndef USART_H
#define USART_H

	#ifdef STM32F103C8
		#include "stm32f10x.h"
		#include "stm32f10x_gpio.h"
		#include "stm32f10x_rcc.h"
		#include "stm32f10x_usart.h"
		#include "misc.h"
	#endif

	#define DEF_USART			USART1
	#define DEF_USART_RX_PIN		GPIO_Pin_10
	#define DEF_USART_TX_PIN		GPIO_Pin_9

	#define DEF_USART_PORT			GPIOA
	#define BUFFER_SIZE			20
	#define DEF_BAUDRATE		9600
	/// ************************* SREG bits **************
	#define USART_INIT				0x01
	#define USART_NEW_DATA			0x02
	#define USART_TRANSMIT_NEEDED	0x04
	#define USART_TRANSMIT_BUF		0x08
	//#define USART_BUF_EMPTY			0x10


	typedef struct {
		USART_TypeDef *USARTX;
		uint16_t usart_rx_pin;
		uint16_t usart_tx_pin;
		GPIO_TypeDef *Usart_PORT;
		uint8_t RxBuffer[BUFFER_SIZE];
		uint8_t TxBuffer[BUFFER_SIZE];
		uint8_t RxCounter;
		uint8_t TxCounter;
		uint8_t SendedCounter;

		uint16_t BaudRate;

		uint8_t sreg;

	}Usart_t;


	Usart_t PID_Usart;
	static Usart_t Temp_Usart;

#endif
