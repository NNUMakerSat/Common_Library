#include <msp430.h> 
#include <stdint.h>
/*
 * main.c
 */
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;

 //   SYSCFG0 = FRWPPW | DFWP;

    // fram function that writes to FRAM, or somehow make EVERYTHING
    // FRAM? Also, HOW?
    uint8_t FRAM_i;
    uint8_t FRAM_j = 0x22;
//	#pragma PERSISTENT(OReally)
    /*__persistent */ uint8_t OReally = 0x11;

    int FRAM_int = 0xAAAA;
    while (FRAM_int < 0xAAAF) {
    	FRAM_int++;
    }
    uint8_t FRAM_array[10];
    uint8_t FRAM_array2[10] = {0xAB, 0xBC, 0xCD, 0xDE, 0xEF, 0xFF, 0x11, 0x55, 0x33, 0x44};
	
    FRAM_array[1] = 0x56;

	return 0;
}
