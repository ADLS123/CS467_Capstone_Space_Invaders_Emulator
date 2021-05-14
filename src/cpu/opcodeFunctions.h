/****************************************************************************************
** File Name: opcodeFunctions.h
** Description: This file contains function declarations for the helper functions for 
		the Space Invaders ROM
****************************************************************************************/
#include "cpu.h"

#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H


int parity(int value);		//checks parity of argument value
int setCarry(int value);
int setDoubleCarry(int value);
int logicSetSign(int value);
int zero(int value);		//function for setting the zero flag

/* Arithmetic group helper functions*/
void inxFunc(uint8_t* hi, uint8_t* lo);
void dcxFunc(uint8_t* hi, uint8_t* lo);

void callFunc(State8080* state, uint16_t callAddr);
void retFunc(State8080* state);
void popFunc(State8080* state, uint8_t* hi, uint8_t* lo);
void pushFunc(State8080* state, uint8_t* hi, uint8_t* lo);
void setLogicFlagsA(State8080* state);
void unimplementedInstruction(State8080* state);
void emulate8080(State8080* state);
void generateInterrupt(State8080* state, int interruptNum);



// read invaders roms to memory
void readInvaderstoMem(State8080* state);
void readFiletoMem(State8080* state, char* fileName, uint32_t offset);

// initialize state
State8080* init8080CPU(void);

// print opcode function prototype
int extractOpCode(unsigned char* buffer, int pc);

#endif
