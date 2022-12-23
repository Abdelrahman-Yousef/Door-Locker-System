/******************************************************************************
 *
 * Module: Buzzer
 *
 * File Name: buzzer.c
 *
 * Description: driver for Buzzer module
 *
 * Author: Abdelrahman Saeed
 *
 *******************************************************************************/

#include "buzzer.h"
#include "gpio.h"

/*
 * Description :
 * initialize the buzzer by setting its pins as output pins
 */
void BUZZER_init(void) {
	/*
	 * configure the buzzer pin as output pin
	 */
	GPIO_setupPinDirection(BUZZER_PORT, BUZZER_PIN, PIN_OUTPUT);

	/*
	 * turn the buzzer off at the start
	 */
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}

/*
 * Description :
 * turn on the buzzer
 */
void BUZZER_on(void) {
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_HIGH);
}

/*
 * Description :
 * turn off the buzzer
 */
void BUZZER_off(void) {
	GPIO_writePin(BUZZER_PORT, BUZZER_PIN, LOGIC_LOW);
}

