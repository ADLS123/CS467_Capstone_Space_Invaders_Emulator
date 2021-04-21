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


/* Helper function for CALL instructions */
void callFunc(State8080* state, uint16_t callAddr) {
	// set return address based on how long the instruction is
	uint16_t pcIncr = 0;
	if (callAddr = UINT16_MAX)
		pcIncr = 2
	// push return address on stack (actually return address - 1, bc of pc increment after switch cases)
	uint16_t retAddr = state->pc + pcIncr;
	state->memory[sp - 1] = (retAddr >> 8) & 0xff;
	state->memory[sp - 2] = retAddr & 0xff;

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

		/* CALL and RET instructions */
		case 0xc4: // CNZ
			if (state->cc.z == 0)
				callFunc(state, UINT16_MAX);
			else
				state->pc += 2;
			break;

		case 0xc7: // RST 0
			callFunc(state, 0x00);
			break;

		case 0xcc: // CZ
			if (state->cc.z == 1)
				callFunc(state, UINT16_MAX);
			else
				state->pc += 2;
			break;

		case 0xcd: // CALL
			callFunc(state, UINT16_MAX);
			break;

		case 0xcf: // RST 1
			callFunc(state, 0x08);
			break;

		case 0xd4: // CNC
			if (state->cc.cy == 0)
				callFunc(state, UINT16_MAX);
			else
				state->pc += 2;
			break;

		case 0xd7: // RST 2
			callFunc(state, 0x10);
			break;

		case 0xdc: // CC
			if (state->cc.cy == 1)
				callFunc(state, UINT16_MAX);
			else
				state->pc += 2;
			break;

		case 0xdf: // RST 3
			callFunc(state, 0x18);
			break;

		case 0xe4: // CPO
			if (state->cc.p == 0)
				callFunc(state, UINT16_MAX);
			else
				state->pc += 2;
			break;

		case 0xe7: // RST 4
			callFunc(state, 0x20);
			break;

		case 0xec: // CPE
			if (state->cc.p == 1)
				callFunc(state, UINT16_MAX);
			else
				state->pc += 2;
			break;

		case 0xef: // RST 5
			callFunc(state, 0x28);
			break;

		case 0xf4: // CP
			if (state->cc.s == 0)
				callFunc(state, UINT16_MAX);
			else
				state->pc += 2;
			break;

		case 0xf7: // RST 6
			callFunc(state, 0x30);
			break;

		case 0xfc: // CM
			if (state->cc.s == 1)
				callFunc(state, UINT16_MAX);
			else
				state->pc += 2;
			break;

		case 0xff: // RST 7
			callFunc(state, 0x38);
			break;

		/* RET instructions*/
		case 0xc0: // RNZ
			if (state->cc.z == 0)
				retFunc(state);
			break;
			
		case 0xc8: // RZ
			if (state->cc.z == 1)
				retFunc(state);
			break;

		case 0xc9: // RET
			retFunc(state);
			break;

		case 0xd0: // RNC
			if (state->cc.cy == 0)
				retFunc(state);
			break;

		case 0xd8: // RC
			if (state->cc.cy == 1)
				retFunc(state);
			break;

		case 0xe0: // RPO
			if (state->cc.p == 0)
				retFunc(state);
			break;

		case 0xe8: // RPE 
			if (state->cc.p == 1)
				retFunc(state);
			break;

		case 0xf0: // RP
			if (state->cc.s == 0)
				retFunc(state);
			break;

		case 0xf8: // RM
			if (state->cc.s == 1)
				retFunc(state);
			break;
	}

	state->pc += 1;
}

int main(int argc, char** argv) {
	return 0;
}