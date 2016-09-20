#include <msp430.h>
#include <stdint.h>
#include <stdbool.h>
#include "Initialize.h"
//#include "LED.h"
//#include "SPI_Polling.h"
#include "SPI_Main.h"

#define DAC_PD_NORMAL 0x3FFF
#define DAC_PD_LOWPOWER 0x2FFF

#define SCI_1_SEL 0x10
#define SCI_2_SEL 0x20
#define SCI_3_SEL 0x40
#define SCI_4_SEL 0x80

uint16_t dac_Val = 0x0000;
uint8_t tx_Data_8 = 100;
uint16_t tx_Data_16 = 0xAAAA;
uint8_t g_RXData;						// changed from 8 bit
uint32_t error_Counter;
//	uint8_t array[10] = {0x10, 0x20, 0x30, 0x40, 0x50, 0x60, 0x70, 0x80, 0x90, 0x15};		//SB 1
	uint8_t array[10] = {0x90, 0x89, 0x88, 0x04, 0x05, 0x77, 0x00, 0x55, 0x69, 0x42};		//SB 2
uint8_t i = 0;
uint8_t j = 0;

// main.c
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
	uint8_t eg_Counter = 0;
	uint8_t eg_Counter2;
	uint8_t read[1];						// Hub's copy array

	initialize_Ports();						// Init all non used ports
	initialize_Clocks();					// Sets up timers (takes care of FRAM issue)

	uint8_t pin_Setting = 2;				// selects the pins used for 6989
	uint8_t device_CS = 1;					// selects the SYNC/SS pin (5k POT)

//	init_SPI (2, 1);		// 2 is slave (changed in header and course file)
	// clk_Rate -> 8 = 8MHz, 4 = 4MHz, 2 = 2.67MHz, 1 = 1MHz (currently only at 1MHz)
	//P1.4 - CLK, P1.6 - SIMO, P1.7 - SOMI, 4.1 - GPIO

//	flash_LED_2(1,10); // make hub wait
//	P4OUT |= BIT1;
//	UCB0TXBUF = 0x50;

	////////////////////
	init_SPI (0, device_CS);  //gpio p4.3
	/////////////////////

////////////////////////// Hub ////////////////////////////////////////
//    for (eg_Counter = 0; eg_Counter < 10; ++eg_Counter) { //loop 10 times
//    read_SPI (); 	// device 0 is Hub
//    	read[0] = g_RXData;				// saves values read in array
//   }

    for (i = 0; i < 10; ++i) {
    	write_uint8_SPI (array[i], device_CS);
    	if (UCB0RXBUF != 0xAA) {
    		write_uint8_SPI (array[i], device_CS);
    	}
    	//++j;
    }
    write_uint8_SPI (0xFF, device_CS);

/*
    while (read[0] = 0x00) {
    	read_SPI (); 	// device 0 is Hub
    	read[1] = g_RXData;
    	++error_Counter;
    }
    if (read[0] == 0x40) {								//changed reading to 16 bytes in the SPI_Polling .h .c
    	write_uint8_SPI (0x50, device_CS);
//    	write_uint16_SPI (0xBB, device_CS);
//   	write_uint16_SPI (0xCC, device_CS);
    	LED_1_On();
    }else {
    	write_uint8_SPI (0xF0, device_CS);
    	LED_2_On();
    }
/*    for (eg_Counter2 = 0; eg_Counter2 < 10; ++eg_Counter2) { //loop 10 times
        	write_uint16_SPI (read[eg_Counter2], device_CS); 	// device 0 is Hub
       }
*/
    P1DIR |= BIT0;
    P1OUT |= BIT0;
    while (1) {}
}
