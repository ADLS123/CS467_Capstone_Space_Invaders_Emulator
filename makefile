# Type make into the command line to automatically compile all of the files
program: ./src/runShell.c 
	gcc -o spaceInvaders  ./src/cpu/emulatorShell.c ./src/runShell.c ./src/cpu/opcodeFunctions.c ./src/cpu/dissassembler.c ./src/machine/ioPorts.c ./src/debug/debugFunc.c ./src/debug/testAsm.c
