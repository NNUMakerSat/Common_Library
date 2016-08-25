/*
 * I2C_Polling.h
 *
 *  Created on: Aug 24, 2016
 *      Author: aaronewing
 */

#ifndef I2C_POLLING_H_
#define I2C_POLLING_H_

void init_I2C_Hub (uint8_t slave_Address, uint16_t byte_Count, bool pin_Setting);			// initalizes I2C clk rate and which pins are being used
void init_I2C_SB (bool pin_Setting);								// initalizes I2C clk rate and which pins are being used
void write_uint8_I2C (uint8_t tx_Data_8, uint8_t device_CS);		// writes 8 bits with I2C
void write_uint16_I2C (uint16_t tx_Data_16, uint8_t device_CS);		// writes 16 bits with I2C
void write_uint32_I2C (uint32_t tx_Data_32, uint32_t device_CS);	// writes 16 bits with I2C
uint8_t read_I2C (void);											// reads 8 bits with I2C

#endif /* I2C_POLLING_H_ */
