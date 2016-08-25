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

#define G_GAIN		0.07		//[deg/s/LSB]
#define A_GAIN		0.732		//[mg/LSB]
#define thousand	1000		//[mg/LSB]

char xRate_L;  // Variable to hold X_Low register value
char xRate_H;  // Variable to hold X_High register value
char yRate_L;  // Variable to hold Y_Low register value
char yRate_H;  // Variable to hold Y_High register value
char zRate_L;  // Variable to hold Z_Low register value
char zRate_H;  // Variable to hold Z_High register value
char xaccel_L;
char xaccel_H;
char yaccel_L;
char yaccel_H;
char zaccel_L;
char zaccel_H;

int gyrRawx;
int gyrRawy;
int gyrRawz;
float gyroXangle;
float gyroYangle;
float gyroZangle;

short accRawx;
short accRawy;
short accRawz;
float g_x;
float g_y;
float g_z;

float rate_gyr_x;
float rate_gyr_y;
float rate_gyr_z;
char gyro_Address = 0x6A;				//Use 6A on berryIMU, 6B on Hub
char XMAddress = 0x1E;					//Use 1E on berryIMU, 1D on Hub
char outputRegister;
char outString[50]; 					//character string to output to Putty
int i;

void init_I2C(void);
void initUart(void);
void initGyro(void);
void initXM(void);
char read(void);

int main(void) {
	WDTCTL = WDTPW + WDTHOLD;                 			// Stop WDT
	PM5CTL0 &= ~LOCKLPM5;

////////////////////////Set clock speed///////////////////////////
	CSCTL0_H = CSKEY >> 8;                    				// Unlock CS registers
	CSCTL1 = DCOFSEL_6;                       				// Set DCO to 8MHz
	CSCTL2 = SELA__VLOCLK | SELS__DCOCLK | SELM__DCOCLK; 	// Set SMCLK = MCLK = DC0 = ACLK = VLOCLK
	CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;     				// Set all dividers to 1
	CSCTL0_H = 0;                             				// Lock CS registers

////////////////////////Set clock speed///////////////////////////
	//P9DIR |= 0xE0;

	rate_gyr_x = 0;
	rate_gyr_y = 0;
	rate_gyr_z = 0;

	init_I2C();
	//P9OUT = 0xA0;				//Turn on red LED after i2c initialization
	initUart();
	__delay_cycles(800);
	initGyro();
	//P9OUT = 0x40;				//Turn on LED after gyroscope initialization
	__delay_cycles(800);
	initXM();

	while (1) {
		////////////READ GYROSCOPE//////////////
		UCB1I2CSA = gyro_Address;

		outputRegister = OUT_X_L_G;
		xRate_L = read();

		outputRegister = OUT_X_H_G;
		xRate_H = read();

		outputRegister = OUT_Y_L_G;
		yRate_L = read();

		outputRegister = OUT_Y_H_G;
		yRate_H = read();

		outputRegister = OUT_Z_L_G;
		zRate_L = read();

		outputRegister = OUT_Z_H_G;
		zRate_H = read();

		/////////////READ ACCELEROMETER/////////////////
		UCB1I2CSA = XMAddress;

		outputRegister = OUT_X_L_A;
		xaccel_L = read();

		outputRegister = OUT_X_H_A;
		xaccel_H = read();

		outputRegister = OUT_Y_L_A;
		yaccel_L = read();

		outputRegister = OUT_Y_H_A;
		yaccel_H = read();

		outputRegister = OUT_Z_L_A;
		zaccel_L = read();

		outputRegister = OUT_Z_H_A;
		zaccel_H = read();

		////////////////CONVERT TO MEANINGFUL DATA//////////////////////
		gyrRawx = (xRate_L | xRate_H << 8);
		gyrRawy = (yRate_L | yRate_H << 8);
		gyrRawz = (zRate_L | zRate_H << 8);

		rate_gyr_x = gyrRawx * G_GAIN;
		rate_gyr_y = gyrRawy * G_GAIN;
		rate_gyr_z = gyrRawz * G_GAIN;

		accRawx = (xaccel_L | xaccel_H << 8);
		accRawy = (yaccel_L | yaccel_H << 8);
		accRawz = (zaccel_L | zaccel_H << 8);

		g_x =( accRawx * A_GAIN)/thousand;
		g_y =( accRawy * A_GAIN)/thousand;
		g_z =( accRawz * A_GAIN)/thousand;

		/////////////SEND DATA THROUGH UART///////////////////////////
		UCA0TXBUF = zaccel_H;
		while(UCA0IFG == 0){};

		/*sprintf(outString, "You took %d ms\n", g_x);
		for(i = 0; outString[i] != 0x00; ++i) {
		UCA1TXBUF = outString[i];
		while(UCA1IFG == 0) {
		}
		}*/
	}
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char read() {
	uint8_t data;
	data = 0;
	while (UCB1CTL1 & UCTXSTP);               		// Ensure stop condition got sent
		UCB1CTL1 |= UCTR + UCTXSTT;               	// I2C TX with start condition
	while ((UCB1CTL1 & UCTXSTT) || !(UCB1IFG & UCTXIFG0));
		UCB1TXBUF = outputRegister;
	while (!(UCB1IFG & UCTXIFG0));
		UCB1CTL1 &= ~UCTR;                     		// Clear I2C TX flag
		UCB1CTL1 |= UCTXSTT;
	while (UCB1CTL1 & UCTXSTT);                     // I2C start condition
		UCB1CTL1 |= UCTXSTP;
	while (!(UCB1IFG & UCRXIFG0));
		data = UCB1RXBUF;

	return data;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char write( registerAddress, setBits) {

	while (UCB1CTL1 & UCTXSTP);               				// Ensure stop condition got sent
		UCB1CTL1 |= UCTR + UCTXSTT;               			// I2C TX with start condition
	while ((UCB1CTL1 & UCTXSTT) || !(UCB1IFG & UCTXIFG0));
		UCB1TXBUF = registerAddress;
	while ((UCB1CTL1 & UCTXSTT) || !(UCB1IFG & UCTXIFG0));
		UCB1TXBUF = setBits;                     			// Clear I2C TX flag
		UCB1CTL1 |= UCTXSTP;
	return 0;
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void initGyro(void) {
	UCB1I2CSA = gyro_Address;                   	// Slave Address is 069h

	while (UCB1CTL1 & UCTXSTP);               		// Ensure stop condition got sent
		UCB1CTL1 |= UCTR + UCTXSTT;              	// I2C TX with start condition
	while (UCB1CTL1 & UCTXSTT);
		UCB1TXBUF = CTRL_REG1_G; 					// Low pass filter value register address to TXBuffer
	while ((UCB1IFG & UCTXIFG) == 0); 				//ADD CONDITION HERE TO CHECK FOR ACK
		UCB1TXBUF = 0x0F;     						// config value to TXBuffer
	while ((UCB1IFG & UCTXIFG) == 0);
		UCB1CTL1 |= UCTXSTP;                   		// I2C stop condition
		UCB1IFG &= ~UCTXIFG;                   		// Clear USCI_B0 TX int flag

	while (UCB1CTL1 & UCTXSTP);               		// Ensure stop condition got sent
		UCB1CTL1 |= UCTR + UCTXSTT;               	// I2C TX with start condition
	while (UCB1CTL1 & UCTXSTT);
		UCB1TXBUF = CTRL_REG4_G;
	while ((UCB1IFG & UCTXIFG) == 0);				// Sample Rate Divider value to TXBuffer
		UCB1TXBUF = 0x30; 							// config value to TXBuffer
	while ((UCB1IFG & UCTXIFG) == 0);
		UCB1CTL1 |= UCTXSTP;                    	// I2C stop condition
		UCB1IFG &= ~UCTXIFG;                     	// Clear USCI_B0 TX int flag
}

void initXM(void) {
	UCB1I2CSA = XMAddress;                   		// Slave Address is 069h

	while (UCB1CTL1 & UCTXSTP);               		// Ensure stop condition got sent
		UCB1CTL1 |= UCTR + UCTXSTT;             	// I2C TX with start condition
	while (UCB1CTL1 & UCTXSTT);
		UCB1TXBUF = CTRL_REG1_XM; 					// Low pass filter value register address to TXBuffer
	while ((UCB1IFG & UCTXIFG) == 0); 				//ADD CONDITION HERE TO CHECK FOR ACK
		UCB1TXBUF = 0x67;     						// config value to TXBuffer
	while ((UCB1IFG & UCTXIFG) == 0);
		UCB1CTL1 |= UCTXSTP;                    	// I2C stop condition
		UCB1IFG &= ~UCTXIFG;                    	// Clear USCI_B0 TX int flag

	while (UCB1CTL1 & UCTXSTP);               		// Ensure stop condition got sent
		UCB1CTL1 |= UCTR + UCTXSTT;               	// I2C TX with start condition
	while (UCB1CTL1 & UCTXSTT);
		UCB1TXBUF = CTRL_REG2_XM;					// Sample Rate Divider value to TXBuffer
	while ((UCB1IFG & UCTXIFG) == 0);
		UCB1TXBUF = 0x20; 							// config value to TXBuffer
	while ((UCB1IFG & UCTXIFG) == 0);
		UCB1CTL1 |= UCTXSTP;                    	// I2C stop condition
		UCB1IFG &= ~UCTXIFG;                     	// Clear USCI_B0 TX int flag

	while (UCB1CTL1 & UCTXSTP);               		// Ensure stop condition got sent
		UCB1CTL1 |= UCTR + UCTXSTT;            		// I2C TX with start condition
	while (UCB1CTL1 & UCTXSTT);
		UCB1TXBUF = CTRL_REG5_XM;					// Sample Rate Divider value to TXBuffer
	while ((UCB1IFG & UCTXIFG) == 0);
		UCB1TXBUF = 0xF0; 							// config value to TXBuffer
	while ((UCB1IFG & UCTXIFG) == 0);
		UCB1CTL1 |= UCTXSTP;                    	// I2C stop condition
		UCB1IFG &= ~UCTXIFG;                    	// Clear USCI_B0 TX int flag

	while (UCB1CTL1 & UCTXSTP);               		// Ensure stop condition got sent
		UCB1CTL1 |= UCTR + UCTXSTT;               	// I2C TX with start condition
	while (UCB1CTL1 & UCTXSTT);
		UCB1TXBUF = CTRL_REG6_XM;					// Sample Rate Divider value to TXBuffer
	while ((UCB1IFG & UCTXIFG) == 0);
		UCB1TXBUF = 0x60; 							// config value to TXBuffer
	while ((UCB1IFG & UCTXIFG) == 0);
		UCB1CTL1 |= UCTXSTP;                    	// I2C stop condition
		UCB1IFG &= ~UCTXIFG;                     	// Clear USCI_B0 TX int flag

	while (UCB1CTL1 & UCTXSTP);               		// Ensure stop condition got sent
		UCB1CTL1 |= UCTR + UCTXSTT;               	// I2C TX with start condition
	while (UCB1CTL1 & UCTXSTT);
		UCB1TXBUF = CTRL_REG7_XM;					// Sample Rate Divider value to TXBuffer
	while ((UCB1IFG & UCTXIFG) == 0);
		UCB1TXBUF = 0x00; 							// config value to TXBuffer
	while ((UCB1IFG & UCTXIFG) == 0);
		UCB1CTL1 |= UCTXSTP;                    	// I2C stop condition
		UCB1IFG &= ~UCTXIFG;                     	// Clear USCI_B0 TX int flag
}

void init_I2C(void) {
	P3SEL0 |= BIT1 + BIT2;     							// Assign I2C pins to USCI_B1
	P3SEL1 &= ~(BIT1 + BIT2);     						// Assign I2C pins to USCI_B1
	UCB1CTLW0 |= UCSWRST;                      			// Enable SW reset
	UCB1CTLW0 |= UCMST + UCMODE_3 + UCSYNC + UCSSEL_2; 	// I2C Master, synchronous mode
	UCB1BRW = 80;                             			// fSCL = 1Mhz/100 = ~100kHz
	UCB1CTLW0 &= ~UCSWRST;                 				// **Initialize USCI state machine**
}

void initUart(void) {
	P2SEL0 |= BIT0 + BIT1;								//Initialize Port 2 pins 0 and 1 to UART mode
	P2SEL1 &= ~(BIT0 + BIT1);							//Initialize Port 2 pins 0 and 1 to UART mode
	UCA0CTL1 |= UCSWRST;								// Enable SW reset
	UCA0CTL1 |= UCSSEL_2;                     			// Use SMCLK
//	UCA0BRW = 104;
	UCA0BRW =13;
	UCA0CTL1 &= ~UCSWRST;
}
