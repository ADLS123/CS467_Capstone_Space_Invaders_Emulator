/****************************************************************************************
** File Name: helperFunctions.h
** Authors: Devin Swanson, Kevin Zhu
** Description: This file contains function declarations for the helper functions for 
		the Space Invaders ROM
****************************************************************************************/


#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H


int parity(int value);		//checks parity of argument value
int setCarry(int value);
int setDoubleCarry(int value);
int logicSetSign(int value);
int zero(int value);		//function for setting the zero flag

void callFunc(State8080*, uint16_t);
void retFunc(State8080*);
void setLogicFlagsA(State8080* state);
void unimplementedInstruction(State8080*);
void emulate8080(State8080*);

// read invaders roms to memory
void readInvaderstoMem(State8080* state);
void readFiletoMem(State8080* state, char* fileName, uint32_t offset);

// initialize state
void init8080CPU(void);

#endif
