#include <msp430.h> 

void Science_Board_Cantilever_1() {

	P1OUT &= ~BIT0;						// Turns off LED1 & turn LED2 on for 1 second
	P9OUT |= BIT7;
	__delay_cycles(1000000);

}

void Science_Board_Cantilever_2() {

	P1OUT |= BIT0;						// Turns on LED1 & turn LED2 off for 1 second
	P9OUT &= ~BIT7;
	__delay_cycles(1000000);

}

void Science_Board_Camera() {

	P1OUT |= BIT0;						// Turns on LED1 & LED2 for 1 second
	P9OUT |= BIT7;
	__delay_cycles(1000000);

}

void Science_Board_Caldwell_High() {
	int i;
	for (i = 0; i<6; i++) {			// Alternate LED1 & LED2 on and off for 1 second
		P1OUT ^= BIT0;
		P9OUT ^= BIT7;
		__delay_cycles(100000);
	}
}

/* main.c */
int main(void) {

    WDTCTL = WDTPW | WDTHOLD;			// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;				// Needed for FRAM

    P1DIR |= BIT0;						// Set LED1 as an output
    P9DIR |= BIT7;						// Set LED2 as an output

    while(1) {

    	Science_Board_Cantilever_1();	// Off Solid for 1 second

    	Science_Board_Cantilever_2();	// Solid Off for 1 second

    	Science_Board_Camera();		// Solid Solid for 1 second

    	Science_Board_Caldwell_High();	// Flashing Flashing for 1 second

     }

	return 0;
}
