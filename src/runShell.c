#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 199309L // for CLOCK_REALTIME to work
#endif

#include <time.h>

//#include <iostream>

#include "helperFunctions.h"
#include "cpu.h"
#include "ioPorts.h"


// Global variable for keeping time
struct timespec nowTime;
long lastInterrupt = 0;

int main(int argc, char** argv) {
	State8080* state = init8080CPU();
	StateSIMachine* machine = initSIMachine();
	readInvaderstoMem(state);
	
	
	printf("Press enter whenever you want to load the next instruction\n");

	while (1) {
		
		if(getchar()) {

			uint8_t* opCode = &state->memory[state->pc];

			if (*opCode == 0xdb) {	// machine IN
				machineIN(state, machine, opCode[1]);
				extractOpCode(state->memory, state->pc);
				state->pc++;
			}
			else if (*opCode == 0xd3) { // machine OUT
				machineOUT(state, machine, opCode[1]);
				extractOpCode(state->memory, state->pc);
				state->pc++;
			}
			else { // regular 8080 opcodes
				emulate8080(state);
				extractOpCode(state->memory, state->pc);
				printf("OP Current: %x\nPC: %x\nSP: %x\n", state->memory[state->pc], state->pc, state->sp);
				printf("Registers:  A: %x B: %x C: %x D: %x E: %x H: %x L: %x \n", state->a, state->b, state->c, state->d, state->e, state->h, state->l);
				printf("---------------------\n");
			}
		}

		// trigger interrupt if 1/60 of a second has passed since last interrupt, and interrupts are enabled
		clock_gettime(CLOCK_REALTIME, &nowTime);
		if (nowTime.tv_nsec - lastInterrupt > 1000000000. / 60. && state->int_enable) {
			generateInterrupt(state, 2);
			lastInterrupt = nowTime.tv_nsec;
		}

	}
	return 0;
}
