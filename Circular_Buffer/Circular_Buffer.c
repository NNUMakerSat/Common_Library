/*
 * Circular_Buffer.c
 *
 *  Created on: Aug 10, 2016
 *      Author: Dr. Griffin
 */
#include <msp430fr6989.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include "Circular_Buffer.h"

#define MAXBUFFERSIZE 5

uint16_t g_buffer[MAXBUFFERSIZE];

int16_t g_begin = 0;
int16_t g_end = MAXBUFFERSIZE-1;
int16_t g_top;
int16_t g_bottom;
bool g_bufferEmpty;
bool g_bufferFull;

void initBuffer(void){
    g_top = 0;
    g_bottom = 0;
    g_bufferFull = false;
    g_bufferEmpty = false;
}

void writeBuffer(uint8_t data) {
    g_buffer[g_bottom] = data;
    g_bottom++;
    if(g_bottom > g_end) {
        g_bottom = g_begin;
    }
    if(g_top == g_bottom){
        g_bufferFull = true;
    } else {
        g_bufferFull = false;
    }
}

uint8_t readBuffer(void) {
    uint8_t readVal = g_buffer[g_top];
    g_top++;
    if(g_top > g_end) {
        g_top = g_begin;
    }
    if(g_top == g_bottom){
        g_bufferEmpty = true;
    } else {
        g_bufferEmpty = false;
    }
    return readVal;
}

bool isBufferFull(void) {
    return g_bufferFull;
}

bool isBufferEmpty(void) {
    return g_bufferEmpty;
}
