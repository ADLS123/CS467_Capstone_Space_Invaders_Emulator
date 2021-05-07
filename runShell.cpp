#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "helperFunctions.h"
#include "disassembler.h"
#include "cpu.h"
#include <time.h>


// Global variable for keeping time
struct timespec nowTime;
long lastInterrupt = 0;

int runRom() {
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

        // trigger interrupt if 1/60 of a second has passed since last interrupt, and interrupts are enabled
        clock_gettime(CLOCK_REALTIME, &nowTime);
        if (nowTime.tv_nsec - lastInterrupt > 1000000000. / 60. && state->int_enable) {
            generateInterrupt(state, 2);
            lastInterrupt = nowTime.tv_nsec;
        }

    }
    return 0;
}
