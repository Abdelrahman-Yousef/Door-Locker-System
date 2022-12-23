/******************************************************************************
 *
 * Module: TIMER1
 *
 * File Name: timer1.c
 *
 * Description: source file for the AVR Timer1 driver
 *
 * Author: Abdelrahman Saeed
 *
 *******************************************************************************/

#include "timer1.h"
#include <avr/io.h>
#include <avr/interrupt.h>

/* call back pointer and set it initially to NULL */
volatile static void (*callBack_ptr)(void) = NULL_PTR;

/* interrupt service routine for normal mode */
ISR(TIMER1_OVF_vect) {
	if (callBack_ptr != NULL_PTR) {
		(*callBack_ptr)();
	}
}

/* interrupt service routine for CTC mode */
ISR(TIMER1_COMPA_vect) {
	if (callBack_ptr != NULL_PTR) {
		(*callBack_ptr)();
	}
}

/*
 * Discription :
 * initialize timer1 to support both CTC mode and normal mode and
 * the function takes an structure as parameter to set the configurations
 */
void TIMER1_init(TIMER1_ConfigType *config_ptr) {

	TCCR1A = (1 << FOC1A) | (1 << FOC1B) | (config_ptr->mode & 0x0003);

	/*
	 * configure timer1 to operate in CTC mode or normal mode and
	 * specify the prescaler
	 */
	TCCR1B = ((config_ptr->mode & 0x000C) << 1) | (config_ptr->prescaler);

	/*
	 * set TCNT1 with the initial value
	 */
	TCNT1 = config_ptr->initialValue;

	/*
	 * set the compera register if working in CTC mode
	 */
	if (config_ptr->mode == CTC) {
		OCR1A = config_ptr->compareValue;
	}

	/*
	 * Enable the interrupt of the specified mode
	 */
	if (config_ptr->mode == NORMAL) {
		TIMSK = (1 << TOIE1);
	} else {
		TIMSK = (1 << OCIE1A);
	}
}

/*
 * Description :
 * function to disable timer1 and remove old configurations
 */
void TIMER1_deInit(void) {
	TCCR1A = 0;
	TCCR1B = 0;
	TCNT1 = 0;
	TIMSK = 0;
}

/*
 * Description :
 * set the call back pointer
 */
void TIMER1_setCallBack(void (*a_ptr)(void)) {
	callBack_ptr = a_ptr;
}

