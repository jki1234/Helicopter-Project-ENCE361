/*
 * myQuad.c
 *
 *  Created on: 29/04/2024
 *      Author: jki123
 */
#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_ints.h"
#include "driverlib/gpio.h"
#include "driverlib/uart.h"
#include "driverlib/sysctl.h"
#include "driverlib/systick.h"
#include "driverlib/debug.h"
#include "driverlib/pin_map.h"
#include "utils/ustdlib.h"
#include "stdio.h"
#include "stdlib.h"
#include "circBufT.h"
#include "OrbitOLED/OrbitOLEDInterface.h"
#include "buttons4.h"
#include "myHelistate.h"

#define NUM_SLOTS 112
int32_t yaw = 0;
uint8_t prev_state;
uint8_t curr_state;

void
SysQuadIntHandler (void)
{
    prev_state = curr_state;
    curr_state = GPIOPinRead(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    if (prev_state == 0b00) //Phase 1
    {
        if (curr_state == 0b01) //Phase 2
        {
            yaw--;
        } else if (curr_state == 0b10) //Phase 4
        {
            yaw++;
        }
    } else if (prev_state == 0b01) //Phase 2
    {
        if (curr_state == 0b11) //Phase 3
        {
            yaw--;
        } else if (curr_state == 0b00) //Phase 1
        {
            yaw++;
        }
    } else if (prev_state == 0b11) //Phase 3
    {
        if (curr_state == 0b01) //Phase 2
        {
            yaw++;
        } else if (curr_state == 0b10) //Phase 4
        {
            yaw--;
        }
    } else if (prev_state == 0b10) //Phase 4
    {
        if (curr_state == 0b11) //Phase 3
        {
            yaw++;
        } else if (curr_state == 0b00) //Phase 1
        {
            yaw--;
        }
    }
    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
}

void
SysRefIntHandler (void)
{
    bool yaw_Ref = GPIOPinRead(GPIO_PORTC_BASE, GPIO_PIN_4);
    if (yaw_Ref)
    {
        yaw = 0;
    }
    GPIOIntClear(GPIO_PORTC_BASE, GPIO_INT_PIN_4);
    GPIOIntEnable(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    //Sets the helicopter state to LANDED once the reference point has been found. Once the reference point has been found, the yaw in degrees will start to be measured. 
    set_state(LANDED);
    set_prev_state(INIT);
}

void initQuadEncoder (void)
{
    // Enable the GPIOA peripheral
    //
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
    SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);

    GPIOIntDisable(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOIntClear(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);

    GPIOIntRegister(GPIO_PORTB_BASE, SysQuadIntHandler);
    GPIOIntRegister(GPIO_PORTC_BASE, SysRefIntHandler);

    GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1);
    GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_4);

    GPIOIntTypeSet(GPIO_PORTB_BASE, GPIO_PIN_0 | GPIO_PIN_1, GPIO_BOTH_EDGES);

    GPIOIntTypeSet(GPIO_PORTC_BASE, GPIO_PIN_4, GPIO_RISING_EDGE);

    GPIOIntEnable(GPIO_PORTC_BASE, GPIO_PIN_4);

}

int32_t
find_yaw (void)
{
    int32_t yaw_deg_x100 = yaw*36000/4/NUM_SLOTS;
    if ((yaw_deg_x100 <= -18000) && (yaw_deg_x100 > -36000))
    {
        yaw_deg_x100 = yaw_deg_x100 + 36000;
    }
    else if ((yaw_deg_x100 > 18000) && (yaw_deg_x100 < 36000))
    {
        yaw_deg_x100 = yaw_deg_x100 - 36000;
    }
    return yaw_deg_x100;
}


