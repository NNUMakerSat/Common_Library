/*
 * Initialize.c
 *
 *  Created on: Aug 0xFFFF0xFFFF, 200xFFFF6
 *      Author: aaronewing
 */
#include <msp430fr6989.h>
#include "Initialize.h"

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

	P1OUT |= 0x000;						// sets all pins on all ports to a low output (redundant)
	P2OUT |= 0x000;
	P3OUT |= 0x000;
	P4OUT |= 0x000;
	P5OUT |= 0x000;
	P6OUT |= 0x000;
	P7OUT |= 0x000;
	P8OUT |= 0x000;
	P9OUT |= 0x000;
	P10OUT |= 0x000;
}

void initialize_Clocks(void) {

}
