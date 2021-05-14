#include <stdio.h>
#include <stdlib.h>

#include "../cpu/cpu.h"
#include "../cpu/opcodeFunctions.h"


/* Loads diagnostic 8080 assembly code for our program to run
Taken from emulator101.com */
void loadTestAsm(State8080* state) {
	readFiletoMem(state, "./src/debug/cpudiag.bin", 0x100);
	state->memory[0] = 0xc3; // JMP to 0x100, since code assumes code starts at 0x100
	state->memory[1] = 0;
	state->memory[2] = 0x01;

	// Fix the stack pointer from 0x6ad to 0x7ad    
	// this 0x06 byte 112 in the code, which is    
	// byte 112 + 0x100 = 368 in memory    
	state->memory[368] = 0x7;

	//Skip DAA test    
	state->memory[0x59c] = 0xc3; // JMP
	state->memory[0x59d] = 0xc2;
	state->memory[0x59e] = 0x05;
}


// Modified CALL function to work with diagnostic code taken from emulator101.com
void debugCALL(State8080* state) {
	unsigned char* opCode = &state->memory[state->pc];
	// likely that the original OS has code at address $0005 that prints messages to console
	if (5 == ((opCode[2] << 8) | opCode[1])) {
		if (state->c == 9) {
			uint16_t offset = (state->d << 8) | (state->e);
			// skip prefix bytes
			char* str = &state->memory[offset + 3];  

			while (*str != '$') {
				printf("%c", *str++);
			}
			printf("\n");
		}
		else if (state->c == 2) {
			// this is in the code, but is never called
			printf("print char routine called\n");
		}
		else if (state->c == 0) {
			return 0;
		}
	}
	else {
		callFunc(state, ((opCode[2] << 8) | opCode[1]));
		state->pc++;
	}
}