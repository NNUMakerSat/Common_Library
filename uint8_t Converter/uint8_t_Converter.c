/*
 * uint8_t_Converter.c
 *
 *  Created on: Aug 11, 2016
 *      Author: aaronewing
 */
#include <msp430fr6989.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

uint8_t 16_To_8(uint16_t data, uint8_t array_Name) {
	uint8_t array_Name[2];							// creates array with 2 elements (each being a single byte) customly named
	array_Name[0] = data/256;						// dec value of first byte placed in first element
	array_Name[1] = data-(array_Name[0]*256);		// dec value of second byte placed in second element

	return array_Name;								// returns array containing all the information given to the function
}

uint8_t 32_To_8(uint32_t data, uint8_t array_Name) {
	uint8_t array_Name[4];										// creates array with 4 elements (each being a single byte) customly named
	uint32_t new_Data = data << 16;								// shifts bits to left 16 times ie xxxxxxxx xxxxxxxx 00000000 00000000
	//7022592 = data-(16777216*array_Name[0])
	//x = 7022592 -(([7022592/65536]dec)*65536)
	// y = x/256
	array_Name[0] = data/16777216;								// uint value of first byte placed in first element <works><238.xxx>
	array_Name[1] = (data-(array_Name[0]*16777216))/65536;		// uint value of second byte placed in second element <works>

	array_Name[2] = new_Data/16777216;
	array_Name[3] = (new_Data-(array_Name[2]*16777216))/65536;	// uint value of second byte placed in second element <works>
/*
	uint32_t var1 = data - (16777216*array_Name[0]);		// holds 7022592
	uint8_t var2 = var1/65536;								// holds int 107.xxx
	array_Name[2] = (var1-((var2)*65536))/256				// dec value of third byte placed in third element <works>
	array_Name[2] = (array_-(array_Name[0]*65536))/65536;		// dec value of second byte placed in second element <works>
*/
	return array_Name;								// returns array containing all the information given to the function
}

