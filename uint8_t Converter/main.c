#include <msp430.h> 
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

// main.c
// demo by Aaron Ewing
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // disable high impedence mode
	
    uint16_t var1 = 60000;		// example data
    uint32_t var2 = 4000000000	// example data

    16_To_8(var1, 0);

    32_Tp_8(var2, 1);

	return 0;
}
