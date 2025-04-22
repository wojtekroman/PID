#include "PID.h"

// parmeters Kx * 100
void PID_Init(PID_t* pid, uint16_t Kp, uint16_t Ki, uint16_t Kd, uint16_t dt, uint16_t out_min, uint16_t out_max, uint16_t target)
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

// parmeters Kx * 100
void PID_DEF_Init(PID_t* pid)
{
    pid->Kp = DEF_KP;
    pid->Ki = DEF_KI;
    pid->Kd = DEF_KD;
    pid->dt = DEF_DT;
    pid->integral = 0;
    pid->previous_error = 0;
    pid->output_min = DEF_OUT_MIN;
    pid->output_max = DEF_OUT_MAX;
    pid->target = DEF_TARGET;
}

void PID_TargetInit(PID_t* pid, uint16_t target)
{
	pid->target = (uint16_t)target;
	if (pid->sreg & PID_RESET)
	{
		 pid->integral = 0;
		 pid->previous_error = 0;
	}
}

/*
void PID_KpInit(PID_t* pid, uint16_t newValue)
{
	pid->Kp = (uint16_t)newValue;
	if (pid->sreg & PID_RESET)
	{
		 pid->integral = 0;
		 pid->previous_error = 0;
	}
}


void PID_KiInit(PID_t* pid, uint16_t newValue)
{
	pid->Ki = (uint16_t)target;
	if (pid->sreg & PID_RESET)
	{
		 pid->integral = 0;
		 pid->previous_error = 0;
	}
}
void PID_KdInit(PID_t* pid, uint16_t newValue)
{
	pid->Kd = (uint16_t)newValue;
	if (pid->sreg & PID_RESET)
	{
		 pid->integral = 0;
		 pid->previous_error = 0;
	}
}

void PID_DtInit(PID_t* pid, uint16_t newValue)
{
	pid->Kd = (uint16_t)newValue;
	if (pid->sreg & PID_RESET)
	{
		 pid->integral = 0;
		 pid->previous_error = 0;
	}
}
*/
void PID_NewUint16Init(PID_t* pid, uint16_t* parameter, uint16_t newValue)
{
	*parameter = (uint16_t)newValue;
	if (pid->sreg & PID_RESET)
	{
		 pid->integral = 0;
		 pid->previous_error = 0;
	}
}

uint16_t PID_Compute(PID_t* pid, uint16_t setpoint, uint16_t measurement)
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
