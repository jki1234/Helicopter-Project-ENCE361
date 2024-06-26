/*
 * myMotor.h
 *
 *  Created on: 7/05/2024
 *      Author: jki123
 */

#ifndef MYMOTOR_H_
#define MYMOTOR_H_

void
initMotor (void);

void
setPWM(uint32_t, uint32_t);

void
set_Main_PWM(uint32_t);

void
set_Tail_PWM(uint32_t);

void
disable_motors (void);

#endif /* MYMOTOR_H_ */