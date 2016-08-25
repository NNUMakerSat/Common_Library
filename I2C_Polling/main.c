/* Program that will allow simple I2C communication to take place.
 *
 */

#include <msp430.h> 
#include <stdint.h>
#include "Initialize.h"
#include "I2C_Polling.h"

// main.c
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

	initialize_Clocks ();					// Sets up timers (takes care of FRAM issue)
	initialize_Ports();						// Init all non used ports


	
	return 0;
}
