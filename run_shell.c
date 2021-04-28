#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
//#include <iostream>

#include "helperFunctions.h"
#include "cpu.h"

int main(int argc, char** argv) {
	State8080* state = init8080CPU();
	readInvaderstoMem(state);
	
	
	printf("Press enter whenever you want to load the next instruction\n");
	while (1) {
		
		if(getchar());
		{emulate8080(state);
		extractOpCode(state->memory, state->pc);
		printf("OP Current: %x\nPC: %x\nSP: %x\n",state->memory[state->pc],state->pc, state->sp);
		printf("Registers:  A: %x B: %x C: %x D: %x E: %x H: %x L: %x \n",state->a,state->b,state->c,state->d,state->e,state->h,state->l);
		printf("---------------------\n");
		}
	}

	return 0;
}