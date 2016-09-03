#include <msp430.h>
#include <stdint.h>
#include "Initialize.h"
#include "LED.h"
#include "SPI_Polling.h"

#define DAC_PD_NORMAL 0x3FFF
#define DAC_PD_LOWPOWER 0x2FFF

#define SCI_1_SEL 0x10
#define SCI_2_SEL 0x20
#define SCI_3_SEL 0x40
#define SCI_4_SEL 0x80

uint16_t dac_Val = 0x0000;
uint8_t tx_Data_8 = 100;
uint16_t tx_Data_16 = 0xAAAA;
uint8_t g_RXData;

// main.c
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
	uint8_t eg_Counter;
	uint8_t eg_Counter2;
	uint16_t read[15];						// Hub's copy array

	initialize_Ports();						// Init all non used ports
	initialize_Clocks();					// Sets up timers (takes care of FRAM issue)

	uint8_t pin_Setting = 0;				// selects the pins used for 6989
	uint8_t device_CS = 0;					// selects the SYNC/SS pin (5k POT)

	init_SPI (pin_Setting);
	// clk_Rate -> 8 = 8MHz, 4 = 4MHz, 2 = 2.67MHz, 1 = 1MHz (currently only at 1MHz)
	//P1.4 - CLK, P1.6 - SIMO, P1.7 - SOMI

////////////////////////// Hub ////////////////////////////////////////
    for (eg_Counter = 0; eg_Counter < 10; ++eg_Counter) { //loop 10 times
    	read_SPI (); 	// device 0 is Hub
    	read[eg_Counter] = g_RXData;				// saves values read in array
    }

    for (eg_Counter2 = 0; eg_Counter2 < 10; ++eg_Counter2) { //loop 10 times
        	write_uint16_SPI (read[eg_Counter2], device_CS); 	// device 0 is Hub
       }

    while (1) {}
}
/////////////////////////// Hub /////////////////////////////////////////////



/*
////////////////////////// Slave ////////////////////////////////////////
	flash_LED_1(0,5);							// Makes Hub wait for reply
	LED_1_On();

    for (eg_Counter = 0; eg_Counter < 10; ++eg_Counter) { //loop 10 times
    	write_uint16_SPI (tx_Data_16, device_CS); 	// device 0 is Hub
    }

    while (1) {
    	flash_LED_2(1,1);
    }
}
/////////////////////////// Slave /////////////////////////////////////////////
*/
/*	P4DIR |= SCI_1_SEL;
	P4OUT |= SCI_1_SEL;

	P1SEL0 &= ~BIT4;
	P1SEL1 &= ~BIT4;							// P1.4 - SYNC/Slave Select (5k POT)
	P1SEL0 |= BIT6 + BIT7;
	P1DIR |= BIT4 + BIT6 + BIT7;
	P1OUT |= BIT4 + BIT6 + BIT7;

	while (1) {
		P1OUT ^= BIT4 + BIT6 + BIT7;
		__delay_cycles(1000);
	} */
