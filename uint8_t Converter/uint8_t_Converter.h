/*
 * uint8_t_Converter.h
 *
 *  Created on: Aug 11, 2016
 *      Author: aaronewing
 */
#include <msp430fr6989.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#ifndef UINT8_T_CONVERTER_H_
#define UINT8_T_CONVERTER_H_

uint8_t 16_To_8(uint16_t data, uint8_t array_Name);
uint8_t 32_To_8(uint32_t data, uint8_t array_Name);

#endif /* UINT8_T_CONVERTER_H_ */
