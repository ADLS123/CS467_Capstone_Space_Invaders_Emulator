/****************************************************************************************
** File Name: 8080State.h
** Description: This file contains prototypes for structs that define the state of the
** simulated 8080 cpu.
****************************************************************************************/

#include <stdint.h>


#ifndef STATE8080_H
#define STATE8080_H

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


#endif // STATE8080_H