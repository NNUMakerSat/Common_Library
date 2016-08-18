#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "Initialize.h"
#include "UART_Polling.h"

uint8_t TX_Data = 0;
uint8_t RX_Data;

// main.c
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

	initialize_Clocks ();					// Sets up timers (takes care of FRAM issue)
	initialize_Ports();						// Init all non used ports

	bool baud_Rate = 1;					// Sets the Baud rate
	bool pin_Setting = 0;				// selects the pins used

	init_UART (baud_Rate, pin_Setting);		// baud_Rate -> 0 = 9600 baud, 1 = 38,400 baud, default = 0
											// pin_Setting -> 0 = P2.0 - TX, P2.1 - RX, P1.4 Radio Busy (default)
											// pin_Setting -> 1 = P3.4 - TX, P3.5 - RX, P1.4 Radio Busy
    while(1) {
    	write_UART (TX_Data);
    	++TX_Data;
    	read_UART ();						// read RX_Data
    	__delay_cycles(100000);
    }
}
