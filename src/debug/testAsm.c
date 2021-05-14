#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>


//#include <iostream>

#include "../cpu/opcodeFunctions.h"
#include "../cpu/cpu.h"
#include "../machine/ioPorts.h"
#include "debugFunc.h"


int beginTest(int argc, char** argv) {
	State8080* state = init8080CPU();
	StateSIMachine* machine = initSIMachine();
	loadTestAsm(state);

	// track whether the test has failed
	// based on printed message
	int succeed = 0;

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
				succeed = debugCALL(state);
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

			// if system jumps back to 00, that means result has been reached
			if (*opCode == 0xc3 && opCode[1] == 0x00 && opCode[2] == 0x00) {
				
				// if error was printed, exit with status 1, else break and return with status 0
				if (succeed != 1) {
					exit(EXIT_FAILURE);
				}
				else {
					break;
				}

			}
	}
	return 0;
}
