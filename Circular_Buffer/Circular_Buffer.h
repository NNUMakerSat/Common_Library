/*
 * Circular_Buffer.h
 *
 *  Created on: Aug 10, 2016
 *      Author: Dr. Griffin
 */
#include <msp430fr6989.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef CIRCULAR_BUFFER_H_
#define CIRCULAR_BUFFER_H_

void initBuffer(void);
void writeBuffer(uint8_t data);
uint8_t readBuffer(void);
bool isBufferFull(void);
bool isBufferEmpty(void);

#endif /* CIRCULAR_BUFFER_H_ */
