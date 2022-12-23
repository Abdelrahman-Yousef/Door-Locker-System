/******************************************************************************
 *
 * Module: DC Motor (Fan)
 *
 * File Name: dc_motor.h
 *
 * Description: header file for the DC Motor driver
 *
 * Author: Abdelrahman Saeed
 *
 *******************************************************************************/

#ifndef DC_MOTOR_H_
#define DC_MOTOR_H_

#include "std_types.h"

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define MOTOR_PORT PORTB_ID
#define MOTOR_FIRST_PIN PIN1_ID
#define MOTOR_SECOND_PIN PIN2_ID

/*******************************************************************************
 *                               Types Declaration                             *
 *******************************************************************************/

typedef enum {
	STOP, CLOCKWISE, ANTICLOCKWISE
} DcMotor_State;

/*******************************************************************************
 *                              Functions Prototypes                           *
 *******************************************************************************/

/*
 * Description :
 * setup the pins of the DC Motor as output pins
 * and turn off the DC motor at the beginning
 */
void DcMotor_init(void);

/*
 * Description :
 * Rotate the DC Motor according to the passed speed
 * in direction as specified in the state
 */
void DcMotor_rotate(DcMotor_State state, uint8 speed);

#endif /* DC_MOTOR_H_ */
