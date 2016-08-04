#include <msp430.h> 

/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;		// for FRAM

    //blah blah this is just to test GitHub import/export ^.^
	
	return 0;
}
