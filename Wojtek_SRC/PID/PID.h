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
} PID_t;


PID_t pid_capacitor;

#define MAX_INT32_T	0x7FFFFFFF
#define MIN_INT32_T	0x80000000

// **************** SREG *****************8
#define PID_INIT	0x01
#define PID_RESET	0x02



#define DEF_KP		300
#define DEF_KI		10
#define DEF_KD		4
#define DEF_DT		10
#define DEF_OUT_MIN	1
#define DEF_OUT_MAX	10000
#define DEF_TARGET 	2500

void PID_Init(PID_t* pid, uint16_t Kp, uint16_t Ki, uint16_t Kd, uint16_t dt, uint16_t out_min, uint16_t out_max, uint16_t target);
uint16_t PID_Compute(PID_t* pid, uint16_t setpoint, uint16_t measurement);

#endif
