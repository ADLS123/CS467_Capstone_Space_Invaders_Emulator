/**************************************************************************************************
** File Name: ioPorts.h
** Description: This file contains the definition of the space invaders machine state, and 
function prototypes for handling the calls to the machine IO ports.
**************************************************************************************************/


#include "../cpu/8080State.h"
#ifndef IOPORTS_H
#define IOPORTS_H

typedef struct StateSIMachine {
    //input Ports
    uint8_t input0 = 0x70;
    uint8_t input1 = 0x10;
    uint8_t input2 = 0;
    uint8_t input3 = 0;

    //output Ports
    uint8_t output2 = 0;
    uint8_t output3 = 0;
    uint8_t output4 = 0;
    uint8_t output5 = 0;
    uint8_t output6 = 0;

	uint8_t shiftLo;	// lower order bits shift register
	uint8_t shiftHi;	// higher order bits shift register
	uint8_t shiftAmt;	// changed by OUT 2 opcode
} StateSIMachine;


// function prototypes
StateSIMachine* initSIMachine(void);

#endif
