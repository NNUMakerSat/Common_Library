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

void init_SPI (void);		// pin select?
void write_SPI (uint8_t);
uint8_t read_SPI (void);

#endif /* SPI_PULLING_H_ */
