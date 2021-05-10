/**************************************************************************************************
** File Name: ioPorts.c
** Description: This file contains function declarations for handling the calls to the IO ports.
**************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ioPorts.h"
#include "cpu.h"


/* Reads from the machine ports into the CPU */
uint8_t machineIN(state8080* state, StateSIMachine* machine, uint8_t port) {
	uint8_t output = 0;
	switch (port) {
		case 3:		// read from shift registers based on shiftAmt
		{
			uint16_t shiftRegs = (state->shiftHi << 8) | state->shiftLo;
			output = (shiftRegs >> (8 - machine->shiftAmt)) & 0xFF;
			break;
		}
	}
	return output;
}


/* Writes out from the CPU to the machine ports */
void machineOUT(state8080* state, StateSIMachine* machine, uint8_t port) {
	switch (port) {
		case 2:		// write to shift amount from bits 0-2 of A register
		{
			// only read bits 0, 1, 2 of A register
			machine->shiftAmt = state->a & 0x07;
			break;
		}
		case 4:		// write to shift registers from A register
		{
			machine->shiftLo = shiftHi;
			machine->shiftHi = state->a;
			break;
		}
	}
}