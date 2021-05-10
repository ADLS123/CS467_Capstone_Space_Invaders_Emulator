/**************************************************************************************************
** File Name: ioPorts.h
** Description: This file contains the definition of the space invaders machine state, and 
function prototypes for handling the calls to the machine IO ports.
**************************************************************************************************/

#include "cpu.h"

#ifndef IOPORTS_H
#define IOPORTS_H


typdef struct StateSIMachine {
	uint8_t shiftLo;	// lower order bits shift register
	uint8_t shiftHi;	// higher order bits shift register
	uint8_t shiftAmt;	// changed by OUT 2 opcode
} StateSIMachine;