#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/* Simulates condition codes/flags of 8080 processor */
typedef struct ConditionCodes {
	uint8_t	z : 1;
	uint8_t s : 1;
	uint8_t	p : 1;
	uint8_t cy : 1;
	uint8_t ac : 1;
	uint8_t pad : 3;

} ConditionCodes;

/* Simulates registers, stack pointer, program counter, etc (overall state)  of 8080 processor */
typedef struct State8080 {
	uint8_t a;
	uint8_t b;
	uint8_t c;
	uint8_t d;
	uint8_t e;
	uint8_t h;
	uint8_t l;

	uint16_t sp;
	uint16_t pc;
	uint8_t* memory;
	struct ConditionCodes cc;
	uint8_t int_enable;

} State8080;

/* Prints error message if unexpected instruction is encountered */
void unimplementedInstruction(State8080* state) {
	fprintf(stderr, "ERROR: Unimplemented instruction at $%x\n", &state->memory[state->pc]);
	exit(1);
}

/* Contains code for emulating opcodes */
void emulate8080(State8080* state) {
	unsigned char* opCode = &state->memory[state->pc];

	switch (*opCode) {
	
		/* JUMP instructions */
		case 0xc2: // JNZ
			if (state->cc.z == 0)
				state->pc = (opCode[2] << 8 | opCode[1]);
			else
				state->pc += 2;
			break;

		case 0xc3: // JMP
			state->pc = (opCode[2] << 8 | opCode[1]);
			break;

		case 0xca: // JZ
			if (state->cc.z == 1)
				state->pc = (opCode[2] << 8 | opCode[1]);
			else 
				state->pc += 2;
			break;

		case 0xd2: // JNC
			if (state->cc.cy == 0)
				state->pc = (opCode[2] << 8 | opCode[1]);
			else
				state->pc += 2;
			break;

		case 0xda: // JC
			if (state->cc.cy == 1)
				state->pc = (opCode[2] << 8 | opCode[1]);
			else
				state->pc += 2;
			break;

		case 0xe2: // JPO
			if (state->cc.p == 0)
				state->pc = (opCode[2] << 8 | opCode[1]);
			else
				state->pc += 2;
			break;

		case 0xea: // JPE
			if (state->cc.p == 1)
				state->pc = (opCode[2] << 8 | opCode[1]);
			else
				state->pc += 2;
			break;

		case 0xf2: // JP (plus sign)
			if (state->cc.s == 0)
				state->pc = (opCode[2] << 8 | opCode[1]);
			else
				state->pc += 2;
			break;

		case 0xfa: // JM (minus sign)
			if (state->cc.s == 1)
				state->pc = (opCode[2] << 8 | opCode[1]);
			else
				state->pc += 2;
			break;


	}

	state->pc += 1;
}

int main(int argc, char** argv) {
	return 0;
}