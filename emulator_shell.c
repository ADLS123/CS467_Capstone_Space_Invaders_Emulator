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
		case 0x07: // RLC
		{
			uint8_t value = state->a;
			state->a = (value >> 7) | (value << 1);
			state->cc.cy = (128 == (value & 128));	//sets carry to high order bit
		}
			break;
		/* RRC instructions */
		/* Code comes from the Emulator Logic Branch page: http://emulator101.com/ */
		case 0x0f: // RRC 
		{
			uint8_t value = state->a;
			state->a = ((value & 1) << 7) | (value >> 1);
			state->cc.cy = (1 == (value & 1));		//sets carry bit to low order bit
		}
			break;
		/* RAL instructions */
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
		//performs an AND operation on the accumulator with operand, resets carry bit to 0
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
		/* MOV instructions */
		case 0x40: // MOV B, B
			state->b = state->b;
			break;
		case 0x41: // MOV B, C
			state->b = state->c;
			break;
		case 0x42: // MOV B, D
			state->b = state->d;
			break;
		case 0x43: // MOV B, E
			state->b = state->e;
			break;
		case 0x44: // MOV B, H
			state->b = state->h;
			break;
		case 0x45: // MOV B, L
			state->b = state->l;
			break;
		case 0x46: // MOV B, M
		{	// code for Moving M to from URL: https://github.com/kpmiller/emulator101/blob/master/8080emu-first50.c
			uint16_t offset = (state->h << 8) | (state->l);
			state->b = state->memory[offset];
		}
			break;
		case 0x47: // MOV B, A
			state->b = state->a;
			break;
		case 0x48:  // MOV C, B
			state->c = state->b;
			break;
		case 0x49: // MOV C, C
			state->c = state->c;
			break;
		case 0x4a: // MOV C, D
			state->c = state->d;
			break;
		case 0x4b: // MOV C, E
			state->c = state->e;
			break;
		case 0x4c: // MOV C, H
			state->c = state->h;
			break;
		case 0x4d: // MOV C, L
			state->c = state->l;
			break;
		case 0x4e: // MOV C, M
		{	// code for Moving M to from URL: https://github.com/kpmiller/emulator101/blob/master/8080emu-first50.c
			uint16_t offset = (state->h << 8) | (state->l);
			state->c = state->memory[offset];
		}
			break;
		case 0x4f: // MOV C, A
			state->c = state->a;
			break;
		case 0x50: // MOV D, B
			state->d = state->b;
			break;
		case 0x51: // MOV D, C
			state->d = state->c;
			break;
		case 0x52: // MOV D, D
			state->d = state->d;
			break;
		case 0x53: // MOV D, E
			state->d = state->e;
			break;
		case 0x54: // MOV D, H
			state->d = state->h;
			break;
		case 0x55: // MOV D, L
			state->d = state->l;
			break;
		case 0x56: // MOV D, M
		{	// code for Moving M to from URL: https://github.com/kpmiller/emulator101/blob/master/8080emu-first50.c
			uint16_t offset = (state->h << 8) | (state->l);
			state->d = state->memory[offset];
		}
			break;
		case 0x57: // MOV D, A
			state->d = state->a;
			break;
		case 0x58: // MOV E, B
			state->e = state->b;
			break;
		case 0x59: // MOV E, C
			state->e = state->c;
			break;
		case 0x5a: // MOV E, D
			state->e = state->d;
			break;
		case 0x5b: // MOV E, E
			state->e = state->e;
			break;
		case 0x5c: // MOV E, H
			state->e = state->h;
			break;
		case 0x5d: // MOV E, L
			state->e = state->l;
			break;
		case 0x5e: // MOV E, M
		{	// code for Moving M to from URL: https://github.com/kpmiller/emulator101/blob/master/8080emu-first50.c
			uint16_t offset = (state->h << 8) | (state->l);
			state->e = state->memory[offset];
		}
			break;
		case 0x5f: // MOV E, A
			state->e = state->a;
			break;
		case 0x60: // MOV H, B
			state->h = state->b;
			break;
		case 0x61: // MOV H, C
			state->h = state->c;
			break;
		case 0x62: // MOV H, D
			state->h = state->d;
			break;
		case 0x63: // MOV H, E
			state->h = state->e;
			break;
		case 0x64: // MOV H, H
			state->h = state->h;
			break;
		case 0x65: // MOV H, L
			state->h = state->l;
			break;
		case 0x66: // MOV H, M
		{	// code for Moving M to from URL: https://github.com/kpmiller/emulator101/blob/master/8080emu-first50.c
			uint16_t offset = (state->h << 8) | (state->l);
			state->h = state->memory[offset];
		}
			break;
		case 0x67: // MOV H, A
			state->h = state->a;
			break;
		case 0x68: // MOV L, B
			state->l = state->b;
			break;
		case 0x69: // MOV L, C
			state->l = state->c;
			break;
		case 0x6a: // MOV L, D
			state->l = state->d;
			break;
		case 0x6b: // MOV L, E
			state->l = state->e;
			break;
		case 0x6c: // MOV L, H
			state->l = state->h;
			break;
		case 0x6d: // MOV L, L
			state->l = state->l;
			break;
		case 0x6e: // MOV L, M
		{	// code for Moving M to from URL: https://github.com/kpmiller/emulator101/blob/master/8080emu-first50.c
			uint16_t offset = (state->h << 8) | (state->l);
			state->l = state->memory[offset];
		}
			break;
		case 0x6f: // MOV L, A
			state->l = state->a;
			break;
		/*
		* MOV M, x comes  from URL: https://github.com/kpmiller/emulator101/blob/master/8080emu-first50.c
		*/
		case 0x70: // MOV M, B
		{	
			uint16_t offset = (state->h << 8) | (state->l);
			state->memory[offset] = state->b;
		}
			break;
		case 0x71: // MOV M, C
		{
			uint16_t offset = (state->h << 8) | (state->l);
			state->memory[offset] = state->c;
		}
			break;
		case 0x72: // MOV M, D
		{
			uint16_t offset = (state->h << 8) | (state->l);
			state->memory[offset] = state->d;
		}
			break;
		case 0x73: // MOV M, E
		{
			uint16_t offset = (state->h << 8) | (state->l);
			state->memory[offset] = state->e;
		}
			break;
		case 0x74: // MOV M, H
		{
			uint16_t offset = (state->h << 8) | (state->l);
			state->memory[offset] = state->h;
		}
			break;
		case 0x75: // MOV M, L
		{
			uint16_t offset = (state->h << 8) | (state->l);
			state->memory[offset] = state->l;
		}
			break;
		case 0x77: // MOV M, A
		{
			uint16_t offset = (state->h << 8) | (state->l);
			state->memory[offset] = state->a;
		}
			break;
		case 0x78: // MOV A, B
			state->a = state->b;
			break;
		case 0x79: // MOV A, C
			state->a = state->c;
			break;
		case 0x7a: // MOV A, D
			state->a = state->d;
			break;
		case 0x7b: // MOV A, E
			state->a = state->e;
			break;
		case 0x7c: // MOV A, H
			state->a = state->h;
			break;
		case 0x7d: // MOV A, L
			state->a = state->l;
			break;
		case 0x7e: // MOV A, M
		{	// code for Moving M to from URL: https://github.com/kpmiller/emulator101/blob/master/8080emu-first50.c
			uint16_t offset = (state->h << 8) | (state->l);
			state->a = state->memory[offset];
		}
			break;
		case 0x7f: // MOV A, A
			state->a = state->a;
			break;
		/* MVI Instructions */
		case 0x06: // MVI B, D8
			state->b = opCode[1];
			state->pc++;
			break;
		case 0x0e: // MVI C, D8
			state->c = opCode[1];
			state->pc++;
			break;
		case 0x16: // MVI D, D8
			state->d = opCode[1];
			state->pc++;
			break;
		case 0x1e: // MVI E, D8
			state->e = opCode[1];
			state->pc++;
			break;
		case 0x26: // MVI H, D8
			state->h = opCode[1];
			state->pc++;
			break;
		case 0x2e: // MVI L, D8
			state->l = opCode[1];
			state->pc++;
			break;
		case 0x36: // MVI M, D8
		{ //from URL : https://github.com/kpmiller/emulator101/blob/master/8080emu-first50.c
			uint16_t offset = (state->h << 8) | state->l;
			state->memory[offset] = opCode[1];
			state->pc++;
		}
			break;
		case 0x3e: // MVI A, D8
			state->a = opCode[1];
			state->pc++;
			break;
		/* LXI instructions */
		// from URL : https://github.com/kpmiller/emulator101/blob/master/8080emu-first50.c
		case 0x01: // LXI B, D16
			state->c = opCode[1];
			state->b = opCode[2];
			state->pc += 2;
			break;
		case 0x11: // LXI D, D16
			state->e = opCode[1];
			state->d = opCode[2];
			state->pc += 2;
			break;
		case 0x21: // LXI H, D16
			state->l = opCode[1];
			state->h = opCode[2];
			break;
		case 0x31: // LXI SP, D16
			//from URL : https://github.com/kpmiller/emulator101/blob/master/8080emu-first50.c
			state->sp = (opCode[2] << 8) | opCode[1];
			state->pc += 2;
			break;
		/* LDAX instructions */
		case 0x0a: // LDAX B
		{
			uint16_t offset = (state->b << 8) | state->c;
			state->a = state->memory[offset];
		}
			break;
		case 0x1a: // LDAX D
		{
			uint16_t offset = (state->d << 8) | state->e;
			state->a = state->memory[offset];
		}
			break;
		/* STAX instructions */
		case 0x02: // STAX B
		{
			uint16_t offset = (state->b << 8) | state->c;
			state->memory[offset] = state->a;
		}
			break;
		case 0x12: // STAX D
		{
			uint16_t offset = (state->d << 8) | state->e;
			state->memory[offset] = state->a;
		}
			break;
		/* Direct Access Instructions */
		case 0x22: // SHLD adr 2 bytes
		{
			uint16_t offset = (opCode[2] << 8) | opCode[1];
			state->memory[offset] = state->l;
			state->memory[offset + 1] = state->h;
			state->pc += 2;
		}
			break;
		case 0x2a: // LHLD adr 2 bytes
		{
			uint16_t offset = (opCode[2] << 8) | opCode[1];
			state->l = state->memory[offset];
			state->h = state->memory[offset + 1];
			state->pc += 2;
		}
			break;
		case 0x32: // STA adr 2 bytes
		{
			uint16_t offset = (opCode[2] << 8) | opCode[1];
			state->memory[offset] = state->a;
			state->pc += 2;
		}
			break;
		case 0x3a: // LDA adr 2 bytes
		{
			uint16_t offset = (opCode[2] << 8) | opCode[1];
			state->a = state->memory[offset];
			state->pc += 2;
		}
			break;
		case 0xe9: // PCHL 
		{
			uint16_t offset = (state->h << 8) | state->l;
			state->pc = offset;
		}
			break;
		case 0xeb: // XCHG
		{
			uint8_t tempD = state->d;
			uint8_t tempE = state->e;
			state->d = state->h;
			state->e = state->l;
			state->h = tempD;
			state->l = tempE;
		}
			break;
		/* Special Group Instructions */
		case 0xfb: // EI: sets the integer enable flag
			state->int_enable = 1;
			break;
		case 0xf3: // DI: disables the integer enable flag
			state->int_enable = 0;
			break;
		case 0x76: // HLT 
			exit(0);
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

		/* Stack group push and pop instructions */
		case 0xc1: // POP B
			popFunc(state, &state->b, &state->c);
			break;

		case 0xc5: // PUSH B
			pushFunc(state, &state->b, &state->c);
			break;

		case 0xd1: // POP D
			popFunc(state, &state->d, &state->e);
			break;

		case 0xd5: // PUSH D
			pushFunc(state, &state->d, &state->e);
			break;

		case 0xe1: // POP H
			popFunc(state, &state->h, &state->l);
			break;

		case 0xe5: // PUSH H
			pushFunc(state, &state->h, &state->l);
			break;

		case 0xf1: // POP PSW
		{
			uint8_t psw;
			popFunc(state, &state->a, &psw);
			// update flags according to psw
			// bit shift right bit to bit 0, and then apply bitmask
			state->cc.cy = psw & 0x01;
			state->cc.p = (psw >> 2) & 0x01;
			state->cc.ac = (psw >> 4) & 0x01;
			state->cc.z = (psw >> 6) & 0x01;
			state->cc.s = (psw >> 7) & 0x01;
		}
			break;

		case 0xf5: // PUSH PSW
		{
			uint8_t psw = (state->cc.s << 7 |
				state->cc.z << 6 |
				state->cc.ac << 4 |
				state->cc.p << 2 |
				state->cc.cy);
			pushFunc(state, &state->a, &psw);
		}
			break;

		case 0xf9: // SPHL
			state->sp = state->h << 8 | state->l;
			break;

		case 0xe3: // XTHL
		{
			uint8_t hi_buffer = state->h;
			uint8_t lo_buffer = state->l;
			popFunc(state, &state->h, &state->l);
			pushFunc(state, &hi_buffer, &lo_buffer);
		}
			break;
				/*--------------------------------*/
		/* INX instructions, Flags are not affected: Register  + 1 */
		case 0x03: //INX B
			(uint16_t) value = (uint16_t) state->b + 1;     
            state->b = value & 0xff;  
			break; 
		case 0x13: //INX D
			(uint16_t) value = (uint16_t) state->d + 1;     
            state->d = value & 0xff;  
			break; 
		case 0x23: //INX H
			(uint16_t) value = (uint16_t) state->h + 1;     
            state->h = value & 0xff;  
			break; 
		case 0x33: //INX SP
			(uint16_t) value = (uint16_t) state->sp + 1;     
            state->sp = value & 0xff;  
			break; 
		/* DCX instructions, Flags are not affected: Register  - 1 */
		case 0x0b: //DCX B
			(uint16_t) value = (uint16_t) state->b - 1;     
            state->b = value & 0xff;  
			break; 
		case 0x1b: //DCX D
			(uint16_t) value = (uint16_t) state->d - 1;     
            state->d = value & 0xff;  
			break;
		case 0x2b: //DCX H
			(uint16_t) value = (uint16_t) state->h - 1;     
            state->h = value & 0xff;  
			break;
		case 0x3b: //DCX SP
			(uint16_t) value = (uint16_t) state->sp - 1;     
            state->sp = value & 0xff;  
			break;
		/* INR instructions, Flags are affected: Register  + 1 */
		case 0x04: //INR B
			(uint16_t) value = (uint16_t) state->b + 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->b = value & 0xff;  
			break;
		case 0x0c: //INR C
			(uint16_t) value = (uint16_t) state->c + 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->c = value & 0xff;  
			break;
		case 0x14: //INR D
			(uint16_t) value = (uint16_t) state->d + 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->d = value & 0xff;  
			break;
		case 0x1C: //INR E
			(uint16_t) value = (uint16_t) state->e + 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->e = value & 0xff;  
			break;
		case 0x24: //INR H
			(uint16_t) value = (uint16_t) state->h + 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->h = value & 0xff;  
			break;
		case 0x2C: //INR L
			(uint16_t) value = (uint16_t) state->l + 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->l = value & 0xff;  
			break;
		case 0x34: //INR M
			(uint16_t) value = (uint16_t) state->m + 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->m = value & 0xff;  
			break;
		case 0x3c: //INR A
			(uint16_t) value = (uint16_t) state->a + 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		/* DCR instructions, Flags are affected: Register  - 1 */
		case 0x05: //DCR B
			(uint16_t) value = (uint16_t) state->b - 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->b = value & 0xff;  
			break;
		case 0x0d: //DCR C
			(uint16_t) value = (uint16_t) state->c - 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->c = value & 0xff;  
			break;
		case 0x15: //DCR D
			(uint16_t) value = (uint16_t) state->d - 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->d = value & 0xff;  
			break;
		case 0x1d: //DCR E
			(uint16_t) value = (uint16_t) state->e - 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->e = value & 0xff;  
			break;
		case 0x25: //DCR H
			(uint16_t) value = (uint16_t) state->h - 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->h = value & 0xff;  
			break;
		case 0x2d: //DCR L
			(uint16_t) value = (uint16_t) state->l - 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->l = value & 0xff;  
			break;
		case 0x35: //DCR M
			(uint16_t) value = (uint16_t) state->m - 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->m = value & 0xff;  
			break;
		case 0x3d: //DCR A
			(uint16_t) value = (uint16_t) state->a - 1;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;

		/* DAD instructions, Flags are not affected: Register  + 1 */
		/* PLACEHOLDER */
		case 0x80: //ADD B
			(uint16_t) value = (uint16_t) state->a + (uint16_t) state->b;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break; 
		/* ADD instructions: Register A + Mem */
		case 0x80: //ADD B
			(uint16_t) value = (uint16_t) state->a + (uint16_t) state->b;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break; 
		case 0x81:      //ADD C        
            (uint16_t) value = (uint16_t) state->a + (uint16_t) state->c;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x82:      //ADD D        
            (uint16_t) value = (uint16_t) state->a + (uint16_t) state->d;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x83:      //ADD E        
            (uint16_t) value = (uint16_t) state->a + (uint16_t) state->e;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x84:      //ADD H        
            (uint16_t) value = (uint16_t) state->a + (uint16_t) state->h;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x85:      //ADD L        
            (uint16_t) value = (uint16_t) state->a + (uint16_t) state->l;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x86:      //ADD M        
            (uint16_t) value = (uint16_t) state->a + (uint16_t) state->m;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x87:      //ADD A        
            (uint16_t) value = (uint16_t) state->a + (uint16_t) state->a;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		/* ADC with Carry instructions: Register A + Mem  + Carry Resgister*/
		case 0x88:      //ADC B        
            (uint16_t) value = (uint16_t) state->a + (uint16_t) state->b + (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x89:      //ADC C        
            (uint16_t) value = (uint16_t) state->a + (uint16_t) state->c + (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x8a:      //ADC D        
            (uint16_t) value = (uint16_t) state->a + (uint16_t) state->d + (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x8b:      //ADC E        
            (uint16_t) value = (uint16_t) state->a + (uint16_t) state->e + (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x8c:      //ADC H       
            (uint16_t) value = (uint16_t) state->a + (uint16_t) state->h + (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x8d:      //ADC L      
            (uint16_t) value = (uint16_t) state->a + (uint16_t) state->l + (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x8e:      //ADC M     
            (uint16_t) value = (uint16_t) state->a + (uint16_t) state->m + (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x8f:      //ADC A 
            (uint16_t) value = (uint16_t) state->a + (uint16_t) state->a + (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		/* Subtract instructions */
		case 0x90:      //SUB B 
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->b;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x91:      //SUB C
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->c;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x92:      //SUB D
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->d;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x93:      //SUB E
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->e;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x94:      //SUB H
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->h;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x95:      //SUB L
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->l;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x96:      //SUB M
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->m;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x97:      //SUB A
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->a;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		/* Subtract With Borrow instructions */
		case 0x98:      //SBB B
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->b - (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x99:      //SBB C
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->c - (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x9a:      //SBB D
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->d - (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x9b:      //SBB E
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->e - (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x9c:      //SBB H
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->h - (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x9d:      //SBB L
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->l - (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x9e:      //SBB E
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->e - (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		case 0x9f:      //SBB F
            (uint16_t) value = (uint16_t) state->a - (uint16_t) state->f - (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			break;
		/* ADI Add Immediate to Accumulator instructions */
		case 0xc6:      //ADI + Byte
            (uint16_t) value = (uint16_t) state->a + (uint16_t) opCode[1];    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			state->pc++;
			break;
		/* ACI Add Immediate to Accumulator minus carry instructions */
		case 0xce:      //ACI + Byte
            (uint16_t) value = (uint16_t) state->a + (uint16_t) opCode[1] + (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			state->pc++;
			break;
		/* SUI Subtract Immediate to Accumulator instructions */
		case 0xd6:      //SUI - Byte
            (uint16_t) value = (uint16_t) state->a - (uint16_t) opCode[1];    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			state->pc++;
			break;
		/* SBI Subtract Immediate to Accumulator minus carry instructions */
		case 0xde:      //SBI - Byte
            (uint16_t) value = (uint16_t) state->a - (uint16_t) opCode[1] - (uint16_t)state->cc.cy;    
            state->cc.z = ((value & 0xff) == 0);    
            state->cc.s = ((value & 0x80) != 0);    
            state->cc.cy = (value > 0xff);    
            state->cc.p = parity(value&0xff);    
            state->a = value & 0xff;  
			state->pc++;
			break;
		/**************************************************************************************************
		* THE FOLLOWING IN AND OUT INSTRUCTIONS ARE NOT IMPLEMENTED YET
		***************************************************************************************************/
		case 0xdb: // IN D8 NOTE: PLACEHOLDER COME BACK TO IMPLEMENT
			state->pc++;
			break;
		case 0xd3: // OUT D8 NOTE: PLACEHOLDER COME BACK TO IMPLEMENT
			state->pc++;
			break;
		case 0x00: // NOP
			break;
			/* JUMP instructions */
		case 0xc2: // JNZ
			if (state->cc.z == 0)
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