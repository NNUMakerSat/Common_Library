#include <msp430fr6989.h>
#include "Initialize.h"
/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // disable high impedence mode

    initialize_Ports();
	
	return 0;
}
