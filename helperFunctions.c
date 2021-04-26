/**************************************************************************************************
** File Name: helperFunctions.c
** Author(s): Devin Swanson
** Description: This file contains the function definitions to encapsulate the work related to 
		the various flag setting required for the Space Invaders ROM
**************************************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "helperFunctions.h"



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


/* Helper function for CALL instructions */
void callFunc(State8080* state, uint16_t callAddr) {
	// set return address based on how long the instruction is
	uint16_t pcIncr = 0;
	if (callAddr = UINT16_MAX)
		pcIncr = 2;
	// push return address on stack (actually return address - 1, bc of pc increment after switch cases)
	uint16_t retAddr = state->pc + pcIncr;
	state->memory[state->sp - 1] = (retAddr >> 8) & 0xff;
	state->memory[state->sp - 2] = retAddr & 0xff;

	// if no address is specified, UINT16_MAX used for no specific address
	if (callAddr == UINT16_MAX) {
		unsigned char* opCode = &state->memory[state->pc];
		state->pc = (opCode[2] << 8 | opCode[1]);
	}
	// if address is specified
	else {
		state->pc = callAddr;
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
	lo = state->memory[state->sp];
	hi = state->memory[state->sp + 1];
	state->sp += 2;
}


/* Helper function for Push instructions*/
void pushFunc(State8080* state, uint8_t* hi, uint8_t* lo) {
	state->memory[state->sp - 1] = hi;
	state->memory[state->sp - 2] = lo;
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