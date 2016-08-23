/* Code is for testing Hub functionality with radio
 * after a vibe test. The code is supposed to turn on LED(s)
 * while running code, and transmit a single packet to the radio
 */

#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "Initialize.h"
#include "LED.h"
#include "UART_Polling.h"

// main.c
int main(void) {
	uint8_t i;
	uint8_t j = 0;
	uint8_t RX_Data;
	uint8_t collect_Array[39];

	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

	initialize_Ports();
	initialize_Clocks();
	init_LED();

	LED_1_On();								// Turn LED 1 on, indicating power

	init_UART(0, 0);						// Initalize UART at Baud = 9600 with P2.0 - TX, P2.1 - RX, P1.4 - Radio Busy line

	for (i = 0; i < 39; ++i) {						//reads what emulator puts in.
		read_UART();
		LED_2_On();
		collect_Array[j] = RX_Data;
		++j;
	}
	while (1) {};
}
