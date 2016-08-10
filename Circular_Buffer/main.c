#include <msp430.h> 
#include "Circular_Buffer.h"


 // buffer demo by Dr. Griffin
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;       // disable high impedence mode
	
    initBuffer();

        if(!isBufferFull()){
            writeBuffer(data);
        }

        if(!isBufferEmpty()){
            data = readBuffer();
        }

	return 0;
}
