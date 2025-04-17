#define IWDG_C
#include "WDG.h"

#undef IWDG_C

void IWDG_HardwareInit(void)
{
	uint32_t timeout = IWDG_TIMEOUT_VALUE;
#ifdef STM32F103C8
	//RCC_APB2PeriphClockCmd(RCC_APB2Periph_DBGMCU, ENABLE);
	DBGMCU->CR |= (uint32_t)(DBGMCU_IWDG_STOP);


#endif

	RCC -> CSR = RCC -> CSR | 0x00000001; //Enable LSI Clock

	do{
		timeout--;
	}while(((RCC -> CSR & 0x02) == 0) && (timeout));
	timeout=IWDG_TIMEOUT_VALUE;

	IWDG->KR = IWDG_WRITE_ENABLE;
	IWDG->PR = IWDG_PR_128;
	do{
		timeout--;
	}while ((IWDG->SR & IWDG_FLAG_PVU )&& timeout);

	timeout = IWDG_TIMEOUT_VALUE;
	IWDG->KR = IWDG_WRITE_ENABLE;
	IWDG->RLR = IWDG_RELOAD_VALUE;		// reload valud for the IWDG

	do{
		timeout--;
	}while ((IWDG->SR & IWDG_FLAG_RVU )&& timeout);


	IWDG->RLR = IWDG_RELOAD_VALUE;		// reload valud for the IWDG

	IWDG_Start();
}

void IWDG_Reload(void)
{
	IWDG->KR = IWDG_WRITE_ENABLE;
	IWDG->KR = IWDG_RELOAD_COMMAND;
}

void IWDG_Start(void)
{
	IWDG->KR = IWDG_START_COMMAND;
}
