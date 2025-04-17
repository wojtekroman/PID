#define MAIN_C

 #include "stm32f10x_gpio.h"
 #include "main.h"
 #include "timers.h"
 #include "LED.h"
 //#include "RS_232.h"
 //#include "CAN_Volvo.h"
// #include "eeprom.h"
 //#include "OLED.h"
 //#include "DevID.h"
 //#include "ModuleID.h"
 //#include "PowerFail.h"
 //#include "AT24C08.h"
 //#include "Relays.h"
 #include "WDG.h"
 //#include "ADC_Power_chceck.h"
 //#include "ntc4k7.h"
 //#include "HC595.h"
 #include "PWM.h"
#undef MAIN_C




uint8_t FIND_Last_data(void);

int main(void)
{
 static uint8_t StateMachine=0, temp8;
 static uint8_t *RSPointer;
 uint16_t temp16, counter=0;
 uint32_t temp32, temp_hc595;
 uint32_t *temp32_p;
 RSPointer=0;
 static uint8_t StringBuffer[30];
 static uint8_t i, digit;
 //static WR_CanRxMsg TempCAN;
 uint8_t TempString[10];



 while(1)
 {
	 switch (StateMachine)
	  {





	 	 case HARDWARE_INIT_STATE:
	 	 {

	 		 if (HARDWARE_INIT())
	 		 {
	 			 StateMachine++;
	 		 }
	 		 break;
	 	 }

	 	 case SOFTWARE_INIT_STATE:
	 	 {
	 		 if (SOFTWARE_INIT())
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
	 		TimersHUpdate();

	 		StateMachine++;

	 		break;
	 	 }

	 	 case GET_DEV_ID_STATE:
	 	 {

	 	//	if (CANTx.ExtId == CAN_NO_FRAME_ID)
	 /*		{
	 			temp8=GET_DEVICE_ID();


		 				if ((uint32_t)temp8 != ((CAN_DEV_ID>> 4 ) &0xFF))
		 				{
		 					temp32 = (1 << HC595_CAN1_REL_ACT_POS);


		 					//if (*(HC595_buffer.BufferPointer) & temp32)
		 					switch (temp8)
		 					{
		 						case 1:
		 						{
		 							*(HC595_buffer.BufferPointer) = 0;
		 							if (counter >= 32)
		 								{
		 									counter = 0;
		 								}
		 							SET_BIT_HC595(HC595_buffer.BufferPointer, counter);
		 							counter++;



		 							//TOGLE_BIT_HC595(HC595_buffer.BufferPointer, HC595_CAN1_REL_ACT_POS);

		 							// display HC595 buffer at lCD

			 					 			digit = HC595_buffer.buffer[0]&0xF0;
			 					 			digit >>= 4;
			 					 			digit += 0x30;
			 					 			OLEDBuferPutChar(&LCD_OLED, 32, 0, &digit);		// no OLED
			 					 			digit = HC595_buffer.buffer[0]&0x0F;
			 					 			digit += 0x30;
			 					 			OLEDBuferPutChar(&LCD_OLED, 40, 0, &digit);		// no OLED

			 					 			digit = HC595_buffer.buffer[1]&0xF0;
			 					 			digit >>= 4;
			 					 			digit += 0x30;
			 					 			OLEDBuferPutChar(&LCD_OLED, 48, 0, &digit);		// no OLED
			 					 			digit = HC595_buffer.buffer[1]&0x0F;
			 					 			digit += 0x30;
			 					 			OLEDBuferPutChar(&LCD_OLED, 56, 0, &digit);		// no OLED

			 					 			digit = HC595_buffer.buffer[2]&0xF0;
			 					 			digit >>= 4;
			 					 			digit += 0x30;
			 					 			OLEDBuferPutChar(&LCD_OLED, 64, 0, &digit);		// no OLED
			 					 			digit = HC595_buffer.buffer[2]&0x0F;
			 					 			digit += 0x30;
			 					 			OLEDBuferPutChar(&LCD_OLED, 72, 0, &digit);		// no OLED

			 					 			digit = HC595_buffer.buffer[3]&0xF0;
			 					 			digit >>= 4;
			 					 			digit += 0x30;
			 					 			OLEDBuferPutChar(&LCD_OLED, 80, 0, &digit);		// no OLED
			 					 			digit = HC595_buffer.buffer[3]&0x0F;
			 					 			digit += 0x30;
			 					 			OLEDBuferPutChar(&LCD_OLED, 86, 0, &digit);		// no OLED


			 					 			LCD_OLED.sreg |= OLED_REFRESH;




		 							HC595_buffer.sreg |= HC595_DATA_REFRESH_REQ;
		 							break;
		 						}
		 						case 2:
		 						{
		 							//TOGLE_BIT_HC595(HC595_buffer.BufferPointer, HC595_FAN_ACT_POS);
		 							HC595_LATCH_DISABLE;

		 							HC595_RESET_ENABLE;
		 							counter=0;
		 							HC595_RESET_DISABLE;
		 							HC595_LATCH_ENABLE;
		 							HC595_buffer.sreg |= HC595_DATA_REFRESH_REQ;
		 							break;

		 						}
		 						case 4:
		 						{
		 							TOGLE_BIT_HC595(HC595_buffer.BufferPointer, HC595_CAN2_REL_ACT_POS);
		 							HC595_buffer.sreg |= HC595_DATA_REFRESH_REQ;
		 							break;

		 						}
		 						default:
		 						{
		 							break;
		 						}
		 					}

		 				}


	 			if (temp8 != ((CAN_DEV_ID>> 4 ) &0xFF))
	 			{

	 				//
	 				if (temp8/10)
	 				{
	 					digit = temp8/10;
	 					digit += 0x30;

	 					OLEDBuferPutChar(&LCD_OLED, 8, 0, &digit);		// no OLED

	 					digit = (temp8%10)+0x30;
	 				}
	 				else {
	 					OLEDBuferPutChar(&LCD_OLED, 8, 0, &"0");	// no OLED

	 					digit = temp8;
						digit += 0x30;
					}

	 				OLEDBuferPutChar(&LCD_OLED, 16, 0, &digit);	// no OLED

	 				LCD_OLED.sreg |= OLED_REFRESH;

	 			}
	 			CAN_DEV_ID = CAN_BASE_ID | ((uint64_t)(temp8 << 4)) ;
	 			CANTx.ExtId = CAN_DEV_ID ;


	 		}*/

	 		StateMachine++;

	 		break;
	 	 }

	 	 case CAN_READ_STATE:
	 	 {
 	/*	if (CAN_sreg & CAN_NEW_DATA_READ)
	 		{
 			TempCAN = CANRx[CAN_data_number-1];
 				if (TempCAN.ExtId == CAN_DEV_ID)
 				{

 					jumpers.jumper1 = 0;
 					jumpers.jumper1 = TempCAN.Data[0];
 					jumpers.jumper1 |= (uint16_t)((TempCAN.Data[1] & RELAYS_BIT_MASK) << 8);

 					jumpers.jumper2 = 0;
 					jumpers.jumper2 = TempCAN.Data[2];
 					jumpers.jumper2 |= (uint16_t)((TempCAN.Data[3] & RELAYS_BIT_MASK) << 8);

 					if (( Relays_sreg & RELAYS_BIT_MASK) != TempCAN.Data[4])
 					{
 						 Relays_sreg |= TempCAN.Data[4] & RELAYS_BIT_MASK;
 						 Relays_sreg |= UPDATE_STATUS;
 					}
 				}


	 		}*/
	 		StateMachine++;
	 		break;
	 	 }

	 	 case CAN_SEND_STATE:
	 	 {
	 	/*	if (CAN_sreg & CAN_NEW_DATA_SEND)//(CANTx.ExtId != 0 )

	 		{

	 			CAN_Transmit(CAN1,&CANTx );
 				CAN_sreg &= ~CAN_NEW_DATA_SEND;
 				CAN_TX_ENA();
 				//CANTx.ExtId = 0;
	 		}
*/
	 		StateMachine++;
	 		break;
	 	 }

	 	 case LED_STATE:
	 	 {
#ifdef LED_AS_HEARTBIT

			 if (Timers_s[TIMER_LED] == TIMER_STOP)
	 		 {
	 			LED_OFF;
	 			Timers_s[TIMER_LED] = LED_TIMER_DELAY_S;
	 		 }
	 		 else if (Timers_s[TIMER_LED] == TIMER_END)
	 		 {
#endif
#ifdef LED_AS_RS_TIMER_HARTBIT
	 		 if (GET_RS232_TIME(RS_Time))
	 		 {
	 		  RS_Time=0;
#endif
#ifdef CAN_TX_LED
	 			CANTx.ExtId = 0x01234567;
	 			CANTx.DLC = 8;
	 			i=0;
	 			do
	 			{
	 				CANTx.Data[i] = 0x30+i;
	 					i++;
 				}while ( i < 8 );
	 			CAN_sreg |= CAN_NEW_DATA_SEND;
#endif

#ifdef LED_AS_HEARTBIT

	 			Timers_s[TIMER_LED] = LED_TIMER_DELAY_S;
#endif

#ifdef LED_AS_RFID_IND
	 			if ((RFID2.time < GET_RS232_TIME(RS_Time)) && ((GET_RS232_TIME(RS_Time) - RFID2.time) < 200))
	 			{
	 				LED_ON;
	 			}
	 			else
	 			{
	 				if ((RFID1.time < GET_RS232_TIME(RS_Time)) && ((GET_RS232_TIME(RS_Time) - RFID1.time) < 200))
	 				{
	 					LED_ON;
	 				}
	 				else
	 				{

	 					LED_OFF;

					}
	 				LED_OFF;

	 			}


#else
	 			LED_TOGLE;
	 		 }//else if (Timers_s[TIMER_LED] == TIMER_END)
#endif

#ifdef LED_AS_RS_TIMER_HARTBIT
	 		}
#endif

	 		StateMachine++;
		 	break;

	 	 }	//case LED_STATE:

	 	 case POWER_SUPLY_CHECK_STATE:
	 	 {

	 		 StateMachine++;
			 break;
	 	 }
	 	 case POWER_FAIL_STATE:
	 	 {


	 		 StateMachine++;
			 break;
	 	 }
	 	 case LCD_STATE:
	 	 {

	 		 StateMachine++;
			 break;

	 	 }
	 	 case EEPROM_SATE:
	 	 {


	 		 StateMachine++;
			 break;
	 	 }
	 	 case FAN_STATE:
	 	 {
	 		 if (Timers_100ms[TIMER_FAN_SPEED_CHANGE] == TIMER_STOP)
	 		 {
	 			Timers_100ms[TIMER_FAN_SPEED_CHANGE] =1; //PWM_Unit.FanSpeedChangeDelayValue;
	 		 }
	 		 if (!Timers_100ms[TIMER_FAN_SPEED_CHANGE]  )
	 		 {


	 			PWM_Unit.TIMx->CCR3 =(uint16_t)PWM_SET_FREQ(PWM_Unit.Filling);
	 			Timers_100ms[TIMER_FAN_SPEED_CHANGE] = 1; //PWM_Unit.FanSpeedChangeDelayValue;
	 		 }
	 		 StateMachine++;

	 		 break;
	 	 }
	 	 case HC595_STATE:
	 	 {



	 		 StateMachine++;
	 		 break;
	 	 }
	 	 default:
	 	 {

	 		 StateMachine = TIMERS_STATE;

	 	 }
	  }	//  switch (StateMachine)




 }		// while (1)


return 0;
}

// ************* HARDWARE FUNCTION INITIALIZATION **********************

uint8_t HARDWARE_INIT(void)
{


	LED_HARDWARE_INIT();
	IWDG_HardwareInit();
	TIMERS_HARDWARE_INIT();
	PWM_HardwareInit();
	return TRUE;
}

// ************* SOFTWARE FUNCTION INITIALIZATION **********************
uint8_t SOFTWARE_INIT(void)
{
	TIMERS_SOFTWARE_INIT();
	PWM_SoftwareInit();

	return TRUE;
}

//****************** FLASH INIT *******************************************

void   FLASH_INIT(void)
{
	uint8_t data;
	// to work with flash, first we have to unlock it
	  FLASH_Unlock();
	  EE_Init();
	  data = FIND_Last_data();


}

// ********************** function to find last saved data **********************

uint8_t FIND_Last_data(void)
{
	uint16_t Eeprom_Value, status;
	uint16_t Address=1234;
	status = EE_ReadVariable(Address, &Eeprom_Value);
		if (Eeprom_Value == 0xFFFF)
		{
			status = EE_WriteVariable(Address, 1);

		}
		else
		{
			status = EE_WriteVariable(Address, ++Eeprom_Value);

		}


}
