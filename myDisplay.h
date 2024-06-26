  /*
 * myDisplay.h
 *
 *  Created on: 11/05/2024
 *      Author: jki123
 */

#ifndef MYDISPLAY_H_
#define MYDISPLAY_H_

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
#include "myHelistate.h"

//Infotainment system for OrbitLEDInterface
void
infotainment_system(int32_t, int32_t, int32_t, int32_t,
                    int32_t, uint32_t);

//Status string for UART
void
uart_status_info(int32_t, int32_t, int32_t, int32_t,
                    int32_t, uint32_t);

#endif /* MYDISPLAY_H_ */
