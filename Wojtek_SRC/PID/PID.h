#ifndef PID_H
#define PID_H
#include "stm32f10x.h"
typedef struct {
    float Kp;
    float Ki;
    float Kd;

    float dt;

    float integral;
    float previous_error;

    float output_min;
    float output_max;
    uint8_t sreg;
    uint16_t target;
} PID_Controller;

// **************** SREG *****************8
#define PID_INIT	0x01

void PID_Init(PID_Controller* pid, float Kp, float Ki, float Kd, float dt, float out_min, float out_max, float target);
float PID_Compute(PID_Controller* pid, float setpoint, float measurement);

#endif
