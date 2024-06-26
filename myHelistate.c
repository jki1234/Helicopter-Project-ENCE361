/*
 * myHelistate.c
 *
 *  Created on: 14/05/2024
 *      Author: jki123
 */


#include "myHelistate.h"

//Functions to set the current and previous states. 
void
set_state(uint8_t state)
{
    curHeliMode = state;
}

void
set_prev_state(uint8_t state)
{
    prev_HeliMode = state;
}
//Simple functions to return the current and previous states. Used to avoid using external variables. 
uint8_t
get_state (void)
{
    return curHeliMode;
}

uint8_t
get_prev_state (void)
{
    return prev_HeliMode;
}

void
state_slider_handler (void)
{
    //This occurs when the helicopter is first turned on. The switch has to be switched up for the helicopter to find the reference yaw. 
    if ((get_state() == IDLE) && (slider_state == 128))
    {
        set_state(INIT);
        set_prev_state(IDLE);
        initQuadEncoder (); //Initialises the Quadrature Encoder. 
    }

    if (get_state() == LANDED)
    {
        //If the helicopter is landed, check the switch state to see if it should be LANDED or FLYING. 
        switch (slider_state)
            {
            case 128:
                set_state(FLYING);
                set_prev_state(LANDED);
                break;
            case 0:
                set_state(LANDED);
                break;
            }
    }

    if (get_state() == FLYING)
    {
        switch (slider_state)
            {
            case 128:
                set_state(FLYING);
                break;
            case 0:
            //If the switch is down when the helicopter is FLYING, change the state to LANDING to perform the landing sequence. 
                set_state(LANDING);
                set_prev_state(FLYING);
                break;
            }
    }
}

void
state_operator (int32_t alt_percent, int32_t yaw)
{

    //Button checks
    //Finds the current helicopter state. 
    switch (get_state())
    {
    case INIT:
    //Sets the main rotors on to allow the helicopter to slowly rotate about its yaw axis to find the reference point. 
        set_Main_PWM(40);
        break;
    case FLYING:
        //Alt and Yaw control is present when the helicopter is in the FLYING state. 
        alt_controller(alt_percent);
        yaw_controller(yaw/100);

        //Increments/decrements to the Alt and Yaw setpoints resulting from button inputs. 
        switch(up_but_state)
        {
        case PUSHED:
            increment_alt();
            break;
        case NO_CHANGE:
            break;
        case RELEASED:
            break;
        }

        switch(down_but_state)
        {
        case PUSHED:
            decrement_alt();
            break;
        case NO_CHANGE:
            break;
        case RELEASED:
            break;
        }

        switch(left_but_state)
        {
        case PUSHED:
            decrement_yaw();
            break;
        case NO_CHANGE:
            break;
        case RELEASED:
            break;
        }

        switch(right_but_state)
        {
        case PUSHED:
            increment_yaw();
            break;
        case NO_CHANGE:
            break;
        case RELEASED:
            break;
        }
        break;

    case LANDING:
    //Hard sets the Alt and Yaw setpoints to 0 and then uses PID control to land facing the reference point. 
        hard_set_alt();
        hard_set_yaw();
        alt_controller(alt_percent);
        yaw_controller(yaw/100);
        //Checks if the helicopter is at an acceptable altitude and yaw to enter the LANDED state. 
        if ((alt_percent < 1) && (yaw/100 >= -1) && (yaw/100 <= 1))
        {
            set_state(LANDED);
            set_prev_state(LANDING);
        }
        break;

    case LANDED:
    //Turns the motors off if the switch is down. 
        if ((slider_state == 0) && (get_prev_state() == LANDING))
        {
            disable_motors();
        } else {
            //Alt and Yaw control
            alt_controller(alt_percent);
            yaw_controller(yaw/100);
        }
        break;
   }
}
