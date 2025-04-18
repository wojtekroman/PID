#include "PID.h"

void PID_Init(PID_Controller* pid, float Kp, float Ki, float Kd, float dt, float out_min, float out_max, float target)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->dt = dt;
    pid->integral = 0.0f;
    pid->previous_error = 0.0f;
    pid->output_min = out_min;
    pid->output_max = out_max;
    pid->target = (uint16_t)target;
}

float PID_Compute(PID_Controller* pid, float setpoint, float measurement)
{
    float error = setpoint - measurement;

    // P
    float Pout = pid->Kp * error;

    // I
    pid->integral += error * pid->dt;

    // Anti-windup
    if (pid->integral * pid->Ki > pid->output_max)
        pid->integral = pid->output_max / pid->Ki;
    else if (pid->integral * pid->Ki < pid->output_min)
        pid->integral = pid->output_min / pid->Ki;

    float Iout = pid->Ki * pid->integral;

    // D
    float derivative = (error - pid->previous_error) / pid->dt;
    float Dout = pid->Kd * derivative;

    // PID output
    float output = Pout + Iout + Dout;

    // Saturacja
    if (output > pid->output_max)
        output = pid->output_max;
    else if (output < pid->output_min)
        output = pid->output_min;

    // Zapisz b³¹d
    pid->previous_error = error;

    return output;
}
