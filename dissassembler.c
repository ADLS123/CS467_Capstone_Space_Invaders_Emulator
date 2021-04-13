#include <stdio.h>
/**
    ExtractROM is used to map an opcode to print instructions to
    the standard output based on the code currently being read. 
    The base code for this function was designed using the following link.
    http://www.emulator101.com/disassembler-pt-1.html
**/

int extractOpCode(unsigned char *buffer, int pc)    
{    
unsigned char *current_op = &buffer[pc];   
// Number of bytes per operation  
int op_size= 1;  
printf ("%04x ", pc);    

// Switch statements mapping Opcode to instructions
switch (*current_op)    
{    
    case 0x00: printf("NOP"); break;    
 
} 
}

int main(int argc, char** argv) {
    FILE* fp = fopen(argv[1], "rb");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: could not open file %s\n", argv[1]);
        exit(1);
    }

    // open ROM file and read contents to memory, then close file
    fseek(fp, 0L, SEEK_END);
    size_t fsize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    char* romBuffer = calloc(fsize + 1, sizeof(char));
    fread(romBuffer, fsize, 1, fp);
    fclose(fp);

    // iterate through entire contents of ROM in memory
    int pc = 0;
    while (pc < fsize) {
        pc += extractOpCode(romBuffer, pc);
    }

    return 0;
}