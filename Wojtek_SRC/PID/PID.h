#ifndef PID_H
#define PID_H
#include "stm32f10x.h"
typedef struct {
    uint16_t Kp;
    uint16_t Ki;
    uint16_t Kd;

    uint16_t dt;

    int32_t integral;
    int32_t previous_error;

    uint8_t output_min;
    uint32_t output_max;
    uint8_t sreg;
    uint16_t target;
} PID_Controller;

#define MAX_INT32_T	0x7FFFFFFF
#define MIN_INT32_T	0x80000000

// **************** SREG *****************8
#define PID_INIT	0x01


void PID_Init(PID_Controller* pid, uint16_t Kp, uint16_t Ki, uint16_t Kd, uint16_t dt, uint16_t out_min, uint16_t out_max, uint16_t target);
uint16_t PID_Compute(PID_Controller* pid, uint16_t setpoint, uint16_t measurement);

#endif
