#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "helperFunctions.h"

/* Prototypes */
typedef struct ConditionCodes;
typedef struct State8080;
void callFunc(State8080*, uint16_t);
void retFunc(State8080*);
void setLogicFlagsA(State8080* state);
void unimplementedInstruction(State8080*);
void emulate8080(State8080*);


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
/* Prints error message if unexpected instruction is encountered */
void unimplementedInstruction(State8080* state) {
	fprintf(stderr, "ERROR: Unimplemented instruction at $%x\n", &state->memory[state->pc]);
	exit(1);
}

/* Contains code for emulating opcodes */
void emulate8080(State8080* state) {
	unsigned char* opCode = &state->memory[state->pc];

	switch (*opCode) {

		/* ANA instructions */
		case 0xa0: // ANA B
			state->a = state->a & state->b;
			setLogicFlagsA(state);
			break;
		case 0xa1: // ANA C
			state->a = state->a & state->c;
			setLogicFlagsA(state);
			break;
		case 0xa2: // ANA D
			state->a = state->a & state->d;
			setLogicFlagsA(state);
			break;
		case 0xa3: // ANA E 
			state->a = state->a & state->e;
			setLogicFlagsA(state);
			break;
		case 0xa4: // ANA H
			state->a = state->a & state->h;
			setLogicFlagsA(state);
			break;
		case 0xa5: // ANA L
			state->a = state->a & state->l;
			setLogicFlagsA(state);
			break;
		case 0xa6: // ANA M
			{	//how to handle the hl register came from URL: https://github.com/kpmiller/emulator101/blob/master/8080emu-first50.c
				uint32_t hl = (state->h << 8) | state->l;
				state->a = state->a & hl;
				setLogicFlagsA(state);
			}
			break;
		case 0xa7: // ANA A
			state->a = state->a & state->a;
			setLogicFlagsA(state);
			break;
		/* XRA instructions */
		case 0xa8: // XRA B
			state->a = state->a ^ state->b;
			setLogicFlagsA(state);
			break;
		case 0xa9: // XRA C
			state->a = state->a ^ state->c;
			setLogicFlagsA(state);
			break;
		case 0xaa: // XRA D
			state->a = state->a ^ state->d;
			setLogicFlagsA(state);
			break;
		case 0xab: // XRA E
			state->a = state->a ^ state->e;
			setLogicFlagsA(state);
			break;
		case 0xac: // XRA H
			state->a = state->a ^ state->h;
			setLogicFlagsA(state);
			break;
		case 0xad: // XRA L
			state->a = state->a ^ state->l;
			setLogicFlagsA(state);
			break;
		case 0xae: // XRA M
		{	//how to handle the hl register came from URL: https://github.com/kpmiller/emulator101/blob/master/8080emu-first50.c
			uint32_t hl = (state->h << 8) | state->l;
			state->a = state->a ^ hl;
			setLogicFlagsA(state);
		}
			break;
		case 0xaf: // XRA A
			state->a = state->a ^ state->a;
			setLogicFlagsA(state);
			break;
		/* ORA instructions*/
		case 0xb0: // ORA B
			state->a = state->a | state ->b;
			setLogicFlagsA(state);
			break;
		case 0xb1: // ORA C
			state->a = state->a | state->c;
			setLogicFlagsA(state);
			break;
		case 0xb2: // ORA D
			state->a = state->a | state->d;
			setLogicFlagsA(state);
			break;
		case 0xb3: // ORA E
			state->a = state->a | state->e;
			setLogicFlagsA(state);
			break;
		case 0xb4: // ORA H
			state->a = state->a | state->h;
			setLogicFlagsA(state);
			break;
		case 0xb5: // ORA L
			state->a = state->a | state->l;
			setLogicFlagsA(state);
			break;
		case 0xb6: // ORA M
		{
			uint32_t hl = (state->h << 8) | state->l;
			state->a = state->a | hl;
			setLogicFlagsA(state);
		}
			break;
		case 0xb7: // ORA A
			state->a = state->a | state->a;
			setLogicFlagsA(state);
			break;
		/* CMP instructions */
		case 0xb8: // CMP B
		{
			uint8_t value = state->a - state->b;
			state->cc.z = zero(value);
			state->cc.p = parity(value);
			state->cc.cy = (state->a < state->b);
		}
			break;
		case 0xb9: // CMP C
		{
			uint8_t value = state->a - state->c;
			state->cc.z = zero(value);
			state->cc.p = parity(value);
			state->cc.cy = (state->a < state->c);
		}
			break;
		case 0xba: // CMP D
		{
			uint8_t value = state->a - state->d;
			state->cc.z = zero(value);
			state->cc.p = parity(value);
			state->cc.cy = (state->a < state->d);
		}
			break;
		case 0xbb: // CMP E
		{
			uint8_t value = state->a - state->e;
			state->cc.z = zero(value);
			state->cc.p = parity(value);
			state->cc.cy = (state->a < state->e);
		}
			break;
		case 0xbc: // CMP H
		{
			uint8_t value = state->a - state->h;
			state->cc.z = zero(value);
			state->cc.p = parity(value);
			state->cc.cy = (state->a < state->h);
		}
			break;
		case 0xbd: // CMP L
		{
			uint8_t value = state->a - state->l;
			state->cc.z = zero(value);
			state->cc.p = parity(value);
			state->cc.cy = (state->a < state->l);
		}
			break;
		case 0xbe: // CMP M
		{
			uint32_t hl = (state->h << 8) | state->l;
			uint8_t value = state->a - hl;
			state->cc.z = zero(value);
			state->cc.p = parity(value);
			state->cc.cy = (state->a < hl);
		}
			break;
		case 0xbf: // CMP A
		{
			uint8_t value = state->a - state->a;
			state->cc.z = zero(value);
			state->cc.p = parity(value);
			state->cc.cy = (state->a < state->a);
		}
			break;
		/* RLC instructions */
		/* Code comes from swapping directions of RRC code from http://emulator101.com/ */
		case 0x07: // RLC
		{
			uint8_t value = state->a;
			state->a = (value >> 7) | (value << 1);
			state->cc.cy = (1 == (value & 1));
		}
			break;
		/* RRC instructions */
		/* Code comes from the Emulator Logic Branch page: http://emulator101.com/ */
		case 0x0f: // RRC 
		{
			uint8_t value = state->a;
			state->a = ((value & 1) << 7) | (value >> 1);
			state->cc.cy = (1 == (value & 1));
		}
			break;
		/* RAL instructions */
		/* Code comes from swapping directions of RAR code from http://emulator101.com/ */
		case 0x17: // RAL
		{
			uint8_t value = state->a;
			state->a = (state->cc.cy) | (value << 1);
			state->cc.cy = (1 == (value & 1));
		}
			break;
		/* RAR instructions */
		/* Code comes from the Emulator Logic Branch page: http://emulator101.com/ */
		case 0x1f: // RAR
		{
			uint8_t value = state->a;
			state->a = (state->cc.cy << 7) | (value >> 1);
			state->cc.cy = (1 == (value & 1));
		}
			break;
		/* ANI instruction */
		/* Code comes from the Emulator Logic Branch page: http://emulator101.com/ */
		case 0xe6: // ANI byte
		{
			uint8_t value = state->a & opCode[1];
			state->cc.z = zero(value);
			state->cc.s = logicSetSign(value);
			state->cc.p = parity(value);
			state->cc.cy = 0;
			state->a = value;
			state->pc++;
		}
			break;
		/* CMA instructions */
		/* Code comes from the Emulator Logic Branch page: http://emulator101.com/ */
		case 0x2f: // CMA
			state->a = ~state->a;
			break;
		/* XRI instructions */
		case 0xee: // XRI byte
		{
			uint8_t value = state->a ^ opCode[1];
			state->cc.z = zero(value);
			state->cc.s = logicSetSign(value);
			state->cc.p = parity(value);
			state->cc.cy = 0;
			state->a = value;
			state->pc++;
		}
			break;
		/* ORI instructions */
		case 0xf6: // ORI byte
		{
			uint8_t value = state->a | opCode[1];
			state->cc.z = zero(value);
			state->cc.s = logicSetSign(value);
			state->cc.p = parity(value);
			state->cc.cy = 0;
			state->a = value;
			state->pc++;
		}
			break;
		/* CPI instructions */
		/* Code comes from the Emulator Logic Branch page: http://emulator101.com/ */
		case 0xfe: // CPI byte
		{
			uint8_t value = state->a - opCode[1];
			state->cc.z = zero(value);
			state->cc.s = logicSetSign(value);
			state->cc.p = parity(value);
			state->cc.cy = (state->a < opCode[1]);
			state->pc++;
		}
			break;
		/* CMC instructions*/
		case 0x3f: // CMC
			state->cc.cy = ~state->cc.cy;
			break;
		/* STC instruction */
		case 0x37: // STC
			state->cc.cy = 1;
			break;
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