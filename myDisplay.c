/*
 * myDisplay.c
 *
 *  Created on: 11/05/2024
 *      Author: jki123
 */
#include "myDisplay.h"


//This function displays the values of interest nicely on the OrbitLED display. 
void
infotainment_system(int32_t alt_percent, int32_t yaw_deg, int32_t alt_sp, int32_t yaw_sp,
                    int32_t main_duty, uint32_t tail_duty)
{
    int32_t yaw_deg_int = yaw_deg / 100; //Integer part
    int32_t yaw_deg_float = abs(yaw_deg % 100); //Decimal part
    usnprintf (yaw_str, sizeof(yaw_str), "Y=%4d.%2dSP=%4d", yaw_deg_int, yaw_deg_float, yaw_sp);

    char alt_str[17];
    char yaw_str[17];
    char main_pwm_str[17];
    char tail_pwm_str[17];

    usnprintf (alt_str, sizeof(alt_str), "Alt%%=%3d|SP=%3d", alt_percent, alt_sp);
    usnprintf (yaw_str, sizeof(yaw_str), "Y=%4d.%2dSP=%4d", yaw_deg_int, yaw_deg_float, yaw_sp);
    usnprintf (main_pwm_str, sizeof(main_pwm_str), "Mainduty = %2d %%", main_duty);
    usnprintf (tail_pwm_str, sizeof(tail_pwm_str), "Tailduty = %2d %%", tail_duty);

    // Update line on display.
    OLEDStringDraw (alt_str, 0, 0);
    OLEDStringDraw (yaw_str, 0, 1);
    OLEDStringDraw (main_pwm_str, 0, 2);
    OLEDStringDraw (tail_pwm_str, 0, 3);
}

//This function forms the status strings that are displayed via UART. 
void
uart_status_info(int32_t alt_percent, int32_t yaw_deg, int32_t alt_sp, int32_t yaw_sp,
                    int32_t main_duty, uint32_t tail_duty)
{

    char *mode;
    mode = malloc(10 * sizeof * mode);
//Comparison to find the corresponding state in the state enum list with the state number.  
    switch (get_state())
    {
    case IDLE:
        mode = "IDLE";
        break;
    case INIT:
        mode = "INIT";
        break;
    case LANDED:
        mode = "LANDED";
        break;
    case FLYING:
        mode = "FLYING";
        break;
    case LANDING:
        mode = "LANDING";
        break;
    }
    char string[120];
    usnprintf (string, sizeof(string), "Current Alt %d %% | Alt SP %d %% | Current Yaw %4d.%2d deg "
            "| Yaw SP %d deg | Main duty cycle %d %% | Tail duty cycle %d %% | Operating mode: %s \r\n", alt_percent,
            alt_sp, (yaw_deg/100), abs(yaw_deg%100), yaw_sp, main_duty, tail_duty, mode);
    send_UART(string);
}