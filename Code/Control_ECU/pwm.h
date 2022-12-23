/******************************************************************************
 *
 * Module: PWM
 *
 * File Name: pwm.h
 *
 * Description: header file for the AVR PWM driver
 *
 * Author: Abdelrahman Saeed
 *
 *******************************************************************************/

#ifndef PWM_H_
#define PWM_H_

#include "std_types.h"

/*
 * Description :
 * setup Timer0 to work with fast PWM mode
 */
void PWM_Timer0_Start(uint8 duty_cycle);

#endif /* PWM_H_ */
