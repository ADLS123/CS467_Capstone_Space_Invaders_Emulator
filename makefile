# Type make into the command line to automatically compile all of the files
program: run_shell.c 
	gcc -o spaceInvaders  emulator_shell.c run_shell.c helperFunctions.c dissassembler.c
