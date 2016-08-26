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
		P1SEL1 |= BIT6 | BIT7;				// SDA - P1.6, SCL - P1.7 (secondary function)
		UCB0CTLW0 |= UCSWRST;                   // Software reset enabled
		UCB0CTLW0 |= UCMODE_3 | UCMST | UCSYNC | UCSSEL_2;	// I2C mode, Master mode, sync
		UCB0CTLW1 |= UCASTP_2;                  // Automatic stop generated
			                                    // after UCB0TBCNT is reached
		UCB0BRW = 0x0008;                       // baudrate = SMCLK / 8
		UCB0TBCNT = byte_Count;	                // number of bytes to be received
		UCB0I2CSA = slave_Address;				// Slave address
		UCB1BRW = 80;                           // fSCL = 1Mhz/100 = ~100kHz <new>/no effect
		UCB0CTLW0 &= ~UCSWRST;
		break;
	case 1:
	default:
		P3SEL1 |= BIT1 | BIT2;					// SDA - P3.1, SCL - P3.2 (secondary function)
		UCB1CTLW0 |= UCSWRST;                   // Software reset enabled
		UCB1CTLW0 |= UCMODE_3 | UCMST | UCSYNC;	// I2C mode, Master mode, sync
		UCB1CTLW1 |= UCASTP_2;                  // Automatic stop generated
		                                        // after UCB0TBCNT is reached
		UCB1BRW = 0x0008;                       // baudrate = SMCLK / 8
		UCB1TBCNT = byte_Count;	                // number of bytes to be received
		UCB1I2CSA = slave_Address;				// Slave address
		UCB1CTLW0 &= ~UCSWRST;
		break;
	}
}

void init_I2C_SB (uint8_t slave_Address, bool pin_Setting) {			// initalizes I2C clk rate and which pins are being used
	switch (pin_Setting) {
	case 0:
		P1SEL0 |= BIT6 | BIT7;					// SDA - P1.6, SCL - P1.7 (secondary function)
		UCB0CTLW0 |= UCSWRST;                   // Software reset enabled
		UCB0CTLW0 |= UCMODE_3 | UCSYNC;			// I2C mode, slave mode, sync
		UCB0I2COA0 = slave_Address | UCOAEN;	// own address is x + enable
		UCB0CTL1 &= ~UCSWRST;
		break;
	case 1:
	default:
		P3SEL0 |= BIT1 | BIT2;				// SDA - P3.1, SCL - P3.2 (secondary function)
		UCB1CTLW0 |= UCSWRST;                   // Software reset enabled
		UCB1CTLW0 |= UCMODE_3 | UCSYNC;			// I2C mode, slave mode, sync
		UCB1I2COA0 = slave_Address | UCOAEN;             // own address is x + enable
		UCB1CTL1 &= ~UCSWRST;
		break;
	}

}

void write_uint8_I2C (uint8_t tx_Data_8, bool pin_Setting) {			// writes 8 bits with I2C
	switch (pin_Setting) {
	case 0:
		while (!(UCB0IFG & UCTXIFG0)) {};    							// While TXing
		UCB0TXBUF = tx_Data_8;											// 8 bits transmitted (overflow expected and is fine)
		break;
	case 1:
	default:
		while (!(UCB1IFG & UCTXIFG1)) {};    							// While TXing
		UCB1TXBUF = tx_Data_8;											// 8 bits transmitted (overflow expected and is fine)
	}
}

void write_uint16_I2C (uint16_t tx_Data_16, bool pin_Setting) {		// writes 16 bits with I2C
	switch (pin_Setting) {
	case 0:
		while (!(UCB0IFG & UCTXIFG0)) {};    							// While TXing
		UCB0TXBUF = (tx_Data_16 >> 8);									// First 8 bits transmitted (Control bits and data)
		while (!(UCB0IFG & UCTXIFG0)) {};
		UCB0TXBUF = tx_Data_16;											// Last 8 bits transmitted (overflow expected and is fine)
		break;
	case 1:
	default:
		while (!(UCB1IFG & UCTXIFG1)) {};    							// While TXing
		UCB1TXBUF = (tx_Data_16 >> 8);									// First 8 bits transmitted (Control bits and data)
		while (!(UCB1IFG & UCTXIFG1)) {};
		UCB1TXBUF = tx_Data_16;											// Last 8 bits transmitted (overflow expected and is fine)
	}
}

void write_uint32_I2C (uint32_t tx_Data_32, bool pin_Setting) {		// writes 16 bits with I2C
	uint8_t x = 32;
	uint8_t i;
	switch (pin_Setting) {
		case 0:
			for (i = 1; i < 5; ++i) {
				while (!(UCB0IFG & UCTXIFG0)) {};    							// While TXing
				UCB0TXBUF = (tx_Data_32 >> (x-8*i));							// First through fourth 8 bit block transmitted (Control bits and data)
			}
			break;
		case 1:
		default:
			for (i = 1; i < 5; ++i) {
				while (!(UCB1IFG & UCTXIFG1)) {};    							// While TXing
				UCB0TXBUF = (tx_Data_32 >> (x-8*i));							// First through fourth 8 bit block transmitted (Control bits and data)
			}
	}
}

uint8_t read_I2C (bool pin_Setting) {												// reads 8 bits with I2C
	uint8_t RX_Data;
	switch (pin_Setting) {
	case 0:
		while (UCB0CTL1 & UCTXSTP);             // Ensure stop condition got sent
		UCB0CTL1 |= UCTXSTT;                    // I2C start condition

		while (!(UCB0IFG & UCRXIFG0)) {};		// When RX Buffer is full
		RX_Data = UCB0RXBUF;
		break;
	case 1:
	default:
		while (UCB1CTL1 & UCTXSTP);             // Ensure stop condition got sent
		UCB1CTL1 |= UCTXSTT;                    // I2C start condition

		while (!(UCB1IFG & UCRXIFG1)) {};		// When RX Buffer is full
		RX_Data = UCB1RXBUF;
		break;
	}
	return RX_Data;
}
