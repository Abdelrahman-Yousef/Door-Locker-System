/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.h
 *
 * Description: driver for Buzzer module
 *
 * Author: Abdelrahman Saeed
 *
 *******************************************************************************/

#ifndef BUZZER_H_
#define BUZZER_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define BUZZER_PORT PORTA_ID
#define BUZZER_PIN PIN0_ID

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * initialize the buzzer by setting its pins as output pins
 */
void BUZZER_init(void);

/*
 * Description :
 * turn on the buzzer
 */
void BUZZER_on(void);

/*
 * Description :
 * turn off the buzzer
 */
void BUZZER_off(void);

#endif /* BUZZER_H_ */
