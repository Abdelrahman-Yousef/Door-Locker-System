/******************************************************************************
 *
 * Module: DC Motor (Fan)
 *
 * File Name: dc_motor.c
 *
 * Description: source file for the DC Motor driver
 *
 * Author: Abdelrahman Saeed
 *
 *******************************************************************************/

#include "dc_motor.h"
#include "gpio.h"
#include "common_macros.h"
#include "pwm.h" /* to use the PWM module to control speed */

/*
 * Description :
 * setup the pins of the DC Motor as output pins
 * and turn off the DC motor at the beginning
 */
void DcMotor_init(void) {
	/*
	 * configure the pins of the DC Motor
	 * as output pins
	 */
	GPIO_setupPinDirection(MOTOR_PORT, MOTOR_FIRST_PIN, PIN_OUTPUT);
	GPIO_setupPinDirection(MOTOR_PORT, MOTOR_SECOND_PIN, PIN_OUTPUT);

	/*
	 * turn off the DC Motor at the beginning
	 */
	GPIO_writePin(MOTOR_PORT, MOTOR_FIRST_PIN, LOGIC_LOW);
	GPIO_writePin(MOTOR_PORT, MOTOR_SECOND_PIN, LOGIC_LOW);
}

/*
 * Description :
 * Rotate the DC Motor according to the passed speed
 * in direction as specified in the state
 */
void DcMotor_rotate(DcMotor_State state, uint8 speed) {
	/*
	 * rotate the DC Motor as specified in state argument
	 * as Clock-wise or Anti clock-wise or stop
	 */
	GPIO_writePin(MOTOR_PORT, MOTOR_FIRST_PIN, GET_BIT(state, 0));
	GPIO_writePin(MOTOR_PORT, MOTOR_SECOND_PIN, GET_BIT(state, 1));

	/*
	 * control the DC Motor speed using the PWM Driver
	 */
	PWM_Timer0_Start(speed);
}
