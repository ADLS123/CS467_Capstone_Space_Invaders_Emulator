# Type make into the command line to automatically compile all of the files
program: dissassembler.c 
	gcc -o dissassembler  dissassembler.c
	cat ./roms/invaders.h > invaders
	cat ./roms/invaders.g >> invaders
	cat ./roms/invaders.f >> invaders
	cat ./roms/invaders.e >> invaders