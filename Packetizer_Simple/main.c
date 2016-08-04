#include <msp430.h> 
#include <stdint.h>				// needed for uint16_t to work

/* main.c */
	int main(void) {
    WDTCTL = WDTPW | WDTHOLD;	// Stop watchdog timer
    PM5CTL0 &= ~LOCKLPM5;
    uint16_t packetized_Data[39];			// will NOT work with just a byte..
    uint16_t packets_Converted = 0;

    uint16_t	pre_Packetized_Data[35] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 34};

	packetized_Data[0] = packetized_Data[1] = packetized_Data[2] = 0x50;
	packetized_Data[3] = 0x0C;

	uint16_t i = 4;
	for (; i < 39; i++) {			// while room left in packatized array
		packetized_Data[i] = pre_Packetized_Data[packets_Converted];
		pre_Packetized_Data[packets_Converted] = 0;
		++packets_Converted;
	}
	
	return 0;
}


