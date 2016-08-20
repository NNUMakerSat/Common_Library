/*
 * LED.c
 *
 *  Created on: Aug 20, 2016
 *      Author: aaronewing
 */
#include <stdint.h>
#include "LED.h"

uint32_t g_timer1;						// init timer for LED 1 (so won't have to destroy variable)
uint32_t g_timer2;						// init timer for LED 2 (so won't have to destroy variable)

void LED_1_On(void) {
	P1DIR |= BIT0;						// Set LED1 as an output
	P1OUT = BIT0;						// Turn on LED1
}

void LED_2_On(void) {
	P9DIR |= BIT7;						// Set LED2 as an output
	P9OUT |= BIT7;						// Turn on LED2
}

void LED_1_Off(void) {
	P1DIR |= BIT0;						// Set LED1 as an output
	P1OUT &= ~BIT0;						// Turn off LED1

}

void LED_2_Off(void) {
	P9DIR |= BIT7;						// Set LED2 as an output
	P9OUT &= ~BIT7;					// Turn off LED2
}

void flash_LED_1(uint32_t clk_Delay1, uint32_t duration1) {
	P1DIR |= BIT0;						// Set LED1 as an output

	for (g_timer1 = 0; g_timer1 < duration; ++g_timer1) {	// Flash LED however long you want at whatever frequency you want
		P1OUT = BIT0;
		__delay_cycles(clk_Delay);
		P1OUT &= ~BIT0;
	}
}
	void flash_LED_2(uint8_t clk_Delay2, uint8_t duration2) {
	P9DIR |= BIT7;						// Set LED2 as an output

	for (g_timer2 = 0; g_timer2 < duration; ++g_timer2) {	// Flash LED however long you want at whatever frequency you want
		P9OUT = BIT7;
		__delay_cycles(clk_Delay);
		P9OUT &= ~BIT7;
	}
}
