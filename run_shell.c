#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "helperFunctions.h"
#include "cpu.h"

int main(int argc, char** argv) {
	State8080* = init8080CPU();
	readInvaderstoMem();

	while (1) {
		emulate8080(state);
	}

	return 0;
}