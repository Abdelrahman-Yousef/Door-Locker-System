/******************************************************************************
 *
 * Module: External EEPROM
 *
 * File Name: external_eeprom.c
 *
 * Description: Source file for the External EEPROM Memory
 *
 * Author: Mohamed Tarek
 *
 *******************************************************************************/
#include "external_eeprom.h"
#include "twi.h"

uint8 EEPROM_writePassword(uint16 u16addr, uint32 password) {
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8) (0xA0 | ((u16addr & 0x0700) >> 7)));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	/* Send the required memory location address */
	TWI_writeByte((uint8) (u16addr));
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/*
	 * send the password [4 bytes]
	 */
	uint8 *ptr = (uint8*) &password;
	for (uint8 i = 0; i < 4; i++) {
		TWI_writeByte(*(ptr + i));
		if (TWI_getStatus() != TWI_MT_DATA_ACK)
			return ERROR;
	}

	/* Send the Stop Bit */
	TWI_stop();

	return SUCCESS;
}

uint8 EEPROM_readPassword(uint16 u16addr, uint8 *ptr) {
	/* Send the Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=0 (write) */
	TWI_writeByte((uint8) ((0xA0) | ((u16addr & 0x0700) >> 7)));
	if (TWI_getStatus() != TWI_MT_SLA_W_ACK)
		return ERROR;

	/* Send the required memory location address */
	TWI_writeByte((uint8) (u16addr));
	if (TWI_getStatus() != TWI_MT_DATA_ACK)
		return ERROR;

	/* Send the Repeated Start Bit */
	TWI_start();
	if (TWI_getStatus() != TWI_REP_START)
		return ERROR;

	/* Send the device address, we need to get A8 A9 A10 address bits from the
	 * memory location address and R/W=1 (Read) */
	TWI_writeByte((uint8) ((0xA0) | ((u16addr & 0x0700) >> 7) | 1));
	if (TWI_getStatus() != TWI_MT_SLA_R_ACK)
		return ERROR;

	/*
	 * recieve the first three bytes with ACK
	 */
	uint8 i = 0;
	for (; i < 3; i++) {
		*(ptr + i) = TWI_readByteWithACK();
		if (TWI_getStatus() != TWI_MR_DATA_ACK)
			return ERROR;
	}

	/* Read Byte from Memory without send ACK */
	*(ptr + i) = TWI_readByteWithNACK();
	if (TWI_getStatus() != TWI_MR_DATA_NACK)
		return ERROR;

	/* Send the Stop Bit */
	TWI_stop();

	return SUCCESS;
}
