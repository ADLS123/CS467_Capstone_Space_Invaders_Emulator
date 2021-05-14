#include <stdlib.h>
#include <stdint.h>

#include "../cpu/cpu.h"


#ifndef TESTASM_H
#define TESTASM_H


/* Loads diagnostic 8080 assembly code for our program to run
Taken from emulator101.com */
void loadTestAsm(State8080* state);
// Modified CALL function to work with diagnostic code taken from emulator101.com
void debugCALL(State8080* state);

#endif