/*
 * Send_To_Radio.c
 *
 *  Created on: Aug 19, 2016
 *      Author: aaronewing
 */
#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "Circular_Buffer.h"
#include "Initialize.h"
#include "UART_Polling.h"
#include "Send_To_Radio.h"


uint8_t TX_Data = 0;
uint8_t RX_Data;
uint8_t collect_Array[35];
uint8_t i;
uint8_t j = 0;

// main.c
void Send_To_Radio(void) {
//	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

	initialize_Clocks ();					// Sets up timers (takes care of FRAM issue)
	initialize_Ports();						// Init all non used ports

	bool baud_Rate = 1;						// Sets the Baud rate
	bool pin_Setting = 0;					// selects the pins used

	init_UART (baud_Rate, pin_Setting);		// baud_Rate -> 0 = 9600 baud, 1 = 38,400 baud, default = 0
											// pin_Setting -> 0 = P2.0 - TX, P2.1 - RX, P1.4 Radio Busy (default)
											// pin_Setting -> 1 = P3.4 - TX, P3.5 - RX, P1.4 Radio Busy

	init_Buffer();							// Init circular buffer

	for (i = 0; i < 35; ++i) {		// Put 0, 1, 2, 3... into circular buffer
		write_Buffer(TX_Data);
		++TX_Data;
	}

	while(1) {
		write_UART (read_Buffer());			// write what is in buffer
	    read_UART ();						// read RX_Data

	    collect_Array[j] = RX_Data;
	    ++j;
//	    __delay_cycles(100000);
	}


/*    while(1) {
    	write_UART (TX_Data);
    	++TX_Data;
    	read_UART ();						// read RX_Data
    	__delay_cycles(100000);
    } */
}



