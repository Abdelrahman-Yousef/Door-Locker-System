/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.c
 *
 * Description: source file for the AVR PWM driver
 *
 * Author: Abdelrahman Saeed
 *
 *******************************************************************************/

#include "pwm.h"
#include <avr/io.h> /* to use the AVR registers */

/*
 * Description :
 * setup Timer0 to work with fast PWM mode
 */
void PWM_Timer0_Start(uint8 duty_cycle) {
	/*
	 * configure the bits to work in PWM mode
	 * in the non-inverting mode
	 * with pre-scaler equals to 8
	 */
	TCCR0 |= (1 << WGM00) | (1 << WGM01) | (1 << COM01) | (1 << CS01);

	/*
	 * initialize the counter register with zero
	 */
	TCNT0 = 0;

	/*
	 * assign the compare match register with value to get
	 * the specified duty cycle
	 */
	OCR0 = (uint8) (0xFF * (duty_cycle / 100.0));

	/*
	 * configure the OC0 pin as output pin
	 */
	DDRB |= (1 << PB3);
}

