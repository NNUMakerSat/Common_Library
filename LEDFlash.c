#include <msp430.h> 

/*
 This Code should make the two LEDs on the Launchpad alternate in flashing, starting with
 LED1. 
 */
int main(void)
{
    WDTCTL = WDTPW | WDTHOLD;			// Stop watchdog timer
	PM5CTL0 &= ~LOCKLPM5;

	P1DIR |= BIT0;						// Set LED1 as an output
	P9DIR |= BIT7;						// Set LED2 as an output

	for(;;)
	{


		P1OUT = BIT0;					// Turn on LED1
		__delay_cycles(1000000);
		P1OUT &= ~BIT0;					// Turn off LED1

		P9OUT = BIT7;					// Turn on LED2
		__delay_cycles(1000000);
		P9OUT &= ~BIT7;					// Turn off LED2 

	}
	return 0;
}
