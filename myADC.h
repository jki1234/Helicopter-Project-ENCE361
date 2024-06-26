/*
 * myADC.h
 *
 *  Created on: 18/03/2024
 *      Author: jki123
 */

#ifndef MYADC_H_
#define MYADC_H_

void
ADCIntHandler(void);

void
initADC (void);

uint32_t
FindMeanVal(void);

int32_t
findAltPercent (int32_t, int32_t);

#endif /* MYADC_H_ */