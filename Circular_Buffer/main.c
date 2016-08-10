#include <msp430.h> 
#include <msp430fr6989.h>
#include <stdint.h>
#include <stdbool.h>
#include "Circular_Buffer.h"


 // buffer demo by Dr. Griffin
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // disable high impedence mode

    uint8_t data;
    data = 52;					// since value is ASCII it will apepar as its ASCII value (ie 52 = "4")
	
    init_Buffer();

    while(data != 62) {

        if(!is_Buffer_Full()){
            write_Buffer(data);
        }

        if(!is_Buffer_Empty()){
            data = read_Buffer();
        }
        ++data;
    }

	return 0;
}
