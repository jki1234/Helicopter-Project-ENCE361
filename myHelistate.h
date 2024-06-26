/*
 * myHelistate.h
 *
 *  Created on: 14/05/2024
 *      Author: jki123
 */

#ifndef MYHELISTATE_H_
#define MYHELISTATE_H_

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

#include "buttons4.h"
#include "myADC.h"
#include "myQuad.h"
#include "myMotor.h"
#include "myControl.h"
#include "myUART.h"
#include "myDisplay.h"

// Heli mode enumerator and matching strings for output
enum Helistate {IDLE = 0, INIT, LANDED, FLYING, LANDING};
static volatile uint8_t curHeliMode = 0;
static volatile uint8_t prev_HeliMode;

uint8_t up_but_state, down_but_state, left_but_state, right_but_state, slider_state, reset_state;

uint8_t
get_state (void);

uint8_t
get_prev_state (void);

void
set_state (uint8_t);

void
set_prev_state(uint8_t);

void
state_slider_handler (void);

void
state_operator (int32_t , int32_t);

#endif /* MYHELISTATE_H_ */
