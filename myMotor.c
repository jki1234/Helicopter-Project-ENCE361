/*
 * myMotor.c
 *
 *  Created on: 7/05/2024
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


// Systick configuration
#define SYSTICK_RATE_HZ 100
#define PWM_FREQ 200


uint32_t period;


//Sets the PWM duty cycles for both Main and Tail at once. 
void
setPWM(uint32_t main_duty, uint32_t tail_duty)
{
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, (main_duty*period/100));
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (tail_duty*period/100));
}

//Sets the PWM duty cycles for Main and Tail individually. 
void
set_Main_PWM(uint32_t main_duty)
{
    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, period);
    PWMPulseWidthSet(PWM0_BASE, PWM_OUT_7, (main_duty*period/100));
}

void
set_Tail_PWM(uint32_t tail_duty)
{
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, period);
    PWMPulseWidthSet(PWM1_BASE, PWM_OUT_5, (tail_duty*period/100));
}

//Inititialises the correct peripherals and pins etc for the PWM generators for Main and Tail. 
void
initMotor (void)
{
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM0); //Main motor.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_PWM1); //Tail motor.
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);

    GPIOPinConfigure(GPIO_PC5_M0PWM7);
    GPIOPinConfigure(GPIO_PF1_M1PWM5);

    GPIOPinTypePWM(GPIO_PORTC_BASE, GPIO_PIN_5); //Main motor.
    GPIOPinTypePWM(GPIO_PORTF_BASE, GPIO_PIN_1); //Tail motor.

    PWMGenConfigure(PWM0_BASE, PWM_GEN_3,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC); //Main motor.
    PWMGenConfigure(PWM1_BASE, PWM_GEN_2,
                    PWM_GEN_MODE_UP_DOWN | PWM_GEN_MODE_NO_SYNC); //Tail motor.

    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, PWM_FREQ);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, PWM_FREQ);

    // Set the PWM clock rate (using the prescaler)
    SysCtlPWMClockSet(SYSCTL_PWMDIV_4);

    period = SysCtlClockGet() / 4 / PWM_FREQ;

    PWMGenPeriodSet(PWM0_BASE, PWM_GEN_3, period);
    PWMGenPeriodSet(PWM1_BASE, PWM_GEN_2, period);

    PWMGenEnable(PWM0_BASE, PWM_GEN_3); //Main motor.
    PWMGenEnable(PWM1_BASE, PWM_GEN_2); //Tail motor.

    //Enables the PWM output.
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, false); //Main motor.
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, false); //Tail motor.
}

//Disables both PWM generators. Used when the helicopter is in the LANDED state. 
void
disable_motors (void)
{
    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, false); //Main motor.
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, false); //Tail motor.
}



