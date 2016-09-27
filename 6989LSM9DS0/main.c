//  Description: This demo connects a MSP430FR6989 launchpad to an LSM9DS0 via
//  USCI_B1 I2C module. The launchpad acts as a master and the LSM9DS0 acts as a
//  slave. Master reads 6 byte consecutively and send it to the vitual COM
//  Port on Usb with USCI_A0 Uart module. DCO = 1MHz
//
//                  LSM9DS0          			 MSP430FR6989
//                   slave            			    master
//             -----------------  			   -----------------
//            |              SDA|<------+---->|P3.1/UCB1SDA     |
//            |              CLK|<--          |                 |
//            |              GND|<--          |                 |
//            |              SCL|<-+--------->|P3.2/UCB1SCL     |
//            |              VCC|<--.         |                 |
//            |              VIO|<--'3.3V     |                 |
//            |              INT|             |                 |

//  Original code modified by Braden Grim
//******************************************************************************
#include <msp430fr6989.h>
#include <stdio.h>
#include <stdint.h>
#include "LSM9DS0.h"
#include "i2c.h"

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 				// Stop WDT
	PM5CTL0 &= ~LOCKLPM5;

//Set clock speed
	CSCTL0_H = CSKEY >> 8;                    			// Unlock CS registers
	CSCTL1 = DCOFSEL_6;                       				// Set DCO to 8MHz
	CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK; // Set SMCLK = MCLK = DCO
	// ACLK = VLOCLK
	CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;    			// Set all dividers to 1
	CSCTL0_H = 0;                             				// Lock CS registers
//Set clock speed

	init_i2c();
	__delay_cycles(800);
	initGyro();
	__delay_cycles(800);
	initXM();
	__delay_cycles(800);

	while (1) {
	run_IMU();
	}
}
