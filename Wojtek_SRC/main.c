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
 //#include "WDG.h"
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

 RSPointer=0;
 static uint8_t StringBuffer[30];
 static uint8_t digit;
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
		 	//IWDG_Reload();
	 		TimersMsInterupt();
	 		TimersSInterupt();
	 		//TimersHUpdate();

	 		StateMachine++;

	 		break;
	 	 }



	 	 case CAN_READ_STATE:
	 	 {

	 		StateMachine++;
	 		break;
	 	 }

	 	 case CAN_SEND_STATE:
	 	 {

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

