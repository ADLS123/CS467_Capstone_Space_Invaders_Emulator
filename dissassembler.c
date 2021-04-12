#include <stdio.h>
/**
    ExtractROM is used to map an opcode to print instructions to
    the standard output based on the code currently being read. 
    The base code for this function was designed using the following link.
    http://www.emulator101.com/disassembler-pt-1.html
**/

int ExtractROM(unsigned char *buffer, int pc)    
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

int main() {
  

  return 0;
}