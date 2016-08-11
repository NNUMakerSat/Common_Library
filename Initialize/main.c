#include <msp430fr6989.h>
#include "Initialize.h"

// main.c by Aaron Ewing
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // disable high impedence mode

    initialize_Clocks();		// Sets all clocks to standard position (tested, and works)
    initialize_Ports();			// Sets unused ports to proper settings (works)

	return 0;
}
