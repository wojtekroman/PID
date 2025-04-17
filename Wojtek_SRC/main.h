// *************** MAIN.H  ****************************

#define TRUE	1
#define FALSE 	~TRUE
//#define LED_AS_RS_TIMER_HARTBIT
//#define LED_AS_RFID_IND
#define LED_AS_HEARTBIT

//#define LUG_IN		0x01



// MASTER_FRAME_ID -> from CAN_Volvo.h
#define NO_ID	0xFFFFFFFF
#define CAN_BASE_ID		0x02200000		 //MASTER_FRAME_ID



#ifdef MAIN_C

enum {


	HARDWARE_INIT_STATE=0,
	SOFTWARE_INIT_STATE,
	TIMERS_STATE,
	GET_DEV_ID_STATE,
	CAN_READ_STATE,
	CAN_SEND_STATE,
	LED_STATE,
	POWER_SUPLY_CHECK_STATE,
	POWER_FAIL_STATE,
	LCD_STATE,
	EEPROM_SATE,
	//ID_SET_STATE,
	TEMPERATURE_CHECK_STATE,
	FAN_STATE,
	HC595_STATE,
	POWER_STATUS




};

/*
struct {

}Luggage;
*/
#define TIMER_TEMPERATURE_DISPLAY_VALUE				5
#define TIMER_POWER_VOLTAGE_DISPLAY_DELAY_100ms		5
uint32_t	CAN_DEV_ID;
//#include "eeprom.h"
//uint16_t VirtAddVarTab[NumbOfVar] = {0x1234};

uint8_t HARDWARE_INIT(void);
uint8_t SOFTWARE_INIT(void);
#else

#endif
