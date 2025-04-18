// *************** MAIN.H  ****************************
#ifndef MAIN_H
#define MAIN_H

#define TRUE	1
#define FALSE 	~TRUE
#define LED_AS_HEARTBIT

//#define LUG_IN		0x01



// MASTER_FRAME_ID -> from CAN_Volvo.h
#define NO_ID	0xFFFFFFFF
#define CAN_BASE_ID		0x02200000		 //MASTER_FRAME_ID





enum {


	SOFTWARE_INIT_STATE=0,
	HARDWARE_INIT_STATE,
	TIMERS_STATE,

	CAN_READ_STATE,
	CAN_SEND_STATE,
	LED_STATE,
	POWER_SUPLY_CHECK_STATE,
	PWM_STATE,
	LCD_STATE




};

#define TIMER_TEMPERATURE_DISPLAY_VALUE				5
#define TIMER_POWER_VOLTAGE_DISPLAY_DELAY_100ms		5

uint8_t HARDWARE_INIT(void);
uint8_t SOFTWARE_INIT(void);


#endif
