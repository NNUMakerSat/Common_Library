/*
 * LSM9DS0.c
 *
 *  Created on: Sep 24, 2016
 *      Author: bgrim
 */
#include <msp430fr6989.h>
#include <stdint.h>
#include "LSM9DS0.h"
#include "i2c.h"


#define G_GAIN	0.07		//[deg/s/LSB]
#define A_GAIN	0.732		//[mg/LSB]
#define M_GAIN  0.48		//[mgauss/LSB]
#define thousand	1000

char xRate_L = 0;  // Variable to hold X_Low register value
char xRate_H = 0;  // Variable to hold X_High register value
char yRate_L = 0;  // Variable to hold Y_Low register value = 0
char yRate_H = 0;  // Variable to hold Y_High register value
char zRate_L = 0;  // Variable to hold Z_Low register value
char zRate_H = 0;  // Variable to hold Z_High register value
char xaccel_L = 0;
char xaccel_H = 0;
char yaccel_L = 0;
char yaccel_H = 0;
char zaccel_L = 0;
char zaccel_H = 0;
char xmag_L = 0;
char xmag_H = 0;
char ymag_L = 0;
char ymag_H = 0;
char zmag_L = 0;
char zmag_H = 0;

float g_x_array[10];
float g_y_array[10];
float g_z_array[10];
float rate_gyr_x_array[10];
float rate_gyr_y_array[10];
float rate_gyr_z_array[10];
float magnet_x_array[10];
float magnet_y_array[10];
float magnet_z_array[10];
uint8_t i = 0;

int32_t gyrRawx = 0;
int32_t gyrRawy = 0;
int32_t gyrRawz = 0;
float gyroXangle = 0;
float gyroYangle = 0;
float gyroZangle = 0;
float mag_x_raw = 0;
float mag_y_raw = 0;
float mag_z_raw = 0;

int32_t accRawx = 0;
int32_t accRawy = 0;
int32_t accRawz = 0;
float g_x = 0.0;
float g_y = 0.0;
float g_z = 0.0;
float magnet_x = 0.0;
float magnet_y = 0.0;
float magnet_z = 0.0;

float rate_gyr_x = 0.0;
float rate_gyr_y = 0.0;
float rate_gyr_z = 0.0;

void initGyro(void) {
	single_byte_write_i2c(GAddress, CTRL_REG1_G, 0x0F);
	single_byte_write_i2c(GAddress, CTRL_REG4_G, 0x30);
}

void initXM(void) {
	single_byte_write_i2c(XMAddress, CTRL_REG1_XM, 0x67);
	single_byte_write_i2c(XMAddress, CTRL_REG2_XM, 0x20);
	single_byte_write_i2c(XMAddress, CTRL_REG5_XM, 0xF0);
	single_byte_write_i2c(XMAddress, CTRL_REG6_XM, 0x60);
	single_byte_write_i2c(XMAddress, CTRL_REG7_XM, 0x00);
}

void run_IMU(void) {
////////////READ GYROSCOPE//////////////
	for (i = 0; i < 10; i++) {
		xRate_L = single_byte_read_i2c(GAddress, OUT_X_L_G);
		xRate_H = single_byte_read_i2c(GAddress, OUT_X_H_G);
		yRate_L = single_byte_read_i2c(GAddress, OUT_Y_L_G);
		yRate_H = single_byte_read_i2c(GAddress, OUT_Y_H_G);
		zRate_L = single_byte_read_i2c(GAddress, OUT_Z_L_G);
		zRate_H = single_byte_read_i2c(GAddress, OUT_Z_H_G);

		/////////////READ ACCELEROMETER/////////////////
		xaccel_L = single_byte_read_i2c(XMAddress, OUT_X_L_A);
		xaccel_H = single_byte_read_i2c(XMAddress, OUT_X_H_A);
		yaccel_L = single_byte_read_i2c(XMAddress, OUT_Y_L_A);
		yaccel_H = single_byte_read_i2c(XMAddress, OUT_Y_H_A);
		zaccel_L = single_byte_read_i2c(XMAddress, OUT_Z_L_A);
		zaccel_H = single_byte_read_i2c(XMAddress, OUT_Z_H_A);

		////////////READ MAGNETOMETER////////////////////////
		xmag_L = single_byte_read_i2c(XMAddress, OUT_X_L_M);
		xmag_H = single_byte_read_i2c(XMAddress, OUT_X_H_M);
		ymag_L = single_byte_read_i2c(XMAddress, OUT_Y_L_M);
		ymag_H = single_byte_read_i2c(XMAddress, OUT_Y_H_M);
		zmag_L = single_byte_read_i2c(XMAddress, OUT_Z_L_M);
		zmag_H = single_byte_read_i2c(XMAddress, OUT_Z_H_M);

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

		g_x = (accRawx * A_GAIN) / thousand;
		g_y = (accRawy * A_GAIN) / thousand;
		g_z = (accRawz * A_GAIN) / thousand;

		mag_x_raw = (xmag_L | xmag_H << 8);
		mag_y_raw = (ymag_L | ymag_H << 8);
		mag_z_raw = (zmag_L | zmag_H << 8);

		magnet_x = (mag_x_raw * M_GAIN) / thousand;
		magnet_y = (mag_y_raw * M_GAIN) / thousand;
		magnet_z = (mag_z_raw * M_GAIN) / thousand;

		g_x_array[i] = g_x;
		g_y_array[i] = g_y;
		g_z_array[i] = g_z;
		rate_gyr_x_array[i] = rate_gyr_x;
		rate_gyr_y_array[i] = rate_gyr_y;
		rate_gyr_z_array[i] = rate_gyr_z;
		magnet_x_array[i] = magnet_x;
		magnet_y_array[i] = magnet_y;
		magnet_z_array[i] = magnet_z;

		g_x = 0;
		g_y = 0;
		g_z = 0;
		rate_gyr_x = 0;
		rate_gyr_y = 0;
		rate_gyr_z = 0;
		magnet_x = 0;
		magnet_y = 0;
		magnet_z = 0;
	}

}
