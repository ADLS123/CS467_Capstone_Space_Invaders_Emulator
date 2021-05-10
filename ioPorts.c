/**************************************************************************************************
** File Name: ioPorts.c
** Description: This file contains function declarations for handling the calls to the IO ports.
**************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ioPorts.h"

/* Reads from the machine port into the CPU */
uint8_t machineIN(state8080* state, uint8_t port) {
	uint8_t output = 0;
	switch (port) {
		case 3:		// read from shift registers based on shiftAmt
		{
			uint16_t shiftRegs = (state->shiftHi << 8) | state->shiftLo;
			output = (shiftRegs >> (8 - state->shiftAmt)) & 0xFF;
			break;
		}
	}
	return output;
}


uint