/*
 * myControl.h
 *
 *  Created on: 9/05/2024
 *      Author: jki123
 */

#ifndef MYCONTROL_H_
#define MYCONTROL_H_


static int32_t alt_setpoint = 0;
static int32_t yaw_setpoint = 0;


void
increment_alt (void);

void
decrement_alt (void);

void
increment_yaw (void);

void
decrement_yaw (void);

int8_t
FindMainDuty (void);

int8_t
FindTailDuty (void);

int32_t
find_alt_sp (void);

int32_t
find_yaw_sp (void);

void
hard_set_alt (void);

void
hard_set_yaw (void);

void
alt_controller(int32_t);

void
yaw_controller(int32_t);

#endif /* MYCONTROL_H_ */