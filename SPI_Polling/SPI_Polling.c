/*
 * SPI_Pulling.c
 *
 *  Created on: Aug 16, 2016
 *      Author: aaronewing
 */

#include <msp430.h>
#include <stdint.h>

uint8_t g_RXData;

////////////////////// SPI INIT /////////////////////////////////////
void init_SPI (uint8_t clk_Rate, uint8_t pin_Setting) {
	switch (pin_Setting) {
		 case 1:
			 // Configure Primary Function Pins
			 P1SEL0 |= BIT4 | BIT6 | BIT7;       		// P1.4 - CLK, P1.6 - SIMO, P1.7 - SOMI

			 // configure as GPIO used to enable SPI write to DAC
			 P4SEL0 &= ~BIT1;
			 P4SEL1 &= ~BIT1;							// P4.1 - SYNC/Slave Select
			 P4DIR |= BIT1;
			 P4OUT |= BIT1;
			 break;

		 case 2:
			 // Configure Primary Function Pins
			 P2SEL0 |= BIT0 | BIT1 | BIT2;              // P2.0 - SIMO, P2.1 - SOMI, P2.2 - CLK

			 // configure as GPIO used to enable SPI write to DAC
			 P2SEL0 &= ~BIT3;
			 P2SEL1 &= ~BIT3;							// P2.3 - SYNC/Slave Select
			 P1DIR |= BIT3;
			 P1OUT |= BIT3;
			 break;

		 case 3:
			 // Configure Primary Function Pins
			 P3SEL0 |= BIT0 | BIT1 | BIT2;              // P3.0 - CLK, P3.1 - SIMO, P.2 - SOMI

			 // configure as GPIO used to enable SPI write to DAC
			 P1SEL0 &= ~BIT6;
			 P1SEL1 &= ~BIT6;							// P1.6 - SYNC/Slave Select (no 'natural' pin)
			 P1DIR |= BIT6;
			 P1OUT |= BIT6;
			 break;

		 case 4:
			 // Configure Primary Function Pins
			 P3SEL0 |= BIT4 | BIT5 | BIT6;              // P3.4 - SIMO, P3.5 - SOMI, P3.6 - CLK

			 // configure as GPIO used to enable SPI write to DAC
			 P1SEL0 &= ~BIT6;
			 P1SEL1 &= ~BIT6;							// P3.7 - SYNC/Slave Select
			 P1DIR |= BIT6;
			 P1OUT |= BIT6;
			 break;

		 default:
			 // Configure Primary Function Pins
			 P1SEL1 |= BIT5;                          	// P1.5 - CLK
			 P2SEL0 |= BIT0 | BIT1;                   	// P2.0 - SIMO, P2.1 - SOMI

			 // configure as GPIO used to enable SPI write to DAC
			 P1SEL0 &= ~BIT6;
			 P1SEL1 &= ~BIT6;							// P1.6 - SYNC/Slave Select
			 P1DIR |= BIT6;
			 P1OUT |= BIT6;
			 break;
		 }

	 // XT1 Setup
	 CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers

	 switch (clk_Rate) {
	 case 1:
		 CSCTL1 = DCOFSEL_0;				// Set DCO to 1MHz
		 break;
	 case 2:
		 CSCTL1 = DCOFSEL_1;				// Set DCO to 2.67MHzj
	 	break;
	 case 4:
		 CSCTL1 = DCOFSEL_3;				// Set DCO to 4MHz
	 	 break;
	 case 8:
	 	 CSCTL1 = DCOFSEL_7;				// Set DCO to 8MHz
	 	 break;
	 default:
	 	 CSCTL1 = DCOFSEL_0;				// Set DCO to 1MHz
	 }

	CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
 	CSCTL0_H = 0;                             	// Lock CS registers

	 // Configure USCI_A0 for SPI operation
	 UCB0CTLW0 = UCSWRST;                      		// **Put state machine in reset**

	 UCB0CTLW0 |= UCMST | UCSYNC | UCMSB | UCCKPL;	// 3-pin, 8-bit SPI master
	                                          	  	    // Clock polarity high, MSB
	 UCB0CTLW0 |= UCSSEL__SMCLK;              	 	// SMCLK
	 UCB0BR0 = 0x02;                           		// /2  <<look if problems>>
	 UCB0BR1 = 0;                             	 	//
	 UCB0MCTLW = 0;                           	 	// No modulation
	 UCB0CTLW0 &= ~UCSWRST;                   	 	// **Initialize USCI state machine**
}

////////////////////// SPI WRITE 8 BIT /////////////////////////////////////
void write_uint8_SPI (uint8_t tx_Data_8, uint8_t device_CS) {
	while (!(UCB0IFG & UCTXIFG)){};							// If able to TX

	switch (device_CS) {
		case 1:
			P1OUT &= ~BIT4;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};				// While TXing
			UCB0TXBUF = tx_Data_8;							// 8 bits transmitted
			while (UCB0STATW & UCBUSY) {};					// While not busy
			P1OUT |= BIT4;
			break;

		case 2:
			P2OUT &= ~BIT8;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};				// While TXing
			UCB0TXBUF = tx_Data_8;							// 8 bits transmitted
			while (UCB0STATW & UCBUSY) {};
			P2OUT |= BIT6;
			break;

		case 3:
			P3OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = tx_Data_8;							// 8 bits transmitted
			while (UCB0STATW & UCBUSY) {};
			P3OUT |= BIT6;
			break;

		case 4:
			P4OUT &= ~BIT1;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = tx_Data_8;							// 8 bits transmitted
			while (UCB0STATW & UCBUSY) {};
			P4OUT |= BIT1;
			break;

		case 5:
			P5OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = tx_Data_8;							// 8 bits transmitted
			while (UCB0STATW & UCBUSY) {};
			P5OUT |= BIT6;
			break;

		case 6:
			P6OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = tx_Data_8;							// 8 bits transmitted
			while (UCB0STATW & UCBUSY) {};
			P6OUT |= BIT6;
			break;

		case 7:
			P7OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = tx_Data_8;							// 8 bits transmitted
			while (UCB0STATW & UCBUSY) {};
			P7OUT |= BIT6;
			break;

		case 8:
			P8OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = tx_Data_8;							// 8 bits transmitted
			while (UCB0STATW & UCBUSY) {};
			P8OUT |= BIT6;
			break;

		case 9:
			P9OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = tx_Data_8;							// 8 bits transmitted
			while (UCB0STATW & UCBUSY) {};
			P9OUT |= BIT6;
			break;

		case 10:
			P1OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = tx_Data_8;							// 8 bits transmitted
			while (UCB0STATW & UCBUSY) {};
			P1OUT |= BIT6;
			break;

		default:
			P1OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = tx_Data_8;							// 8 bits transmitted
			while (UCB0STATW & UCBUSY) {};
			P1OUT |= BIT6;
	}
}

////////////////////// SPI WRITE 16 BIT /////////////////////////////////////
void write_uint16_SPI (uint16_t tx_Data_16, uint8_t device_CS) {
	while (!(UCB0IFG & UCTXIFG)){};							// If able to TX

	switch (device_CS) {
		case 1:
			P1OUT &= ~BIT4;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = (tx_Data_16 >> 8);					// First 8 bits transmitted (Control bits and data)
			while (!(UCB0IFG & UCTXIFG)) {};
			UCB0TXBUF = tx_Data_16;							// Last 8 bits transmitted (overflow expected and is fine)
			while (UCB0STATW & UCBUSY) {};
			P1OUT |= BIT4;
			break;

		case 2:
			P2OUT &= ~BIT8;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = (tx_Data_16 >> 8);					// First 8 bits transmitted (Control bits and data)
			while (!(UCB0IFG & UCTXIFG)) {};
			UCB0TXBUF = tx_Data_16;							// Last 8 bits transmitted (overflow expected and is fine)
			while (UCB0STATW & UCBUSY) {};
			P2OUT |= BIT6;
			break;

		case 3:
			P3OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = (tx_Data_16 >> 8);					// First 8 bits transmitted (Control bits and data)
			while (!(UCB0IFG & UCTXIFG)) {};
			UCB0TXBUF = tx_Data_16;							// Last 8 bits transmitted (overflow expected and is fine)
			while (UCB0STATW & UCBUSY) {};
			P3OUT |= BIT6;
			break;

		case 4:
			P4OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = (tx_Data_16 >> 8);					// First 8 bits transmitted (Control bits and data)
			while (!(UCB0IFG & UCTXIFG)) {};
			UCB0TXBUF = tx_Data_16;							// Last 8 bits transmitted (overflow expected and is fine)
			while (UCB0STATW & UCBUSY) {};
			P4OUT |= BIT6;
			break;

		case 5:
			P5OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = (tx_Data_16 >> 8);					// First 8 bits transmitted (Control bits and data)
			while (!(UCB0IFG & UCTXIFG)) {};
			UCB0TXBUF = tx_Data_16;							// Last 8 bits transmitted (overflow expected and is fine)
			while (UCB0STATW & UCBUSY) {};
			P5OUT |= BIT6;
			break;

		case 6:
			P6OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = (tx_Data_16 >> 8);					// First 8 bits transmitted (Control bits and data)
			while (!(UCB0IFG & UCTXIFG)) {};
			UCB0TXBUF = tx_Data_16;							// Last 8 bits transmitted (overflow expected and is fine)
			while (UCB0STATW & UCBUSY) {};
			P6OUT |= BIT6;
			break;

		case 7:
			P7OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = (tx_Data_16 >> 8);					// First 8 bits transmitted (Control bits and data)
			while (!(UCB0IFG & UCTXIFG)) {};
			UCB0TXBUF = tx_Data_16;							// Last 8 bits transmitted (overflow expected and is fine)
			while (UCB0STATW & UCBUSY) {};
			P7OUT |= BIT6;
			break;

		case 8:
			P8OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = (tx_Data_16 >> 8);					// First 8 bits transmitted (Control bits and data)
			while (!(UCB0IFG & UCTXIFG)) {};
			UCB0TXBUF = tx_Data_16;							// Last 8 bits transmitted (overflow expected and is fine)
			while (UCB0STATW & UCBUSY) {};
			P8OUT |= BIT6;
			break;

		case 9:
			P9OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = (tx_Data_16 >> 8);					// First 8 bits transmitted (Control bits and data)
			while (!(UCB0IFG & UCTXIFG)) {};
			UCB0TXBUF = tx_Data_16;							// Last 8 bits transmitted (overflow expected and is fine)
			while (UCB0STATW & UCBUSY) {};
			P9OUT |= BIT6;
			break;

		case 10:
			P1OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = (tx_Data_16 >> 8);					// First 8 bits transmitted (Control bits and data)
			while (!(UCB0IFG & UCTXIFG)) {};
			UCB0TXBUF = tx_Data_16;							// Last 8 bits transmitted (overflow expected and is fine)
			while (UCB0STATW & UCBUSY) {};
			P1OUT |= BIT6;
			break;

		default:
			P1OUT &= ~BIT6;									// Pulls SYNC low
			while (!(UCB0IFG & UCTXIFG)) {};    			// While TXing
			UCB0TXBUF = (tx_Data_16 >> 8);					// First 8 bits transmitted (Control bits and data)
			while (!(UCB0IFG & UCTXIFG)) {};
			UCB0TXBUF = tx_Data_16;							// Last 8 bits transmitted (overflow expected and is fine)
			while (UCB0STATW & UCBUSY) {};
			P1OUT |= BIT6;									// Pulls SYNC high when not busy
	}
}

////////////////////// SPI READ POLLING //////////////////////////////////
uint8_t read_SPI (void) {
	while (!(UCB0IFG & UCRXIFG)) {};    			// While RX flag is high
	g_RXData = UCB0RXBUF;							// First 8 bits transmitted (Control bits and data)
	return g_RXData;
}

////////////////////// LEGAL /////////////////////////////////////
/* XT1 config section:
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
 *******************************************************************************
 *                       MSP430 CODE EXAMPLE DISCLAIMER
 *
 * MSP430 code examples are self-contained low-level programs that typically
 * demonstrate a single peripheral function or device feature in a highly
 * concise manner. For this the code may rely on the device's power-on default
 * register values and settings such as the clock configuration and care must
 * be taken when combining code from several examples to avoid potential side
 * effects. Also see www.ti.com/grace for a GUI- and www.ti.com/msp430ware
 * for an API functional library-approach to peripheral configuration.
 *
 * --/COPYRIGHT--*/
//******************************************************************************
