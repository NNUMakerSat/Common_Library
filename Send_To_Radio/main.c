#include <msp430.h> 
#include "Send_To_Radio.h"

// main.c
int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer

    while (1) {
    Send_To_Radio();
    }
}
