/******************************************************************************
 *
 * Module: TIMER1
 *
 * File Name: timer1.h
 *
 * Description: Header file for the AVR Timer1 driver
 *
 * Author: Abdelrahman Saeed
 *
 *******************************************************************************/

#ifndef TIMER1_H_
#define TIMER1_H_

#include "std_types.h"

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum {
	NO_CLOCK,
	PRESCALER_1,
	PRESCALER_8,
	PRESCALER_64,
	PRESCALER_256,
	PRESCALER_1024
} TIMER1_Prescaler;

typedef enum {
	NORMAL, CTC = 4
} TIMER1_Mode;

typedef struct {
	uint16 initialValue;
	uint16 compareValue;
	TIMER1_Prescaler prescaler;
	TIMER1_Mode mode;
} TIMER1_ConfigType;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Discription :
 * initialize timer1 to support both CTC mode and normal mode and
 * the function takes an structure as parameter to set the configurations
 */
void TIMER1_init(TIMER1_ConfigType *config_ptr);

/*
 * Description :
 * function to disable timer1 and remove old configurations
 */
void TIMER1_deInit(void);

/*
 * Description :
 * set the call back pointer
 */
void TIMER1_setCallBack(void (*a_ptr)(void));

#endif /* TIMER1_H_ */
