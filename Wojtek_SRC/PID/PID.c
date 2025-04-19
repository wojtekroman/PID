#include "PID.h"

// parmeters Kx * 100
void PID_Init(PID_Controller* pid, uint16_t Kp, uint16_t Ki, uint16_t Kd, uint16_t dt, uint16_t out_min, uint16_t out_max, uint16_t target)
{
    pid->Kp = Kp;
    pid->Ki = Ki;
    pid->Kd = Kd;
    pid->dt = dt;
    pid->integral = 0;
    pid->previous_error = 0;
    pid->output_min = out_min;
    pid->output_max = out_max;
    pid->target = (uint16_t)target;
}

uint16_t PID_Compute(PID_Controller* pid, uint16_t setpoint, uint16_t measurement)
{
	int32_t derivative =0 , Dout=0 , output=0, Pout=0, Iout=0;

	int32_t error = setpoint - measurement;

    // P
	Pout = pid->Kp * error / 100;		// /100 due to skalling of parameters (*100)

    // I
    pid->integral += error * (int32_t)pid->dt;

    //  ********************** to avoid overfill **************************
   if (pid->integral * pid->Ki >= (MAX_INT32_T-100))
        //pid->integral = MAX_INT32_T ;
   	    pid->integral = 0;
    else if (pid->integral * pid->Ki < (100+(int32_t)MIN_INT32_T))
        //pid->integral = MIN_INT32_T;
   	   	pid->integral = 0;
    Iout = (pid->Ki * pid->integral) /100;		//	/ 1000 - time is in ms   ; /100 Ki is skaled *100

    // D
    derivative = (error - pid->previous_error)*10 / pid->dt;	// *10 - time is in ms multiply by 100; 1000/100 =10
    Dout = pid->Kd * derivative / 100;				// scaling of Ku

    // PID output
     output = Pout + Iout + Dout;

    // Saturacja
    if (output >(int32_t) pid->output_max)
        output = pid->output_max;
    else if (output < (int32_t)pid->output_min)
        output = pid->output_min;
  //  output/=100;			// due to parameters scalling
    // Zapisz b³¹d
    pid->previous_error = error;

    return (uint16_t)output;
}
