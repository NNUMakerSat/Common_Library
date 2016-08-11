/*
 * Initialize.c
 *
 *  Created on: Aug 0xFFFF0xFFFF, 200xFFFF6
 *      Author: aaronewing
 */
// contains all functions for Initializing MSP430

#include <msp430fr6989.h>
#include "Initialize.h"

void initialize_Clocks(void) {								// Sets all clocks to standard position
	CSCTL0_H = CSKEY_H;									// Unlocks clock system register (CSKEY) (allows for change in clock)

	CSCTL1 = DCORSEL | DCOFSEL_3;							// Set DCOCLK to 8 MHz (option to go to 24 MHz but FRAM doesn't like above 8MHz)

	CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK; 	// Set ACLK to 10kHz (Very Low Power), SMCLK = MCLK = 8 Mhz
																// set ACLK to LFXT if 30+ kHz is required
	CSCTL3 = DIVM__1 | DIVS__1 | DIVA__1; 			 		// Make sure there are no CLK dividers

	CSCTL0_H = 0;											// Locks clock system register (CSKEY) (allows for change in clock)
}

void initialize_Ports(void){			// sets all pins on all ports as an output (except Port 10)
	P1DIR |= 0xFFFF;
	P2DIR |= 0xFFFF;
	P3DIR |= 0xFFFF;
	P4DIR |= 0xFFFF;
	P5DIR |= 0xFFFF;
	P6DIR |= 0xFFFF;
	P7DIR |= 0xFFFF;
	P8DIR |= 0xFFFF;
	P9DIR |= 0xFFFF;
	P10DIR |= 0xFFFF;					// Pins 0, 1, and 2 are the only ones to exist on Port 10

	P1OUT = 0x000;						// sets all pins on all ports to a low output (redundant)
	P2OUT = 0x000;
	P3OUT = 0x000;
	P4OUT = 0x000;
	P5OUT = 0x000;
	P6OUT = 0x000;
	P7OUT = 0x000;
	P8OUT = 0x000;
	P9OUT = 0x000;
	P10OUT = 0x000;
}
