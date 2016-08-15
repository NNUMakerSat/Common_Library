#include <msp430.h> 
#include <stdint.h>

//void DAC_Write(uint16_t dac_Val, uint16_t pD_Bits);
void DAC_Write(uint16_t dac_Val);
void Dec_To_Bin(int);

unsigned int bin[12];
uint16_t dac_Val = 0x1000;
uint16_t RXData;
//uint16_t pD_Bits = 0x0000;		// DAC control bits set to 0

// main.c
int main(void) {
WDTCTL = WDTPW | WDTHOLD;                 // Stop watchdog timer

// Configure GPIO
P1SEL1 |= BIT5 | BIT6;                    // USCI_A0 operation P1.6 = SYNC
P2SEL0 |= BIT0 | BIT1;                    // USCI_A0 operation
PJSEL0 |= BIT4 | BIT5;                    // For XT1

// Disable the GPIO power-on default high-impedance mode to activate
// previously configured port settings
PM5CTL0 &= ~LOCKLPM5;

// XT1 Setup
CSCTL0_H = CSKEY >> 8;                    // Unlock CS registers
CSCTL1 = DCOFSEL_0;                       // Set DCO to 1MHz
CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     // set all dividers
CSCTL4 &= ~LFXTOFF;
do {
  CSCTL5 &= ~LFXTOFFG;                    // Clear XT1 fault flag
  SFRIFG1 &= ~OFIFG;
} while (SFRIFG1&OFIFG);                   // Test oscillator fault flag
CSCTL0_H = 0;                             // Lock CS registers

// Configure USCI_A0 for SPI operation
UCA0CTLW0 = UCSWRST;                      // **Put state machine in reset**
UCA0CTLW0 |= UCMST | UCSYNC | UCCKPL | UCMSB; // 3-pin, 8-bit SPI master
                                          // Clock polarity high, MSB
UCA0CTLW0 |= UCSSEL__ACLK;                // ACLK
UCA0BR0 = 0x02;                           // /2
UCA0BR1 = 0;                              //
UCA0MCTLW = 0;                            // No modulation
UCA0CTLW0 &= ~UCSWRST;                    // **Initialize USCI state machine**
//UCA0IE |= UCRXIE;                         // Enable USCI_A0 RX interrupt

while(1) {
	while (!(UCA0IFG & UCTXIFG)){};
//		DAC_Write(dac_Val, pD_Bits);
		DAC_Write(dac_Val);
		++dac_Val;

	while(!(UCA0IFG & UCRXIFG)){};
		RXData = UCA0RXBUF;			// places RX buffer inside RXData

	}
}

void Dec_To_Bin(int dec)
{
   unsigned int j;
    // initialize the binary array elements to zero
    for (j = 0; j < 12; j++)
    {
        bin[j] = 0;
    }

    if (dec >= 2048)
    {
        bin[11] = 1;
        dec -= 2048;
    }

    if (dec >= 1024)
    {
        bin[10] = 1;
        dec -= 1024;
    }

    if (dec >= 512)
        {
            bin[9] = 1;
            dec -= 512;
        }

    if (dec >= 256)
        {
            bin[8] = 1;
            dec -= 256;
        }

    if (dec >= 128)
        {
            bin[7] = 1;
            dec -= 128;
        }

    if (dec >= 64)
        {
            bin[6] = 1;
            dec -= 64;
        }

    if (dec >= 32)
        {
            bin[5] = 1;
            dec -= 32;
        }

    if (dec >= 16)
        {
            bin[4] = 1;
            dec -= 16;
        }

    if (dec >= 8)
            {
                bin[3] = 1;
                dec -= 8;
            }

    if (dec >= 4)
            {
                bin[2] = 1;
                dec -= 4;
            }

    if (dec >= 2)
            {
                bin[1] = 1;
                dec -= 2;
            }

    if (dec >= 1)
            {
                bin[0] = 1;
                dec -= 1;
            }
}

//void DAC_Write(uint16_t dac_Val, uint16_t pD_Bits) {
void DAC_Write(uint16_t dac_Val) {
	static uint16_t pD_Bits = 0x0000;		// DAC control bits set to 0
	P1OUT &= ~BIT6;										// Pulls SYNC low
	UCA0TXBUF = (dac_Val | pD_Bits);			// Control bits and data transmitted
	while (!UCBUSY) {};
		P1OUT |= BIT6;						// Pulls SYNC high
//		__delay_cycles(100);			//just to test to see if its going too fast
}
