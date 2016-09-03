//#include <msp430.h>
#include "MakerSat.h"

// main.c
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
	
    LED_1_On();

	return 0;
}
