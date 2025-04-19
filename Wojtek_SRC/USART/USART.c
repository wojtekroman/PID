#include "USART.h"

void USARTSoftwareInit (Usart_t *Usartx)
{
	if (Usartx->sreg & USART_INIT)
	{

		Usartx->USARTX = DEF_USART;
		Usartx->Usart_PORT = DEF_USART_PORT;
		Usartx->usart_rx_pin = DEF_USART_RX_PIN;
		Usartx->usart_tx_pin = DEF_USART_TX_PIN;
		Usartx->BaudRate = DEF_BAUDRATE;
		Usartx->RxCounter = 0;
		Usartx->TxCounter = 0;
		Usartx->SendedCounter=0;		//
		Usartx->sreg &= ~USART_INIT;
	}


}

void UsartGpioInit(Usart_t *Usart)
{
	GPIO_InitTypeDef GPIO_InitStruct;

	if (Usart->Usart_PORT == GPIOA)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	else if (Usart->Usart_PORT == GPIOB)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	else if (Usart->Usart_PORT == GPIOC)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

	GPIO_InitStruct.GPIO_Pin = Usart->usart_rx_pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	#ifdef STM32F103C8
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	#endif

	GPIO_Init(Usart->Usart_PORT, &GPIO_InitStruct);

	GPIO_InitStruct.GPIO_Pin = Usart->usart_tx_pin;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_2MHz;

	#ifdef STM32F103C8
		GPIO_InitStruct.GPIO_Mode = GPIO_Mode_AF_PP;
	#endif

	GPIO_Init(Usart->Usart_PORT, &GPIO_InitStruct);



}

void UsartHWInit(Usart_t *Usartx)
{
	USART_InitTypeDef USART_InitStructure;

	if (Usartx->USARTX == USART1)
		RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1,ENABLE);


	/* USART1 configuration ------------------------------------------------------*/
	/* USART1 configured as follow:
		        - BaudRate = 115200 baud
		        - Word Length = 8 Bits
		        - One Stop Bit
		        - No parity
		        - Hardware flow control disabled (RTS and CTS signals)
		        - Receive and transmit enabled
		        - USART Clock disabled
		        - USART CPOL: Clock is active low
		        - USART CPHA: Data is captured on the middle
		        - USART LastBit: The clock pulse of the last data bit is not output to
		                         the SCLK pin
		  */
		  USART_InitStructure.USART_BaudRate = Usartx->BaudRate;// 115200;
		  USART_InitStructure.USART_WordLength = USART_WordLength_8b;
		  USART_InitStructure.USART_StopBits = USART_StopBits_1;
		  USART_InitStructure.USART_Parity = USART_Parity_No;
		  USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
		  USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;

		  USART_Init(USART1, &USART_InitStructure);
		  // W³¹cz przerwanie dla odbioru danych (RXNE)
		  USART_ITConfig(Usartx->USARTX, USART_IT_RXNE, ENABLE);

		  USART_ITConfig(Usartx->USARTX, USART_IT_TC, ENABLE);


		  /* Enable USART1 */
		  USART_Cmd(Usartx->USARTX, ENABLE);

		  // W³¹cz przerwanie w NVIC
		  NVIC_EnableIRQ(USART1_IRQn);

}





static void NVIC_Configuration(Usart_t *Usartx)
{
  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the USARTx Interrupt */
  if (Usartx->USARTX == USART1)
	  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQn;

  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
}
void USARTHardwareInit(Usart_t *Usartx)
{
	UsartGpioInit(Usartx);
	UsartHWInit(Usartx);
	NVIC_Configuration(Usartx);



}



void USARTSendBuf(Usart_t *Usartx)
{
	//&Temp_Usart = Usartx;

	if (Usartx->TxCounter > 0)
	{
		Usartx->sreg |= USART_TRANSMIT_BUF;
		Usartx->USARTX->DR = Usartx->TxBuffer[Usartx->SendedCounter];
		Usartx->SendedCounter++;
	}
}



void USART1_IRQHandler(void)
{
	unsigned char temp;
    if ((USART1->SR & ( USART_FLAG_RXNE )) != (u16)RESET)
	{

    	PID_Usart.RxBuffer[PID_Usart.RxCounter] =USART_ReceiveData(USART1);
    	PID_Usart.RxCounter++;
    	PID_Usart.sreg |= USART_NEW_DATA;
		USART1->SR &= ~USART_FLAG_RXNE;
	}

    if ((USART1->SR & USART_FLAG_TC) ==(u16)USART_FLAG_TC)
    {
    	if (PID_Usart.sreg & USART_TRANSMIT_BUF)
    	{
    		if (PID_Usart.SendedCounter < PID_Usart.TxCounter)
    		{
    			PID_Usart.USARTX->DR = PID_Usart.TxBuffer[PID_Usart.SendedCounter];
    			PID_Usart.SendedCounter++;
    		}
    		else {
    			PID_Usart.TxCounter = 0;
    			PID_Usart.SendedCounter=0;
    			PID_Usart.sreg &= ~USART_TRANSMIT_BUF;

			}



    	}
    	//PID_Usart.sreg |= USART_BUF_EMPTY;
    	USART1->SR &= ~USART_FLAG_TC;
    }
}

