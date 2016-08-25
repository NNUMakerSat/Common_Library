/*
 * I2C_Polling.c
 *
 *  Created on: Aug 24, 2016
 *      Author: aaronewing
 */
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "I2C_Polling.h"

void init_I2C_Hub (uint8_t slave_Address, uint16_t byte_Count, bool pin_Setting) {				// initalizes I2C clk rate and which pins are being used
	switch (pin_Setting) {
	case 0:
		P1SEL0 |= BIT6 | BIT7;				// SDA - P1.6, SCL - P1.7 (secondary function)
		break;
	case 1:
		P3SEL0 |= BIT1 | BIT2;				// SDA - P3.1, SCL - P3.2 (secondary function)
		break;
	default:
		P1SEL0 |= BIT6 | BIT7;				// SDA - P1.6, SCL - P1.7 (secondary function)
	}
	UCB0CTLW0 |= UCSWRST;                   // Software reset enabled
	UCB0CTLW0 |= UCMODE_3 | UCMST | UCSYNC;	// I2C mode, Master mode, sync
	UCB0CTLW1 |= UCASTP_2;                  // Automatic stop generated
	                                        // after UCB0TBCNT is reached
	UCB0BRW = 0x0008;                       // baudrate = SMCLK / 8
	UCB0TBCNT = byte_Count;	                // number of bytes to be received
	UCB0I2CSA = slave_Address;				// Slave address
	UCB0CTL1 &= ~UCSWRST;
}

void write_uint8_I2C (uint8_t tx_Data_8, uint8_t device_CS) {			// writes 8 bits with I2C

}

void write_uint16_I2C (uint16_t tx_Data_16, uint8_t device_CS) {		// writes 16 bits with I2C

}

void write_uint32_I2C (uint32_t tx_Data_32, uint32_t device_CS) {		// writes 16 bits with I2C

}

uint8_t read_I2C (void) {												// reads 8 bits with I2C

}


