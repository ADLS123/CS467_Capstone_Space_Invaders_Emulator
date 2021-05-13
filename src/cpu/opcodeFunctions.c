/**************************************************************************************************
** File Name: opcodeFunctions.c
** Description: This file contains the function definitions to encapsulate the work related to 
		the various flag setting required for the Space Invaders ROM
**************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "opcodeFunctions.h"
#include "cpu.h"

#pragma warning (disable: 4996)

/****************************************************************************************
** Function Name: int parity(int value)
** Parameters: an integer to check the parity of
** Returns: an integer, will return 1 for even parity, returns 0 for odd parity
** Description: Helper function to check and set the parity flag for the emulator. 
		Parity function matches the parity bit setting for the 8080 processor, where 
		even parity is set to 1 and odd parity is set to 0.


** NOTE: Parity function heavily inspired from the code found at:
	URL: https://www.tutorialspoint.com/cplusplus-program-to-find-the-parity-of-a-number-efficiently
****************************************************************************************/
int parity(int value){
	int y;
	y = value ^ (value >> 1);		//sets y to an XOR of value and value right-shifted with 1
	y = y ^ (y >> 2);				//sets y to an XOR of y right-shifted with 2
	y = y ^ (y >> 4);				//repeats the above line with 4, 8, 16
	y = y ^ (y >> 8);
	y = y ^ (y >> 16);
	//checks the rightmost bit
	if (y & 1) {
		return 0;					//returns 0 for odd parity
	}
	return 1;						//returns 1 for even parity
}


/*
* NOTE: The below functions are inspired from the logic from URL: http://emulator101.com/
*/


int logicSetSign(int value) {
	return (0x80 == (value & 0x80));
}
/**************************************************************************************************
** Function Name: int setCarry
** Parameters: an integer to check for setting the carry flag
** Returns: Either a 1 or 0 depending on the truth value of the statement
** Description: This function is used to set the carry flag for the instructions for the emulator
**************************************************************************************************/
int setCarry(int value){
	return (value > 0xff);
}


/**************************************************************************************************
** Function Name: int setDoubleCarry(int value)
** Parameters: an integer value
** Returns: Either zero or one, one if the statement is true, zero if false
** Description: This function is to be used for double instructions to set the carry flag for
		the emulator. This is used for the double instructions
**************************************************************************************************/
int setDoubleCarry(int value) {
	return ((value & 0xffff0000) != 0);
}


//function for setting the zero flag based off of a parameter value
int zero(int value) {
	return (value == 0);
}


/* Arithmetic Group helper functions*/

// Helper function for INX opcode, increments 2 register integer 
void inxFunc(uint8_t* hi, uint8_t* lo) {
	uint16_t temp = ((uint16_t)*hi << 8) | *lo;
	temp++;
	*hi = (temp >> 8) & 0xFF;
	*lo = temp & 0xFF;
}


// Helper function for DCXopcode, decrements 2 register integer
void dcxFunc(uint8_t* hi, uint8_t* lo) {
	uint16_t temp = ((uint16_t)*hi << 8) | *lo;
	temp--;
	*hi = (temp >> 8) & 0xFF;
	*lo = temp & 0xFF;
}


/* Helper function for CALL instructions */
void callFunc(State8080* state, uint16_t callAddr) {
	// set return address based on how long the instruction is
	uint16_t pcIncr = 0;
	if (callAddr == UINT16_MAX)
		pcIncr = 2;
	// push return address on stack (actually return address - 1, bc of pc increment after switch cases)
	uint16_t retAddr = state->pc + pcIncr;
	state->memory[state->sp - 1] = (retAddr >> 8) & 0xff;
	state->memory[state->sp - 2] = retAddr & 0xff;

	// if no address is specified, UINT16_MAX used for no specific address
	if (callAddr == UINT16_MAX) {
		unsigned char* opCode = &state->memory[state->pc];
		state->pc = (opCode[2] << 8 | opCode[1])-1;
	
	}
	// if address is specified
	else {
		state->pc = callAddr-1;
	}
}


/* Helper function for RET instructions */
void retFunc(State8080* state) {
	uint16_t retAddr = (state->memory[state->sp + 1] << 8) | state->memory[state->sp];
	state->pc = retAddr;
	state->sp += 2;
}


/* Helper function for POP instructions */
void popFunc(State8080* state, uint8_t* hi, uint8_t* lo) {
	// lo is lower order bits, hi is higher order bits
	*lo = state->memory[state->sp];
	*hi = state->memory[state->sp + 1];
	state->sp += 2;
}


/* Helper function for Push instructions*/
void pushFunc(State8080* state, uint8_t* hi, uint8_t* lo) {
	state->memory[state->sp - 1] = *hi;
	state->memory[state->sp - 2] = *lo;
	state->sp -= 2;
}


/*
* NOTE: Function for setting the state comes from https://github.com/kpmiller/emulator101/blob/master/8080emu-first50.c
*/

void setLogicFlagsA(State8080* state) {
	state->cc.cy = 0;
	state->cc.ac = 0;
	state->cc.z = zero(state->a);
	state->cc.s = logicSetSign(state->a);
	state->cc.p = parity(state->a);
}


/* Reads one ROM file into memory at a specified offset */
void readFiletoMem(State8080* state, char* fileName, uint32_t offset) {
	FILE* fp = fopen(fileName, "rb");
	if (fp == NULL) {
		fprintf(stderr, "ERROR: could not open file %s\n", fileName);
		exit(1);
	}

	// open ROM file and read contents to memory, then close file
	fseek(fp, 0L, SEEK_END);
	size_t fsize = ftell(fp);
	fseek(fp, 0L, SEEK_SET);

	uint8_t* memBuffer = &state->memory[offset];
	fread(memBuffer, fsize, 1, fp);
	fclose(fp);
}


/* Reads the 4 space invaders ROM files into memory */
void readInvaderstoMem(State8080* state) {
	readFiletoMem(state, "./roms/invaders.h", 0x0000);
	readFiletoMem(state, "./roms/invaders.g", 0x0800);
	readFiletoMem(state, "./roms/invaders.f", 0x1000);
	readFiletoMem(state, "./roms/invaders.e", 0x1800);
}

/* Initializes cpu state struct */
State8080* init8080CPU() {
	State8080* state = calloc(1, sizeof(State8080));
	state->memory = calloc(1, 0x10000); //16000 bytes allocated for memory
	return state;
}


/* Generates an interrupt, basically same as calling a RST opcode */
void generateInterrupt(State8080* state, int interruptNum) {
	// call address is 8 * interrupt number
	uint16_t callAddr = interruptNum * 0x08;
	callFunc(state, callAddr);
	state->pc++;
}




/*
//Testing main for whatever helper function is needed
int main() {
	int n = 23;
	int result = parity(n);
	if (result == 1) {
		printf("My parity: Even parity\n");
	}
	else {
		printf("My Parity: Odd parity\n");
	}
	
	//testing zero flag
	int math = 8 - 7;
	if (zero(math & 0xff)) {
		printf("Zero flag activated, value: %d\n", zero(math));
	}
	else { printf("not zero"); }
	return 0;
}
*/
