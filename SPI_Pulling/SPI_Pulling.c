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

	 // XT1 Setup
	 CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
	 CSCTL1 = DCOFSEL_0;                       // Set DCO to 1MHz
	 CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
	 CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     		// set all dividers
	 CSCTL4 &= ~LFXTOFF;
	 do {
		 CSCTL5 &= ~LFXTOFFG;                  		// Clear XT1 fault flag
	     SFRIFG1 &= ~OFIFG;
	 } while (SFRIFG1&OFIFG);                 	  	// Test oscillator fault flag
	 	 CSCTL0_H = 0;                             	// Lock CS registers

	 // Configure USCI_A0 for SPI operation
	 UCA0CTLW0 = UCSWRST;                      		// **Put state machine in reset**

	 UCA0CTLW0 |= UCMST | UCSYNC | UCMSB | UCCKPL;	// 3-pin, 8-bit SPI master
	                                          	  	    // Clock polarity high, MSB
	 UCA0CTLW0 |= UCSSEL__SMCLK;              	 	// ACLK
	 UCA0BR0 = 0x02;                           		// /2
	 UCA0BR1 = 0;                             	 	//
	 UCA0MCTLW = 0;                           	 	// No modulation
	 UCA0CTLW0 &= ~UCSWRST;                   	 	// **Initialize USCI state machine**
}





/* XT1 section:
 *
 *
 * --COPYRIGHT--,BSD_EX
 * Copyright (c) 2014, Texas Instruments Incorporated
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * *  Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 *
 * *  Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * *  Neither the name of Texas Instruments Incorporated nor the names of
 *    its contributors may be used to endorse or promote products derived
 *    from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS;
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY,
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE,
 * EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 *******************************************************************************/

