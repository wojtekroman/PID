

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

// ****************** extern variables *****************


// *****************************************************

int main(void)
{
 static uint8_t StateMachine=0, temp8;
 static uint8_t *RSPointer;

 RSPointer=0;
 static uint8_t StringBuffer[30];
 static uint8_t digit;

 uint8_t TempString[10];
 PID_Controller pid_capacitor;
 uint16_t temp_16=0;

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
	 		 if (PowerSupply.sreg & POWER_NEW_VOLTAGE)
	 		 	 {
	 			 	 PowerCheckVoltageCount(&PowerSupply);
	 			 	 PowerSupply.Voltage /=10;
	 			 	 PowerSupply.sreg &= ~POWER_NEW_VOLTAGE;

	 		 	 }
	 		 StateMachine++;
			 break;
	 	 }
	 	 case PID_STATE:
	 	 {
	 		if (pid_capacitor.sreg & PID_INIT)

	 			{					//			Kp   		Ki  		Kd			d1 [s]		min			max				target
	 				PID_Init(&pid_capacitor, (float)1.0, (float)0.1, (float)0.04, (float)0.1, (float)1.0, (float)1000.0, (float)2000.0);		// min to 0.1 % of PWM, max is 100%
					pid_capacitor.sreg &= ~PID_INIT;
	 			}

	 		temp_16 = (uint16_t)PID_Compute(&pid_capacitor, pid_capacitor.target, PowerSupply.Voltage);
	 		PWM_Unit.Fulfillment= (uint16_t)(pid_capacitor.output_max / (float)PWM_Unit.Freq * (float)temp_16);


	 		 StateMachine++;
			 break;
	 	 }

	 	 case PWM_STATE:
	 	 {
	 		 if (Timers_100ms[TIMER_FAN_SPEED_CHANGE] == TIMER_STOP)
	 		 {
	 			Timers_100ms[TIMER_FAN_SPEED_CHANGE] =1; //PWM_Unit.FanSpeedChangeDelayValue;
	 		 }
	 		 if (!Timers_100ms[TIMER_FAN_SPEED_CHANGE]  )
	 		 {


	 			PWM_Unit.TIMx->CCR1 =(uint16_t)PWM_SET_FREQ(PWM_Unit.Fulfillment);
	 			Timers_100ms[TIMER_FAN_SPEED_CHANGE] = 1; //PWM_Unit.FanSpeedChangeDelayValue;
	 		 }
	 		 StateMachine++;

	 		 break;
	 	 }
	 	 case LCD_STATE:
	 	 {

	 		if (LCD_OLED.sreg & OLED_REFRESH)
	 			{
	 				INT_dec_2STR(PowerSupply.Voltage, TempString);
	 				OLEDWriteText(&LCD_OLED, TempString);
	 				LCD_OLED.sreg &= ~OLED_REFRESH;
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
		OLEDSoftwareInit(&LCD_OLED);

		SWInitStatus=1;
	}
	return SWInitStatus;
}
///


