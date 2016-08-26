/* Program that will allow simple I2C communication to take place.
 * The example being used is the IMU.
 */

#include <msp430.h> 
#include <stdint.h>
#include <stdbool.h>
#include "Initialize.h"
#include "I2C_Polling.h"
#include "LSM9DS0.h"

#define G_GAIN		0.07		//[deg/s/LSB]
#define A_GAIN		0.732		//[mg/LSB]
#define thousand	1000		//[mg/LSB]

uint8_t xRate_L;  // Variable to hold X_Low register value
uint8_t xRate_H;  // Variable to hold X_High register value
uint8_t yRate_L;  // Variable to hold Y_Low register value
uint8_t yRate_H;  // Variable to hold Y_High register value
uint8_t zRate_L;  // Variable to hold Z_Low register value
uint8_t zRate_H;  // Variable to hold Z_High register value
uint8_t xaccel_L;
uint8_t xaccel_H;
uint8_t yaccel_L;
uint8_t yaccel_H;
uint8_t zaccel_L;
uint8_t zaccel_H;

uint32_t gyrRawx;
uint32_t gyrRawy;
uint32_t gyrRawz;
float gyroXangle;
float gyroYangle;
float gyroZangle;

short accRawx;
short accRawy;
short accRawz;
float g_x;
float g_y;
float g_z;

float rate_gyr_x = 0;
float rate_gyr_y = 0;
float rate_gyr_z = 0;
uint8_t gyro_Address = 0x6A;				//Use 6A on berryIMU, 6B on Hub
uint8_t XMAddress = 0x1E;					//Use 1E on berryIMU, 1D on Hub
uint8_t outputRegister;		// TX output
uint8_t outString[50]; 					//uint8_tacter string to output to Putty

// main.c
int main(void) {
//	uint16_t byte_Count = 10;
	bool pin_Setting = 1;

	WDTCTL = WDTPW | WDTHOLD;               // Stop watchdog timer
	initialize_Clocks ();					// Sets up timers (takes care of FRAM issue)
	initialize_Ports();						// Init all non used ports

	init_I2C_Hub(gyro_Address, pin_Setting);	// init Hub, 100 bytes, SDA - P3.1, SCL - P3.2

	// uint8_t slave_Add, uint8_t reg_Add, uint8_t tx_Data_8, bool pin_Setting
	write_uint8_I2C(gyro_Address, CTRL_REG1_G, 0x0F, pin_Setting);		// sets up Gyro
	write_uint8_I2C(gyro_Address, CTRL_REG4_G, 0x30, pin_Setting);

	write_uint8_I2C(XMAddress, CTRL_REG1_XM, 0x67, pin_Setting);		// sets up XM
	write_uint8_I2C(XMAddress, CTRL_REG2_XM, 0x20, pin_Setting);
	write_uint8_I2C(XMAddress, CTRL_REG5_XM, 0xF0, pin_Setting);
	write_uint8_I2C(XMAddress, CTRL_REG6_XM, 0x60, pin_Setting);
	write_uint8_I2C(XMAddress, CTRL_REG7_XM, 0x00, pin_Setting);

	while (1) {
		///////////////READ GYRO//////////////////////
		UCB1I2CSA = gyro_Address;

		outputRegister = OUT_X_L_G;
		//uint8_t output_Reg, bool pin_Setting
		xRate_L = read_I2C(outputRegister, pin_Setting);

		outputRegister = OUT_X_H_G;
		xRate_H = read_I2C(outputRegister, pin_Setting);

		outputRegister = OUT_Y_L_G;
		yRate_L = read_I2C(outputRegister, pin_Setting);

		outputRegister = OUT_Y_H_G;
		yRate_H = read_I2C(outputRegister, pin_Setting);

		outputRegister = OUT_Z_L_G;
		zRate_L = read_I2C(outputRegister, pin_Setting);

		outputRegister = OUT_Z_H_G;
		zRate_H = read_I2C(outputRegister, pin_Setting);

		/////////////READ ACCELEROMETER/////////////////
		UCB1I2CSA = XMAddress;

		outputRegister = OUT_X_L_A;
		xaccel_L = read_I2C(outputRegister, pin_Setting);

		outputRegister = OUT_X_H_A;
		xaccel_H = read_I2C(outputRegister, pin_Setting);

		outputRegister = OUT_Y_L_A;
		yaccel_L = read_I2C(outputRegister, pin_Setting);

		outputRegister = OUT_Y_H_A;
		yaccel_H = read_I2C(outputRegister, pin_Setting);

		outputRegister = OUT_Z_L_A;
		zaccel_L = read_I2C(outputRegister, pin_Setting);

		outputRegister = OUT_Z_H_A;
		zaccel_H = read_I2C(outputRegister, pin_Setting);

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

		g_x =(accRawx * A_GAIN)/thousand;
		g_y =(accRawy * A_GAIN)/thousand;
		g_z =(accRawz * A_GAIN)/thousand;
	}
}
