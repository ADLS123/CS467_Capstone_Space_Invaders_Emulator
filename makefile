# Type make into the command line to automatically compile all of the files
program: dissassembler.c 
	gcc -o spaceInvaders  emulator_shell.c run_shell.c dissassembler.c
	cat ./roms/invaders.h > invaders
	cat ./roms/invaders.g >> invaders
	cat ./roms/invaders.f >> invaders
	cat ./roms/invaders.e >> invaders