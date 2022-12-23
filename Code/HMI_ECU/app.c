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

#include "lcd.h"
#include "keypad.h"
#include "uart.h"
#include "timer1.h"
#include <avr/io.h>
#include <util/delay.h>

/*******************************************************************************
 *                                Definitions                                  *
 *******************************************************************************/

#define CORRECT 0xFF
#define WRONG 0x00

#define OPEN_DOOR '+'
#define CHANGE_PASS '-'
#define ENTER 13

/*
 * variable flag
 */
volatile uint8 g_flag = 0;

/*
 * function to change the flag variable
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
	 * initialize the LCD module
	 */
	LCD_init();

	/*
	 * initilaize the UART module
	 */
	UART_ConfigType configUART = { EIGHT_BIT, EVEN_PARITY, ONE_BIT, 9600 };
	UART_init(&configUART);

	/*
	 * set the call back pointer with changeFlagValue function
	 */
	TIMER1_setCallBack(changeFlagValue);

	while (1) {
		do {
			/*
			 * ask the user to enter the password
			 */
			LCD_clearScreen();
			LCD_displayString("Plz enter pass:");
			LCD_moveCursor(1, 0);

			/*
			 * variable to store the password entered by the user
			 */
			uint32 password = 0;

			/*
			 * take the input entered by the user
			 */
			uint8 pressedKey = KEYPAD_getPressedKey();
			_delay_ms(200);
			while (pressedKey != ENTER) {
				password = password * 10 + pressedKey;
				LCD_displayCharacter('*');

				pressedKey = KEYPAD_getPressedKey();
				_delay_ms(200);
			}

			/*
			 * ask the user to re-enter the same password
			 */
			LCD_clearScreen();
			LCD_displayString("Plz re-entre the");
			LCD_moveCursor(1, 0);
			LCD_displayString("same pass: ");

			/*
			 * variable to store the re-entered password
			 */
			uint32 confirmPassword = 0;

			/*
			 * take input from the user
			 */
			pressedKey = KEYPAD_getPressedKey();
			_delay_ms(200);
			while (pressedKey != ENTER) {
				confirmPassword = confirmPassword * 10 + pressedKey;
				LCD_displayCharacter('*');

				pressedKey = KEYPAD_getPressedKey();
				_delay_ms(200);
			}

			/*
			 * send the entered password to the other micro using UART
			 * byte by byte using the password_ptr pointer
			 */
			uint8 *password_ptr = (uint8*) &password;
			for (uint8 i = 0; i < 4; i++) {
				UART_sendByte(*(password_ptr + i));
			}

			/*
			 * send the re-entered password to the other micro using UART
			 * byte by byte using the password_ptr pointer
			 */
			password_ptr = (uint8*) &confirmPassword;
			for (uint8 i = 0; i < 4; i++) {
				UART_sendByte(*(password_ptr + i));
			}

			/*
			 * waiting for confirmation for matching from
			 * the other micro if not matched repeat again
			 */
		} while (UART_recieveByte() == WRONG);

		/*
		 * variable to store the selected option by the user
		 */
		uint8 selectedOption = 0;
		while (1) {

			/*
			 * variable to track numbers of wrong passwords
			 * entered by the user
			 */
			uint8 i = 0;

			/*
			 * display the options on the LCD
			 */
			LCD_clearScreen();
			LCD_displayString("+ : Open Door");
			LCD_moveCursor(1, 0);
			LCD_displayString("- : Change Pass");

			/*
			 * take the option entered by the user
			 */
			selectedOption = KEYPAD_getPressedKey();

			do {
				/*
				 * incrementing i each time asking for the password
				 */
				i++;

				/*
				 * break when asking for password for the fourth consecutive time
				 */
				if (i == 4) {
					break;
				}

				/*
				 * ask the user for password
				 */
				LCD_clearScreen();
				LCD_displayString("Plz enter pass:");
				LCD_moveCursor(1, 0);

				/*
				 * variable to store the entered password
				 */
				uint32 password = 0;

				/*
				 * take password entered by user
				 */
				uint8 pressedKey = KEYPAD_getPressedKey();
				_delay_ms(200);
				while (pressedKey != ENTER) {
					password = password * 10 + pressedKey;
					LCD_displayCharacter('*');

					pressedKey = KEYPAD_getPressedKey();
					_delay_ms(200);
				}

				/*
				 * send the entered password to the other micro
				 * byte by byte using password_ptr pointer
				 */
				uint8 *password_ptr = (uint8*) &password;
				for (uint8 i = 0; i < 4; i++) {
					UART_sendByte(*(password_ptr + i));
				}

				/*
				 * waiting for confirmation fro the other micro
				 *  for correct password
				 */
			} while (UART_recieveByte() == WRONG);

			/*
			 * turn on alaram when the user enter wrong password for
			 * three consecutive time
			 */
			if (i == 4) {
				LCD_clearScreen();
				LCD_moveCursor(0, 5);

				/*
				 * configure timer1 to count 1 minute in CTC mode
				 */
				TIMER1_ConfigType configTIMER1 =
						{ 0, 58560, PRESCALER_1024, CTC };
				TIMER1_init(&configTIMER1);

				/*
				 * display error on the LCD
				 */
				LCD_displayString("ERROR");

				/*
				 * wait for the 1 minute to pass
				 */
				while (g_flag == 0) {
				}

				/*
				 * return the flag to its initial value
				 */
				g_flag = 0;

				/*
				 * disable timer1
				 */
				TIMER1_deInit();

				LCD_clearScreen();
			} else {
				break;
			}
		}

		/*
		 * send the selected option to the other micro
		 */
		UART_sendByte(selectedOption);

		LCD_clearScreen();

		/*
		 * if the selecte option is open door
		 */
		if (selectedOption == OPEN_DOOR) {
			/*
			 * configure timer1 to count 15 seconds in CTC mode
			 */
			TIMER1_ConfigType configTIMER1 = { 0, 14648, PRESCALER_1024, CTC };
			TIMER1_init(&configTIMER1);

			/*
			 * display door unlocking on the LCD
			 */
			LCD_displayString("Door Unlocking");

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

			LCD_clearScreen();

			/*
			 * wait for the 3 sceonds to pass
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
			 * configure timer 1 to count 15 seconds in CTC mode
			 */
			configTIMER1.compareValue = 14648;
			TIMER1_init(&configTIMER1);

			/*
			 * display door locking on the LCD
			 */
			LCD_displayString("Door Locking");

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
		}
	}

	return 0;
}

