/**************************************************************************************************
** File Name: ioPorts.c
** Description: This file contains function declarations for handling the calls to the IO ports.
**************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ioPorts.h"
#include "../cpu/cpu.h"


/* Reads from the machine ports into the CPU A register*/
void machineIN(State8080* state, StateSIMachine* machine, uint8_t port) {
	uint8_t output = 0;
	switch (port) {
		case 3:		// read from shift registers based on shiftAmt
		{
			uint16_t shiftRegs = (machine->shiftHi << 8) | machine->shiftLo;
			output = (shiftRegs >> (8 - machine->shiftAmt)) & 0xFF;
			break;
		}
	}
	state->a = output;
}


/* Writes out from the CPU to the machine ports */
void machineOUT(State8080* state, StateSIMachine* machine, uint8_t port) {
	switch (port) {
		case 2:		// write to shift amount from bits 0-2 of A register
		{
			// only read bits 0, 1, 2 of A register
			machine->shiftAmt = state->a & 0x07;
			break;
		}
		case 4:		// write to shift registers from A register
		{
			machine->shiftLo = machine->shiftHi;
			machine->shiftHi = state->a;
			break;
		}
	}
}

StateSIMachine* initSIMachine() {
	StateSIMachine* machine = calloc(1, sizeof(StateSIMachine));
	return machine;
}
