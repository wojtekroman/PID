

 #include "stm32f10x_gpio.h"
 #include "main.h"
 #include "timers.h"
 #include "LED.h"
 #include "ADC_Power_check.h"
 #include "PWM.h"
 #include "WDG.h"
 #include "OLED.h"
 #include "DataConversion.h"
 #include "PID.h"
 #include "USART.h"

// ****************** global variables *****************


// *****************************************************

int main(void)
{
 static uint8_t StateMachine=0, temp8;
 static uint8_t *RSPointer;

 RSPointer=0;
 static uint8_t StringBuffer[30];
 static uint8_t digit;
 int i=0, j=0;
 uint8_t TempString[10];


 uint16_t temp_16=0;
 //pid_capacitor.sreg |= PID_INIT;
 while(1)
 {
	 switch (StateMachine)
	  {
 	 	 case SOFTWARE_INIT_STATE:
 	 	 {
 		 	 if (SOFTWARE_INIT())
 		 	 {
 			 	 StateMachine++;
 		 	 }
 		 	 break;
 	 	 }

	 	 case HARDWARE_INIT_STATE:
	 	 {

	 		 if (HARDWARE_INIT())
	 		 {
	 			 StateMachine++;
	 		 }
	 		 break;
	 	 }



	 	 case TIMERS_STATE:
	 	 {
		 	IWDG_Reload();
	 		TimersMsInterupt();
	 		TimersSInterupt();


	 		StateMachine++;

	 		break;
	 	 }



	 	 case USART_READ_STATE:
	 	 {


	 		 if ((PID_Usart.RxBuffer[PID_Usart.RxCounter-2] == 0x0a) || (PID_Usart.RxBuffer[PID_Usart.RxCounter-1] == 0x0d ))
	 		 {
	 			j=0;
	 			TempString[0]='@';
	 			TempString[1]=0;
	 			i= FindString(PID_Usart.RxBuffer, TempString );
	 			if (i!= 0xFF)
	 				i++;
	 			else{
	 				i=0;
	 			}
	 			switch ((PID_Usart.RxBuffer[i]& (~0x20)))
	 			{
	 				case 'T':
	 				{

		 				i+=2;
		 				do{
		 					TempString[j]= PID_Usart.RxBuffer[j+i];
		 					j++;
		 				}while (PID_Usart.RxBuffer[j+i]>=0x30 && PID_Usart.RxBuffer[j+i]<=0x39);				// while digits are in the buffer

		 				TempString[j]=0;
		 				pid_capacitor.target = (uint16_t) STRING_TO_DEC_(TempString);
		 				pid_capacitor.sreg |= PID_RESET;
		 				PID_TargetInit(&pid_capacitor, pid_capacitor.target);
		 				PID_Usart.RxBuffer[0]=0;
		 				PID_Usart.RxCounter=0;
		 				pid_capacitor.sreg |=PID_TARGET_PRINT;
		 				break;

	 				}
	 				case 'P':
	 				{
	 					i+=2;
	 					j=0;
	 					do{
	 						TempString[j]= PID_Usart.RxBuffer[j+i];
	 					 	j++;
	 					}while (PID_Usart.RxBuffer[j+i]>=0x30 && PID_Usart.RxBuffer[j+i]<=0x39);				// while digits are in the buffer

	 					TempString[j]=0;
	 					pid_capacitor.Kp = (uint16_t) STRING_TO_DEC_(TempString);
	 					pid_capacitor.sreg |= PID_RESET;
	 					PID_NewUint16Init(&pid_capacitor, &(pid_capacitor.Kp), pid_capacitor.Kp);
	 					//PID_KpInit(&pid_capacitor, pid_capacitor.Kp);
	 					pid_capacitor.sreg |=PID_KP_PRINT;
	 					PID_Usart.RxBuffer[0]=0;
	 					PID_Usart.RxCounter=0;
	 					break;

	 				}
	 				case 'I':
	 				{
	 					i+=2;
	 					j=0;
	 					do{
	 						TempString[j]= PID_Usart.RxBuffer[j+i];
	 					 	j++;
	 					}while (PID_Usart.RxBuffer[j+i]>=0x30 && PID_Usart.RxBuffer[j+i]<=0x39);				// while digits are in the buffer

	 					TempString[j]=0;
	 					pid_capacitor.Ki = (uint16_t) STRING_TO_DEC_(TempString);
	 					pid_capacitor.sreg |= PID_RESET;
	 					PID_NewUint16Init(&pid_capacitor, &(pid_capacitor.Ki), pid_capacitor.Ki);
	 					PID_Usart.RxBuffer[0]=0;
	 					PID_Usart.RxCounter=0;
	 					pid_capacitor.sreg |=PID_KI_PRINT;
	 					break;

	 				}
	 				case 'D':
	 				{
	 					i+=2;
	 					j=0;
	 					do{
	 						TempString[j]= PID_Usart.RxBuffer[j+i];
	 					 	j++;
	 					}while (PID_Usart.RxBuffer[j+i]>=0x30 && PID_Usart.RxBuffer[j+i]<=0x39);				// while digits are in the buffer

	 					TempString[j]=0;
	 					pid_capacitor.Kd = (uint16_t) STRING_TO_DEC_(TempString);
	 					pid_capacitor.sreg |= PID_RESET;
	 					PID_NewUint16Init(&pid_capacitor, &(pid_capacitor.Kd), pid_capacitor.Kd);
	 					PID_Usart.RxBuffer[0]=0;
	 					PID_Usart.RxCounter=0;
	 					pid_capacitor.sreg |=PID_KD_PRINT;
	 					break;

	 				}

	 				case 'C':
	 				{
	 					i+=2;
	 					j=0;
	 					do{
	 						TempString[j]= PID_Usart.RxBuffer[j+i];
	 					 	i++;
	 					}while (PID_Usart.RxBuffer[j+i]>=0x30 && PID_Usart.RxBuffer[j+i]<=0x39);				// while digits are in the buffer

	 					TempString[j]=0;
	 					pid_capacitor.dt= (uint16_t) STRING_TO_DEC_(TempString);
	 					pid_capacitor.sreg |= PID_RESET;
	 					PID_NewUint16Init(&pid_capacitor, &(pid_capacitor.dt), pid_capacitor.dt);
	 					PID_Usart.RxBuffer[0]=0;
	 					PID_Usart.RxCounter=0;
	 					pid_capacitor.sreg |=PID_DT_PRINT;
	 					break;

	 				}
	 				case 0x1F://'?':
	 				{
	 					pid_capacitor.sreg |= PID_PRINT_MASK;
	 				}

	 			}
	 		 }
	 		StateMachine++;
	 		break;
	 	 }

	 	 case USART_SEND_STATE:
	 	 {
	 		 if ( (pid_capacitor.sreg & PID_PRINT_MASK) && (!PID_Usart.SendedCounter))
	 		 {
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = 'K';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = 'p';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = '=';
	 			 PID_Usart.TxCounter += U16_DEC_TO_STRING(pid_capacitor.Kp, &(PID_Usart.TxBuffer[PID_Usart.TxCounter]));
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = ';';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = ' ';

	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = 'K';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = 'i';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = '=';
	 			 PID_Usart.TxCounter += U16_DEC_TO_STRING(pid_capacitor.Ki, &(PID_Usart.TxBuffer[PID_Usart.TxCounter]));
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = ';';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = ' ';

	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = 'K';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = 'd';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = '=';
	 			 PID_Usart.TxCounter += U16_DEC_TO_STRING(pid_capacitor.Kd, &(PID_Usart.TxBuffer[PID_Usart.TxCounter]));
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = ';';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = ' ';

	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = 'D';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = 't';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = '=';
	 			 PID_Usart.TxCounter += U16_DEC_TO_STRING(pid_capacitor.dt, &(PID_Usart.TxBuffer[PID_Usart.TxCounter]));
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = ';';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = ' ';

	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = 'T';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = 'a';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = '=';
	 			 PID_Usart.TxCounter += U16_DEC_TO_STRING(pid_capacitor.target, &(PID_Usart.TxBuffer[PID_Usart.TxCounter]));
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = ';';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = ' ';
	 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = '\n';

	 			 USARTSendBuf(&PID_Usart);
	 			 pid_capacitor.sreg &= ~PID_PRINT_MASK;
	 			 PID_Usart.sreg |= USART_TRANSMIT_BUF;
	 		 }

	 		 if( (PID_Usart.sreg & USART_TRANSMIT_BUF) && (!PID_Usart.SendedCounter) )
	 		 {
	 			USARTSendBuf(&PID_Usart);
	 		 }
	 		StateMachine++;
	 		break;
	 	 }

	 	 case LED_STATE:
	 	 {


			 if (Timers_s[TIMER_LED] == TIMER_STOP)
	 		 {
	 			LED_OFF;
	 			Timers_s[TIMER_LED] = LED_TIMER_DELAY_S;
	 		 }
	 		 else if (Timers_s[TIMER_LED] == TIMER_END)
	 		 {
	 			 Timers_s[TIMER_LED] = LED_TIMER_DELAY_S;
	 			 LED_TOGLE;
	 		 }//else if (Timers_s[TIMER_LED] == TIMER_END)
	 		 StateMachine++;
		 	 break;

	 	 }	//case LED_STATE:

	 	 case POWER_SUPLY_CHECK_STATE:
	 	 {
	 		 if (PowerSupply.sreg & POWER_NEW_VOLTAGE)
	 		 	 {
	 			 	 PowerCheckVoltageCount(&PowerSupply);

	 			 	 PowerSupply.sreg &= ~POWER_NEW_VOLTAGE;

	 		 	 }
	 		 StateMachine++;
			 break;
	 	 }
	 	 case PID_STATE:
	 	 {
	 		if (pid_capacitor.sreg & PID_INIT)

	 			{					//		Kp   Ki  Kd	 dt	min	max  	target
	 				PID_Init(&pid_capacitor, 500, 10, 4, 1, 1, 10000, pid_capacitor.target);		// Kx *100 dt in ms target in mV
					pid_capacitor.sreg &= ~PID_INIT;
	 			}




	 		 StateMachine++;
			 break;
	 	 }

	 	 case PWM_STATE:
	 	 {
	 		 if (Timers_100ms[TIMER_FAN_SPEED_CHANGE] == TIMER_STOP)
	 		 {
	 			Timers_100ms[TIMER_FAN_SPEED_CHANGE] =pid_capacitor.dt; //PWM_Unit.FanSpeedChangeDelayValue;
	 		 }
	 		 if (!Timers_100ms[TIMER_FAN_SPEED_CHANGE]  )
	 		 {

	 	 		temp_16 = (uint16_t)PID_Compute(&pid_capacitor, pid_capacitor.target, (uint16_t)PowerSupply.Voltage);
	 	 		PWM_Unit.Fulfillment= (uint16_t)(((uint32_t)PWM_Unit.Freq * (uint32_t)temp_16)/(pid_capacitor.output_max));
	 	 		PWM_Unit.TIMx->CCR1 =(uint16_t)PWM_Unit.Fulfillment;
	 	 		Timers_100ms[TIMER_FAN_SPEED_CHANGE] =pid_capacitor.dt;
	 			if (!PID_Usart.TxCounter)
		 		 {
		 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = 'V';
		 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = '=';
		 			 PID_Usart.TxCounter += U16_DEC_TO_STRING(PowerSupply.Voltage, &(PID_Usart.TxBuffer[PID_Usart.TxCounter]));
		 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = '\n';
		 			 PID_Usart.TxBuffer[PID_Usart.TxCounter++] = '\r';
		 			 PID_Usart.sreg |= USART_TRANSMIT_BUF;
		 		 }
	 		 }
	 		 StateMachine++;

	 		 break;
	 	 }
	 	 case LCD_STATE:
	 	 {

	 		if (LCD_OLED.sreg & OLED_REFRESH)
	 			{
	 				INT_dec_2STR(PowerSupply.Voltage, TempString);
	 				for (i =0; i<4; i++)
	 				{
	 					OLEDBuferPutChar(&LCD_OLED, (i*8), 0, (TempString+i));
	 				}
	 				//OLEDWriteText(&LCD_OLED, TempString);

	 			}
	 		StateMachine++;
	 		break;
	 	 }

	 	 default:
	 	 {

	 		 StateMachine = SOFTWARE_INIT_STATE;

	 	 }
	  }	//  switch (StateMachine)




 }		// while (1)


return 0;
}

// ************* HARDWARE FUNCTION INITIALIZATION **********************

uint8_t HARDWARE_INIT(void)
{
	static uint8_t HWInitStatus=0;
	if (!HWInitStatus)
	{
		LED_HARDWARE_INIT();
		IWDG_HardwareInit();
		TIMERS_HARDWARE_INIT();
		PWM_HardwareInit();
		PowerCheckHardwareInit(&PowerSupply);
		OLEDHardwareInit();
		OLEDDisplayInit(&LCD_OLED);
		OLEDDisplayCleare(&LCD_OLED);
		USARTHardwareInit(&PID_Usart);
		HWInitStatus=1;
	}

	return HWInitStatus;
}

// ************* SOFTWARE FUNCTION INITIALIZATION **********************
uint8_t SOFTWARE_INIT(void)
{
	static uint8_t SWInitStatus=0;
	if (!SWInitStatus)
	{
		TIMERS_SOFTWARE_INIT();
		PowerCheckSoftwareInit(&PowerSupply);
		PID_DEF_Init(&pid_capacitor);
		OLEDSoftwareInit(&LCD_OLED);
		PID_Usart.sreg = USART_INIT;
		USARTSoftwareInit (&PID_Usart);
		SWInitStatus=1;

	}
	return SWInitStatus;
}
///


