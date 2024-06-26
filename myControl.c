/* myControl.c
 *
 *  Created on: 9/05/2024
 *      Author: jki123
 */

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/pin_map.h" //Needed for pin configure
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "circBufT.h"
#include "OrbitOLED/OrbitOLEDInterface.h"

#include "buttons4.h"
#include "myMotor.h"
#include "myControl.h"


//Defined maximum and minimum PWM values for the main and tail PWM generators. 
#define MAIN_MAX 85
#define MAIN_MIN 15

#define TAIL_MAX 60
#define TAIL_MIN 2

//Delta t or tau used for PID Control. 
#define DELTA_T 0.013

//PID coefficients for the Altitude and Yaw control. 
const float KP_ALT = 1.6;// random values
const float KI_ALT = 0.6;
const float KD_ALT = 0.025;

const float KP_YAW = 1.5;
const float KI_YAW = 1.1;
const float KD_YAW = 0.02;

//Previous Altitude and Yaw readings used for derivative control. 
int32_t prev_alt = 0;
int32_t prev_yaw = 0;

int32_t main_PWM_duty = 0;
uint32_t tail_PWM_duty = 0;


//Functions used to increment/decrement Altitude and Yaw setpoints.
void
increment_alt (void)
{
    alt_setpoint += 10;
    if (alt_setpoint > 100)
    {
        alt_setpoint = 100;
    }
}

void
decrement_alt (void)
{
    alt_setpoint -= 10;
    if (alt_setpoint < 0)
    {
        alt_setpoint = 0;
    }

}

void
increment_yaw (void)
{
    yaw_setpoint += 15;
    if (yaw_setpoint <= -180)
    {
        yaw_setpoint += 360;
    }
    else if (yaw_setpoint > 180)
    {
        yaw_setpoint -= 360;
    }
}

void
decrement_yaw (void)
{
    yaw_setpoint -= 15;
    if (yaw_setpoint <= -180)
    {
        yaw_setpoint += 360;
    }
    else if (yaw_setpoint > 180)
    {
        yaw_setpoint -= 360;
    }
}

//Simple functions to the return readings for the duty cycles and set points to avoid the use of external variables. 
int8_t
FindMainDuty (void)
{
    return main_PWM_duty;
}

int8_t
FindTailDuty (void)
{
    return tail_PWM_duty;
}

int32_t
find_alt_sp (void)
{
    return alt_setpoint;
}

int32_t
find_yaw_sp (void)
{
    return yaw_setpoint;
}

//Functions to set the Altitude and Yaw setpoints to 0. Used within the LANDING state. 
void
hard_set_alt (void)
{
    alt_setpoint = 0;
}

void
hard_set_yaw (void)
{
    yaw_setpoint = 0;
}

//PID Controllers for the Altitude and Yaw.
void
alt_controller(int32_t curr_alt)
{
    static float alt_I = 0;
    float curr_alt_error = alt_setpoint - curr_alt;
    float alt_P = KP_ALT * curr_alt_error;

    alt_I += KI_ALT * curr_alt_error * DELTA_T;

    float alt_D = KD_ALT * (prev_alt - curr_alt) / DELTA_T;

    main_PWM_duty = alt_P + alt_I + alt_D + 10; //Small 10% duty cycle offset for the altitude as the Helicopter struggles to climb to 10% altitude from 0% altitude without it. 

    //Gain clamps
    if (main_PWM_duty > MAIN_MAX)
    {
        main_PWM_duty = MAIN_MAX;
    } else if (main_PWM_duty < MAIN_MIN)
    {
        main_PWM_duty = MAIN_MIN;
    }
//Set PWM with the new PID defined duty cycle. 
    setPWM(main_PWM_duty, tail_PWM_duty);
//Sets the previous reading of the altitude to be the current one. Used for D control. 
    prev_alt = curr_alt;
}


void
yaw_controller(int32_t curr_yaw)
{

    float curr_yaw_error = yaw_setpoint - curr_yaw;
    static float yaw_I = 0;
// Small comparison to solve the wraparound problem. Basically finds which CW/ACW direction is more efficient to get to the Yaw setpoint. 
    if (curr_yaw_error > 180)
    {
        curr_yaw_error -= 360;
    } else if (curr_yaw_error <= -180)
    {
        curr_yaw_error += 360;
    }

    float yaw_P = KP_YAW * curr_yaw_error;
    //Integral clamp to ensure the integral does not build up too high. Prevents integral windup.  
    if ((yaw_I > -100) && (yaw_I < 100))
    {
        yaw_I += KI_YAW * curr_yaw_error * DELTA_T;
    }

    float yaw_D = KD_YAW * (prev_yaw - curr_yaw) / DELTA_T;

    tail_PWM_duty = yaw_P + yaw_I + yaw_D;

    //Gain clamps
    if (tail_PWM_duty > TAIL_MAX)
    {
        tail_PWM_duty = TAIL_MAX;
    } else if (tail_PWM_duty < TAIL_MIN)
    {
        tail_PWM_duty = TAIL_MIN;
    }

    setPWM(main_PWM_duty, tail_PWM_duty);

    prev_yaw = curr_yaw;

}
