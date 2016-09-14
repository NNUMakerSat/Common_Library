#include <msp430.h>
#include <stdint.h>
#include "Initialize.h"
#include "Circular_Buffer.h"
//#include "LED.h"
//#include "SPI_Polling.h"
#include "SPI_Main.h"
#include "Packetizer.h"
#include "UART_Polling.h"

#define DAC_PD_NORMAL 0x3FFF
#define DAC_PD_LOWPOWER 0x2FFF

#define SCI_1_SEL 0x10
#define SCI_2_SEL 0x20
#define SCI_3_SEL 0x40
#define SCI_4_SEL 0x80

uint8_t data2[500];
uint8_t data3[500];

volatile int SCI_board = 2;
uint8_t j = 0;
uint8_t i = 0;

// test

//uint16_t dac_Val = 0x0000;
uint8_t tx_Data_8 = 0x40;
//uint16_t tx_Data_16 = 0xBADD;
uint8_t g_RXData;

uint8_t example_RAD[30] = {0x22, 0x00, 0x44, 0x55, 0x52, 0x33, 0x55, 0x10,					// new
			0x11, 0x91, 0x00, 0x44, 0x55, 0x69, 0x15, 0x22, 0x33, 0x44, 0x55,
			0x22, 0x33, 0x55, 0x22, 0x11, 0x91, 0x01, 0x44, 0x55, 0xFF, 0xDD};
uint8_t rxByteCount = 0;
uint32_t source_ID = 1;															// must be 32 bit
uint8_t SPI_Counter;
uint8_t UART_Counter;
uint8_t test;


enum SCI_States { SCI_SMStart, SCI_2, SCI_3, SCI_Done} SCI_State;

void SCI_Loop()
{
	switch(SCI_State){
		case SCI_SMStart:
			SCI_State = SCI_2;
			break;

		case SCI_2:
			P4OUT |= BIT6;
			P4OUT &= ~(BIT4 & BIT5 & BIT7);
			while(g_RXData != 0xFF){
				if (!(P4IN & BIT1)){
					while (!(P4IN & BIT1)) {}							// Waits for GPIO to go high
//					read_SPI ();
					Read_SB_SPI ();
					rxByteCount++;												// new
//					data2[i] = g_RXData;
					write_Buffer(g_RXData);										// new
					i++;
				}
			}

			g_RXData = 0x00;
			i = 0x00;
			SCI_State = SCI_3;
			break;

		case SCI_3:
	    	P4OUT |= BIT6;
			P4OUT &= ~(BIT4 & BIT5 & BIT7);
			while(g_RXData != 0xFF){
				if (!(P4IN & BIT1)){
					while (!(P4IN & BIT1)) {}							// Waits for GPIO to go high
//					read_SPI ();
					Read_SB_SPI ();
					rxByteCount++;												// new
//					data3[i] = g_RXData;
					write_Buffer(g_RXData);										// new
				}
				i++;
			}
			SCI_State = SCI_Done;
			break;

		case SCI_Done:
			P9OUT = BIT0;
			while (1){}
			break;
	}
}

// main.c
int main(void) {
	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
//	uint8_t eg_Counter;
//	uint8_t eg_Counter2;
	uint16_t read[15];						// Hub's copy array

	initialize_Ports();						// Init all non used ports
	initialize_Clocks();					// Sets up timers (takes care of FRAM issue)

	init_Buffer();							// Init circular buffer
	init_UART (1, 0);						// baud_Rate -> 0 = 9600 baud, 1 = 38,400 baud, default = 1
											// pin_Setting -> 0 = P2.0 - TX, P2.1 - RX, P2.2 Radio Busy (default)
											// pin_Setting -> 1 = P3.4 - TX, P3.5 - RX, P1.4 Radio Busy

	uint8_t pin_Setting = 0;				// selects the pins used for 6989
	uint8_t device_CS = 0;					// selects the SYNC/SS pin (5k POT)

	P9DIR |= BIT5 + BIT6 + BIT7;
	P9SEL0 &= !(BIT5 + BIT6 + BIT7);
	P9SEL1 &= !(BIT5 + BIT6 + BIT7);
	P9IN = 0x00;
	P9OUT = 0x00;


	init_SPI (pin_Setting, device_CS);
	// clk_Rate -> 8 = 8MHz, 4 = 4MHz, 2 = 2.67MHz, 1 = 1MHz (currently only at 1MHz)
	//P1.4 - CLK, P1.6 - SIMO, P1.7 - SOMI
/*
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
*/



////////////////////////// Slave ////////////////////////////////////////
//	flash_LED_1(0,5);							// Makes Hub wait for reply
//	LED_1_On();

//    for (eg_Counter = 0; eg_Counter < 10; ++eg_Counter) { //loop 10 times
//    	write_uint16_SPI (tx_Data_16, device_CS); 	// device 0 is Hub

//	write_uint8_SPI (tx_Data_8, device_CS); 	// device 0 is Hub

	SCI_State = SCI_SMStart;
/*    while (1) {
    SCI_Loop();
    } */

	for (SPI_Counter = 0; SPI_Counter < 10; SPI_Counter++) {						// reads 10 bytes from SB 5969
		SCI_Loop();
	}

/*	for (test = 0; test < 30; ++test) {
			write_Buffer(example_RAD[test]);
			rxByteCount++;
		} */
	P1DIR |= BIT0;
	P1OUT |= BIT0;

	Packetizer(source_ID, rxByteCount);												// packetizes info

	for (UART_Counter = 0; UART_Counter < 39; UART_Counter++) {						// sends complete packet to radio
		write_UART (get_Data(UART_Counter, source_ID));
	}

	while (1) {}

}
/////////////////////////// Slave /////////////////////////////////////////////

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
