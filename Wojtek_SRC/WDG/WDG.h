#ifndef WDG_H
#define WDG_H

#define IWDG_WRITE_ENABLE		0x5555
#define IWDG_RELOAD_VALUE		0x0FF
#define IWDG_RELOAD_COMMAND 	0xAAAA
#define IWDG_START_COMMAND 		0xCCCC
#define IWDG_TIMEOUT_VALUE		0xFFFF

#define IWDG_PR_255	 0b110
#define IWDG_PR_128	 0b101
#define IWDG_PR_64	 0b100
#define IWDG_PR_32	 0b011
#define IWDG_PR_16	 0b010
#define IWDG_PR_8	 0b001
#define IWDG_PR_4	 0b000


void IWDG_Start(void);

#ifdef STM32F103C8
 #include "stm32f10x_iwdg.h"
 #include "stm32f10x_dbgmcu.h"
#endif

#endif
