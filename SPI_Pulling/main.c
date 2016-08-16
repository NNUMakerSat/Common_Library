#include <msp430.h> 
#include <stdint.h>
#include "SPI_Pulling.h"
#include "Initialize.h"

//void DAC_Write(uint16_t dac_Val);

#define DAC_PD_NORMAL 0x3FFF
#define DAC_PD_LOWPOWER 0x2FFF

uint16_t dac_Val = 0x0000;
uint16_t tx_Data_16;


// main.c
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;                 	// Stop watchdog timer

	initialize_Clocks ();				// Sets up timers (takes care of FRAM issue)
	initialize_Ports();				// Init all non used ports

	uint8_t clk_Rate = 1;
	uint8_t pin_Setting = 0;

	init_SPI (clk_Rate, pin_Setting);		// clk_Rate -> 8 = 8MHz, 4 = 4MHz, 2 = 2.67MHz, 1 = 1MHz
													// pin_Setting -> 0 =>  P1.5 - UCA0CLK, P1.6 - SYNC, P2.0 - SIMO, P2.1 - SOMI


	write_uint16_SPI (tx_Data_16);

    while(1) {
    //	P1OUT |= BIT6; 								// set sync line high to start
    //	while (!(UCA0IFG & UCTXIFG)){};				// If able to TX

    	tx_Data_16 = (dac_Val & DAC_PD_NORMAL);		// Data to transmit (both value and control bits)
    	write_uint16_SPI (tx_Data_16);
    	++dac_Val;
    }

    //		DAC_Write(dac_Val);
    //		++dac_Val;


	//}
}
/*
void DAC_Write(uint16_t dac_Val) {
	static uint16_t pD_Bits = 0x3FFF;				// DAC control bits set to 0 (control is 00xx for first 4 bytes)
	uint16_t txData = (dac_Val & DAC_PD_NORMAL);
	P1OUT &= ~BIT6;									// Pulls SYNC low
	while (!(UCA0IFG & UCTXIFG)) {};    			// While TXing
	UCA0TXBUF = (txData >> 8);						// First 8 bits transmitted (Control bits and data)
	while (!(UCA0IFG & UCTXIFG)) {};
	UCA0TXBUF = txData;								// Last 8 bits transmitted (overflow expected and is fine)
	while (UCA0STATW & UCBUSY) {};
	P1OUT |= BIT6;									// Pulls SYNC high when not busy
}
*/
/*
	// Configure Primary Function Pins
    P1SEL1 |= BIT5;                           // USCI_A0 operation (P1.5)
    P2SEL0 |= BIT0 | BIT1;                    // USCI_A0 operation (P2.0 & P2.1)
    PJSEL0 |= BIT4 | BIT5;                    // For XT1

    // configure P1.6 (SYNC) as GPIO used to enable SPI write to DAC
    P1SEL0 &= ~BIT6;
    P1SEL1 &= ~BIT6;
    P1DIR |= BIT6;
*/
    // Disable the GPIO power-on default high-impedance mode to activate
    // previously configured port settings
//    PM5CTL0 &= ~LOCKLPM5;
/*
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
*/

//	while(!(UCA0IFG & UCRXIFG)){};					// places RX buffer inside RXData
//		RXData = UCA0RXBUF;
