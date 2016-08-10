#include <msp430.h>
#include <stdint.h>

// define the SPI pins
#define UCA0SIMO 0x01
#define UCA0SOMI 0x02
#define UCA0CLK 0x04

uint8_t input_Data[5] = {2, 3, 4, 5, 6};
//uint16_t g_counter = 0;
uint16_t g_counter = 1;

void Configure_SPI_Master_Registers(void);

// main.c
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	                         // Stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;                                // disable high impedence mode

    Configure_SPI_Master_Registers();

    __enable_interrupt();                                // enable interrupts

    UCA0IE |= UCTXIE;                                    // enable TX interrupt

    UCA0TXBUF = input_Data[0];                   // put data in TX buffer

    while(UCA0TXBUF != 0) {
    }

    return 0;											// ends function
}

void Configure_SPI_Master_Registers(void)
{
    UCA0CTLW0 |= UCSWRST;                                // enable eUSCI initialization of registers

    // configure digital I/O Pins; select primary module function for P2.0 (MOSI), P2.1 (MISO), and P2.2 (SCK)
    P2SEL1 &= ~UCA0SIMO & ~UCA0SOMI & ~UCA0CLK;
    P2SEL0 |= UCA0SIMO | UCA0SOMI | UCA0CLK;
    P1SEL1 &= ~UCA0CLK;
    P1SEL0 |= UCA0CLK;

    UCA0MCTLW = 0x00;                                    // clear modulation register, required for SPI

    UCA0CTLW0 |= UCMST | UCSYNC | UCSSEL_2 | UCMSB;      // select master mode | select SPI mode | SMCLK | MSB first

    UCA0BRW = 0x00;                                      // set the Bit Clock equal to the BRCLK

    __delay_cycles(20);                                  // wait for UCA0SIMO and UCA0SOMI to settle to final volatage

    UCA0CTLW0 &= ~UCSWRST;                               // lock eUSCI registers

    UCA0IFG &= ~UCTXIFG & ~UCRXIFG;                      // clear RX and TX interrupt flags

    return;
}

#pragma vector = USCI_A0_VECTOR
__interrupt void USCI_A0_ISR(void) {						// interrupt is when it goes from TX buffer to TX register and buffer becomes null
	if (g_counter <= 5) {
		UCA0TXBUF = input_Data[g_counter];                    // put data in TX buffer
		++g_counter;
	}
}
