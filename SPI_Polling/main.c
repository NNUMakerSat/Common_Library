#include <msp430.h> 
#include <stdint.h>
#include "Initialize.h"
#include "SPI_Polling.h"

#define DAC_PD_NORMAL 0x3FFF
#define DAC_PD_LOWPOWER 0x2FFF

uint16_t dac_Val = 0x0000;
uint8_t tx_Data_8 = 100;
uint16_t tx_Data_16;

// main.c
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer

	initialize_Clocks ();					// Sets up timers (takes care of FRAM issue)
	initialize_Ports();						// Init all non used ports

	uint8_t clk_Rate = 1;					// Sets the Baud rate
	uint8_t pin_Setting = 5;				// selects the pins used
//	uint8_t device_CS = 11;					// selects the SYNC/SS pin (DAC)
	uint8_t device_CS = 1;					// selects the SYNC/SS pin (5k POT)

	init_SPI (clk_Rate, pin_Setting);		// clk_Rate -> 8 = 8MHz, 4 = 4MHz, 2 = 2.67MHz, 1 = 1MHz
													// pin_Setting -> default =>  P1.5 - UCA0CLK, P1.6 - SYNC, P2.0 - SIMO, P2.1 - SOMI

	P1SEL0 &= ~BIT4;
	P1SEL1 &= ~BIT4;							// P1.4 - SYNC/Slave Select (5k POT)
	P1DIR |= BIT4;
	P1OUT |= BIT4;

    while(1) {
//    	tx_Data_16 = (dac_Val & DAC_PD_NORMAL);		// Data to transmit (both value and control bits)
//    	write_uint16_SPI (tx_Data_16, device_CS); 						(DAC)
//    	++dac_Val;

    	write_uint8_SPI (tx_Data_8, device_CS);
    	++tx_Data_8;													// (5k POT)
    	__delay_cycles(100000);
    }
}
