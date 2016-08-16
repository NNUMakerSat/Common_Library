/*
 * SPI_Pulling.h
 *
 *  Created on: Aug 16, 2016
 *      Author: aaronewing
 */
#include <msp430.h>
#include <stdint.h>

#ifndef SPI_PULLING_H_
#define SPI_PULLING_H_

void init_SPI (uint8_t clk_Rate, uint8_t pin_Setting);		// pin select.
void write_uint8_SPI (uint8_t tx_Data_8);
void write_uint16_SPI (uint16_t tx_Data_16);
uint8_t read_SPI (void);

#endif /* SPI_PULLING_H_ */
