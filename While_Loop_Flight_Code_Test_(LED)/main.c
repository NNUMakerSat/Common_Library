#include <msp430.h> 

void Sleep() {

	P1OUT |= BIT0;					// Turn on LED1 & LED2 for 2 second
	P9OUT |= BIT7;
	__delay_cycles(2000000);
}

void Radio() {
	int i;

		P1OUT |= BIT0;					// Turn on LED1

		for (i = 0; i<5; i++) {			// Flash LED2 on and off for 1 second
			P9OUT |= BIT7;
			__delay_cycles(100000);
			P9OUT &= ~BIT7;
			__delay_cycles(100000);
		}
}

void Science_Board_IMU() {
	int i;

	P1OUT = ~BIT0;					// Turn off LED1

	for (i = 0; i<5; i++) {			// Flash LED2 on and off for 1 second
		P9OUT |= BIT7;
		__delay_cycles(100000);
		P9OUT &= ~BIT7;
		__delay_cycles(100000);
	}
}

/* int Science_Board_Cantilever(cantilever_Select)	{
	int i;

	if (cantilever_Select == 1) {		// Decides which Cantilever Board to turn on

		P1OUT &= ~BIT0;					// Turns off LED1 & LED2 on for 1 second
		P9OUT |= BIT7;
		__delay_cycles(1000000);

		cantilever_Select = 2;			// Alternates so the other Cantilever Board goes next time

	} else {		// Decides which Cantilever Board to turn on

		P9OUT &= ~BIT7;					// Turn off LED 2
		for (i = 0; i<5; i++) {			// Flash LED1 on and off for 1 second
			P1OUT |= BIT0;
			__delay_cycles(100000);
			P1OUT |= ~BIT0;
			__delay_cycles(100000);
		}

		cantilever_Select = 1;			// Alternates so the other Cantilever Board goes next time
	}
	return cantilever_Select;
}
*/

void Science_Board_Cantilever_1() {

	P1OUT &= ~BIT0;					// Turns off LED1 & LED2 on for 1 second
	P9OUT |= BIT7;
	__delay_cycles(1000000);
}

void Science_Board_Cantilever_2() {
	int i;

	P9OUT &= ~BIT7;					// Turn off LED 2
	for (i = 0; i<5; i++) {			// Flash LED1 on and off for 1 second
		P1OUT |= BIT0;
		__delay_cycles(100000);
		P1OUT &= ~BIT0;
		__delay_cycles(100000);
	}

}

void Science_Board_Camera() {
	int i;

	for (i = 0; i<5; i++) {			// Alternate LED1 & LED2 on and off for 1 second
		P1OUT |= BIT0;
		P9OUT &= ~BIT7;
		__delay_cycles(100000);
		P1OUT &= ~BIT0;
		P9OUT |= BIT7;
		__delay_cycles(100000);
		}
}

void Science_Board_Caldwell_High() {
	int i;

		P9OUT |= BIT7;					// Turn on LED2

		for (i = 0; i<5; i++) {			// Flash LED1 on and off for 1 second
			P1OUT |= BIT0;
			__delay_cycles(100000);
			P1OUT &= ~BIT0;
			__delay_cycles(100000);
		}
}

void Deep_Sleep() {

	P1OUT |= BIT0;					// Turn on LED1 & LED2 for 5 second
	P9OUT &= ~BIT7;
	__delay_cycles(5000000);
}

/* main.c */
int main(void) {

    WDTCTL = WDTPW | WDTHOLD;			// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;				// Needed for FRAM

    P1DIR |= BIT0;						// Set LED1 as an output
    P9DIR |= BIT7;						// Set LED2 as an output

//    int cantilever_Select = 1;			// Used for selecting Cantilevers


    while(1) {

    		Sleep();									// Solid Solid for 2 seconds

    	Science_Board_IMU();							// Off Flashing for 1 second

    		Sleep();									// Solid Solid for 2 seconds

    	Radio();										// Solid Flashing for 1 second

    		Sleep();									// Solid Solid for 2 seconds

 //   	Science_Board_Cantilever(cantilever_Select);	// Cantilever 1
    	Science_Board_Cantilever_1();					// Off Solid for 1 second

    		Sleep();									// Solid Solid for 2 seconds

    	Radio();										// Solid Flashing for 1 second

    		Sleep();									// Solid Solid for 2 seconds

//   	Science_Board_Cantilever(cantilever_Select);	// Cantilever 2
     	Science_Board_Cantilever_2();					// Flashing Off for 1 second

     		Sleep();									// Solid Solid for 2 seconds

    	Radio();										// Solid Flashing for 1 second

    		Sleep();									// Solid Solid for 2 seconds

    	Science_Board_Camera();							// Flashing Flashign for 1 second

    		Sleep();									// Solid Solid for 2 seconds

    	Radio();										// Solid Flashing for 1 second

    		Sleep();									// Solid Solid for 2 seconds

    	Science_Board_Caldwell_High();					// Flashing Solid for 1 second

    		Sleep();									// Solid Solid for 2 seconds

    	Radio();										// Solid Flashing for 1 second

    		Sleep();									// Solid Solid for 2 seconds

    	Deep_Sleep();									// Solid Off for 5 seconds
    }
}
