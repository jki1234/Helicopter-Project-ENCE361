#include <stdint.h>
#include <stdbool.h>

#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/adc.h"
#include "driverlib/pwm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/interrupt.h"
#include "driverlib/debug.h"
#include "utils/ustdlib.h"
#include "circBufT.h"
#include "OrbitOLED/OrbitOLEDInterface.h"

//Header files that have been defined or edited myself. 
#include "buttons4.h"
#include "myADC.h"
#include "myQuad.h"
#include "myMotor.h"
#include "myControl.h"
#include "myUART.h"
#include "myDisplay.h"
#include "myHelistate.h"

#define SAMPLE_RATE_HZ 250

static uint32_t g_ulSampCnt;    // Counter for the interrupts

void
SysTickIntHandler(void)
{
    //
    // Initiate a conversion
    //
    ADCProcessorTrigger(ADC0_BASE, 3);
    g_ulSampCnt++;
}

void
initSystick(void)
{
    // Set up the period for the SysTick timer.  The SysTick timer period is
    // set as a function of the system clock.
    SysTickPeriodSet(SysCtlClockGet() / SAMPLE_RATE_HZ);
    //
    // Register the interrupt handler
    SysTickIntRegister(SysTickIntHandler);
    //
    // Enable interrupt and device
    SysTickIntEnable();
    SysTickEnable();
}


void
initClock (void)
{
    // Set the clock rate to 20 MHz
    SysCtlClockSet (SYSCTL_SYSDIV_10 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN |
                   SYSCTL_XTAL_16MHZ);
    //
}



int
main(void)
{

    OLEDInitialise ();
    initClock ();

    SysCtlPeripheralReset (SYSCTL_PERIPH_GPIOC); // Used for main PWM output
    SysCtlPeripheralReset (SYSCTL_PERIPH_PWM0);  // Main Rotor PWM
    SysCtlPeripheralReset (SYSCTL_PERIPH_GPIOF); // Used for tail PWM output
    SysCtlPeripheralReset (SYSCTL_PERIPH_PWM1);  // Tail Rotor PWM

    initButtons();
    initADC ();
    initMotor ();
    initSystick();
    initUART();

    PWMOutputState(PWM0_BASE, PWM_OUT_7_BIT, true); //Main motor.
    PWMOutputState(PWM1_BASE, PWM_OUT_5_BIT, true); //Tail motor.


    int8_t main_duty;
    int8_t tail_duty;

    int32_t ADCaverage, ground_alt, yaw, yaw_sp, alt_sp;
    int32_t alt_percent = 0;

    SysCtlDelay (SysCtlClockGet() / 24);
    ground_alt = FindMeanVal();
    IntMasterEnable();


    while (1)
    {
        ADCaverage = FindMeanVal();
        alt_percent = findAltPercent(ADCaverage, ground_alt);
        yaw = find_yaw();
        alt_sp = find_alt_sp();
        yaw_sp = find_yaw_sp();
        main_duty = FindMainDuty();
        tail_duty = FindTailDuty();

        //Constant polling of button inputs to update their corresponding button state. 
        updateButtons();

        up_but_state = checkButton (UP);
        down_but_state = checkButton (DOWN);
        left_but_state = checkButton (LEFT);
        right_but_state = checkButton (RIGHT);
        slider_state = GPIOPinRead (GPIO_PORTA_BASE, GPIO_PIN_7);
        reset_state = GPIOPinRead (GPIO_PORTA_BASE, GPIO_PIN_6);

        //Constant check for if the reset switch has been set to HIGH.
        if (reset_state != 0)
        {
            SysCtlReset();
        }

        //Print functions
        infotainment_system(alt_percent, yaw, alt_sp, yaw_sp, main_duty, tail_duty);
        //uart_status_info(alt_percent, yaw, alt_sp, yaw_sp, main_duty, tail_duty);

        //Function to handle any of the Helicopter's state transitions resulting from the SW1 input. 
        state_slider_handler();
        //Function to define the operations that occur within each Helicopter state. 
        state_operator(alt_percent, yaw);


    }

}


