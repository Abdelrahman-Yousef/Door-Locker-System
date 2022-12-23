/******************************************************************************
 *
 * APP: Door locker system
 *
 * File Name: app.c
 *
 * Description: Door Locker System
 *
 * Author: Abdelrahman Saeed
 *
 *******************************************************************************/

#include "uart.h"
#include "twi.h"
#include "external_eeprom.h"
#include "dc_motor.h"
#include "timer1.h"
#include "buzzer.h"
#include <avr/io.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define CORRECT 0xFF
#define WRONG 0x00

#define OPEN_DOOR '+'
#define CHANGE_PASS '-'

/*
 * variable flag
 */
volatile uint8 g_flag = 0;

/*
 * finction the change the flag variable value
 */
void changeFlagValue(void) {
	g_flag = 1;
}

int main(void) {
	/*
	 * enable the global interrupt
	 */
	SREG |= (1 << 7);

	/*
	 * initialize the UART module
	 */
	UART_ConfigType configUART = { EIGHT_BIT, EVEN_PARITY, ONE_BIT, 9600 };
	UART_init(&configUART);

	/*
	 * inilialize the TWI module
	 */
	TWI_ConfigType configTWI = { 0x01, 400000 };
	TWI_init(&configTWI);

	/*
	 * set the call back pointer to changeFlagValue function
	 */
	TIMER1_setCallBack(changeFlagValue);

	/*
	 * initialize the DC motor
	 */
	DcMotor_init();

	/*
	 * initialize the buzzer
	 */
	BUZZER_init();

	while (1) {

		while (1) {
			/*
			 * variable to store the recieved password
			 */
			uint32 recievedPassword = 0;

			/*
			 * variable to store thr re-entered password
			 */
			uint32 recievedConfirmPassword = 0;

			/*
			 * recieve the password using UART
			 */
			uint8 *password_ptr = (uint8*) &recievedPassword;
			for (uint8 i = 0; i < 4; i++) {
				*(password_ptr + i) = UART_recieveByte();
			}

			/*
			 * recieve the confirm password using UART
			 */
			password_ptr = (uint8*) &recievedConfirmPassword;
			for (uint8 i = 0; i < 4; i++) {
				*(password_ptr + i) = UART_recieveByte();
			}

			/*
			 * check the two passwrods for matching
			 */
			if (recievedPassword == recievedConfirmPassword) {
				/*
				 * if matched ,store the password in the EEPROM
				 */
				EEPROM_writePassword(0x0311, recievedPassword);

				/*
				 * send confirmation to the other micro
				 */
				UART_sendByte(CORRECT);
				break;
			} else {
				/*
				 * if not matched ,send wrong to the other micro
				 * and repeat again
				 */
				UART_sendByte(WRONG);
			}
		}

		while (1) {

			/*
			 * varaible the count numbers for asking the user to
			 * enter password
			 */
			uint8 i = 0;

			while (1) {

				/*
				 * increment i when asking the user for password
				 */
				i++;

				/*
				 * break when asking the user for password for the fourth
				 * consecutive time
				 */
				if (i == 4) {
					break;
				}

				/*
				 * recieve the password entered by the user
				 */
				uint32 recievedPassword = 0;
				uint8 *password_ptr = (uint8*) &recievedPassword;
				for (uint8 i = 0; i < 4; i++) {
					*(password_ptr + i) = UART_recieveByte();
				}

				/*
				 * get the passwrod saved in the EEPROM
				 */
				uint32 savedPassword = 0;
				EEPROM_readPassword(0x0311, (uint8*) &savedPassword);

				/*
				 * chack if the two are matching
				 */
				if (savedPassword == recievedPassword) {
					/*
					 * if matched ,send confirmation to the other micro
					 */
					UART_sendByte(CORRECT);
					break;
				} else {
					/*
					 * if not matched , send wrong to the other micro
					 * and repeat again
					 */
					UART_sendByte(WRONG);
				}
			}

			/*
			 * if the user entered wrong password the three
			 * consecutive time
			 */
			if (i == 4) {
				/*
				 * configure timer1 to count 1 minute in CTC mode
				 */
				TIMER1_ConfigType configTIMER1 =
						{ 0, 58560, PRESCALER_1024, CTC };
				TIMER1_init(&configTIMER1);

				/*
				 * turn on the buzzer
				 */
				BUZZER_on();

				/*
				 * wait for the 1 minute to pass
				 */
				while (g_flag == 0) {
				}

				/*
				 * turn off the buzzer
				 */
				BUZZER_off();

				/*
				 * return flag to initial value again
				 */
				g_flag = 0;

				/*
				 * disable timer1
				 */
				TIMER1_deInit();
			} else {
				break;
			}
		}

		/*
		 * recieve the selected option entered by the user
		 */
		uint8 selectedOption = UART_recieveByte();

		/*
		 * if open door is selected
		 */
		if (selectedOption == OPEN_DOOR) {
			/*
			 * configure timer1 to count 15 seconds in CTC mode
			 */
			TIMER1_ConfigType configTIMER1 = { 0, 14648, PRESCALER_1024, CTC };
			TIMER1_init(&configTIMER1);

			/*
			 * unlocking the door [rotate the motor in CW]
			 */
			DcMotor_rotate(CLOCKWISE, 100);

			/*
			 * wait for the 15 seconds to pass
			 */
			while (g_flag == 0) {
			}

			/*
			 * return flag to initial value again
			 */
			g_flag = 0;

			/*
			 * disable timer1
			 */
			TIMER1_deInit();

			/*
			 * configure timer1 to count 3 seconds
			 */
			configTIMER1.compareValue = 2929;
			TIMER1_init(&configTIMER1);

			/*
			 * hold [stop] the motor
			 */
			DcMotor_rotate(STOP, 100);

			/*
			 * wait for the 3 seconds to pass
			 */
			while (g_flag == 0) {
			}

			/*
			 * return flag to initial value again
			 */
			g_flag = 0;

			/*
			 * disable timer1
			 */
			TIMER1_deInit();

			/*
			 * configure timer1 to count 15 seconds in CTC mode
			 */
			configTIMER1.compareValue = 14648;
			TIMER1_init(&configTIMER1);

			/*
			 * unlocking the door [rotate the motor in ACW]
			 */
			DcMotor_rotate(ANTICLOCKWISE, 100);

			/*
			 * wait for the 15 seconds to pass
			 */
			while (g_flag == 0) {
			}

			/*
			 * return flag to initial value again
			 */
			g_flag = 0;

			/*
			 * disable timer1
			 */
			TIMER1_deInit();

			/*
			 * hold [stop] the motor
			 */
			DcMotor_rotate(STOP, 100);
		}

	}

	return 0;
}

