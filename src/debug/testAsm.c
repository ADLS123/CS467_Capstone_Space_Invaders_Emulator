#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


//#include <iostream>

#include "../cpu/opcodeFunctions.h"
#include "../cpu/cpu.h"
#include "../machine/ioPorts.h"
#include "debugFunc.h"


int main(int argc, char** argv) {
	State8080* state = init8080CPU();
	StateSIMachine* machine = initSIMachine();
	loadTestAsm(state);

	while (1) {

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
			else if (*opCode == 0xcd) {
				debugCALL(state);
				extractOpCode(state->memory, state->pc);
				printf("OP Current: %x\nPC: %x\nSP: %x\n", state->memory[state->pc], state->pc, state->sp);
				printf("Registers:  A: %x B: %x C: %x D: %x E: %x H: %x L: %x \n", state->a, state->b, state->c, state->d, state->e, state->h, state->l);
				printf("---------------------\n");
			}
			else { // regular 8080 opcodes
				emulate8080(state);
				extractOpCode(state->memory, state->pc);
				printf("OP Current: %x\nPC: %x\nSP: %x\n", state->memory[state->pc], state->pc, state->sp);
				printf("Registers:  A: %x B: %x C: %x D: %x E: %x H: %x L: %x \n", state->a, state->b, state->c, state->d, state->e, state->h, state->l);
				printf("---------------------\n");
			}


		// trigger interrupt if 1/60 of a second has passed since last interrupt, and interrupts are enabled
		//clock_gettime(CLOCK_REALTIME, &nowTime);
		//nowmsec = nowTime.tv_sec * 1000 + nowTime.tv_nsec / 1000000;
		//if ((float)(nowmsec - lastInterrupt) > 16.67 && state->int_enable) {
		//	generateInterrupt(state, 2);
		//	lastInterrupt = nowTime.tv_nsec;
		//}
	}
	return 0;
}
