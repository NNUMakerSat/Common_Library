/*
 * SPI_Pulling.c
 *
 *  Created on: Aug 16, 2016
 *      Author: aaronewing
 */
#include <msp430.h>
#include <stdint.h>

/* P1.5 - UCA0CLK
*  P1.6 - SYNC
*  P2.0 - SIMO
*  P2.1 - SOMI
*/

void init_SPI (void) {
	// Configure Primary Function Pins
	P1SEL1 |= BIT5;                           // USCI_A0 operation (P1.5)
	P2SEL0 |= BIT0 | BIT1;                    // USCI_A0 operation (P2.0 & P2.1)
	PJSEL0 |= BIT4 | BIT5;                    // For XT1

	// configure P1.6 (SYNC) as GPIO used to enable SPI write to DAC
	P1SEL0 &= ~BIT6;
	P1SEL1 &= ~BIT6;
	P1DIR |= BIT6;
}

