# Type make into the command line to automatically compile all of the files
program: runShell.c 
	gcc -o spaceInvaders  emulatorShell.c runShell.c helperFunctions.c dissassembler.c
