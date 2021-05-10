/**************************************************************************************************
** File Name: ioPorts.h
** Description: This file contains the definition of the space invaders machine state, and 
function prototypes for handling the calls to the machine IO ports.
**************************************************************************************************/

#include <stdlib.h>
#include <stdint.h>
#include "../cpu/cpu.h"

#ifndef IOPORTS_H
#define IOPORTS_H

typedef struct StateSIMachine {
	uint8_t shiftLo;	// lower order bits shift register
	uint8_t shiftHi;	// higher order bits shift register
	uint8_t shiftAmt;	// changed by OUT 2 opcode
} StateSIMachine;


// function prototypes
void machineIN(State8080* state, StateSIMachine* machine, uint8_t port);
void machineOUT(State8080* state, StateSIMachine* machine, uint8_t port);
StateSIMachine* initSIMachine(void);

#endif
