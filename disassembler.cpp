#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

/**
    extractOPCode is used to map an opcode to print instructions to
    the standard output based on the code currently being read.
    The base code for this function was designed using the following link.
    http://www.emulator101.com/disassembler-pt-1.html
**/

int extractOpCode(unsigned char* buffer, int pc){
    unsigned char *current_op = &buffer[pc];
    // Number of bytes per operation
    int op_size= 1;
    printf ("%04x ", pc);

    // Switch statements mapping Opcode to instructions
    switch (*current_op)
    {
    //1st third of Disassembler assigned to Aguedo De Los Santos
    case 0x00: printf("NOP"); break;
    case 0x01: printf("LXI  B,#$%02x%02x", current_op[2], current_op[1]); op_size= 3; break;
    case 0x02: printf("STAX  B"); break;
    case 0x03: printf("INX  B"); break;
    case 0x04: printf("INR  B"); break;
    case 0x05: printf("DCR  B"); break;
    case 0x06: printf("MVI  B,#$%02x", current_op[1]); op_size= 2; break;
    case 0x07: printf("RLC"); break;
    case 0x08: printf("NOP"); break;
    case 0x09: printf("DAD  B"); break;
    case 0x0a: printf("LDAX  B"); break;
    case 0x0b: printf("DCX  B"); break;
    case 0x0c: printf("INR  C"); break;
    case 0x0d: printf("DCR  C"); break;
    case 0x0e: printf("MVI  C,#$%02x", current_op[1]); op_size= 2; break;
    case 0x0f: printf("RRC"); break;
    case 0x10: printf("NOP"); break;
    case 0x11: printf("LXI  D,#$%02x%02x", current_op[2], current_op[1]); op_size=3; break;
    case 0x12: printf("STAX D"); break;
    case 0x13: printf("INX  D"); break;
    case 0x14: printf("INR  D"); break;
    case 0x15: printf("DCR  D"); break;
    case 0x16: printf("MVI  D,#$%02x", current_op[1]); op_size=2; break;
    case 0x17: printf("RAL"); break;
    case 0x18: printf("NOP"); break;
    case 0x19: printf("DAD  D"); break;
    case 0x1a: printf("LDAX D"); break;
    case 0x1b: printf("DCX  D"); break;
    case 0x1c: printf("INR  E"); break;
    case 0x1d: printf("DCR  E"); break;
    case 0x1e: printf("MVI  E,#$%02x", current_op[1]); op_size = 2; break;
    case 0x1f: printf("RAR"); break;
    case 0x20: printf("NOP"); break;
    case 0x21: printf("LXI  H,#$%02x%02x", current_op[2], current_op[1]); op_size=3; break;
    case 0x22: printf("SHLD  $%02x%02x", current_op[2], current_op[1]); op_size=3; break;
    case 0x23: printf("INX  H"); break;
    case 0x24: printf("INR  H"); break;
    case 0x25: printf("DCR  H"); break;
    case 0x26: printf("MVI  H,#$%02x", current_op[1]); op_size=2; break;
        case 0x27: printf("DAA"); break;
        case 0x28: printf("NOP"); break;
        case 0x29: printf("DAD  H"); break;
        case 0x2a: printf("LHLD  $%02x%02x", current_op[2], current_op[1]); op_size=3; break;
        case 0x2b: printf("DCX  H"); break;
        case 0x2c: printf("INR  L"); break;
        case 0x2d: printf("DCR  L"); break;
        case 0x2e: printf("MVI  L,#$%02x", current_op[1]); op_size = 2; break;
        case 0x2f: printf("CMA"); break;
        case 0x30: printf("NOP"); break;
        case 0x31: printf("LXI  SP,#$%02x%02x", current_op[2], current_op[1]); op_size=3; break;
        case 0x32: printf("STA  $%02x%02x", current_op[2], current_op[1]); op_size=3; break;
        case 0x33: printf("INX  SP"); break;
        case 0x34: printf("INR  M"); break;
        case 0x35: printf("DCR  M"); break;
        case 0x36: printf("MVI  M,#$%02x", current_op[1]); op_size=2; break;
        case 0x37: printf("STC"); break;
        case 0x38: printf("NOP"); break;
        case 0x39: printf("DAD  SP"); break;
        case 0x3a: printf("LDA  $%02x%02x", current_op[2], current_op[1]); op_size=3; break;
        case 0x3b: printf("DCX  SP"); break;
        case 0x3c: printf("INR  A"); break;
        case 0x3d: printf("DCR  A"); break;
        case 0x3e: printf("MVI  A,#$%02x", current_op[1]); op_size = 2; break;
        case 0x3f: printf("CMC"); break;
        case 0x40: printf("MOV  B,B"); break;
        case 0x41: printf("MOV  B,C"); break;
        case 0x42: printf("MOV  B,D"); break;
        case 0x43: printf("MOV  B,E"); break;
        case 0x44: printf("MOV  B,H"); break;
        case 0x45: printf("MOV  B,L"); break;
        case 0x46: printf("MOV  B,M"); break;
        case 0x47: printf("MOV  B,A"); break;
        case 0x48: printf("MOV  C,B"); break;
        case 0x49: printf("MOV  C,C"); break;
        case 0x4a: printf("MOV  C,D"); break;
        case 0x4b: printf("MOV  C,E"); break;
        case 0x4c: printf("MOV  C,H"); break;
        case 0x4d: printf("MOV  C,L"); break;
        case 0x4e: printf("MOV  C,M"); break;
        case 0x4f: printf("MOV  C,A"); break;
        case 0x50: printf("MOV  D,B"); break;
        case 0x51: printf("MOV  D,C"); break;
        case 0x52: printf("MOV  D,D"); break;
        case 0x53: printf("MOV  D.E"); break;
        //2nd third of Disassembler assigned to Devin Swanson
        case 0x54: printf("MOV  D,H"); break;       //format for MOV is 1 <- 2 so D <- H
        case 0x55: printf("MOV  D,L"); break;
        case 0x56: printf("MOV  D,M"); break;
        case 0x57: printf("MOV  D,A"); break;
        case 0x58: printf("MOV  E,B"); break;
        case 0x59: printf("MOV  E,C"); break;
        case 0x5a: printf("MOV  E,D"); break;
        case 0x5b: printf("MOV  E,E"); break;
        case 0x5c: printf("MOV  E,H"); break;
        case 0x5d: printf("MOV  E,L"); break;
        case 0x5e: printf("MOV  E,M"); break;
        case 0x5f: printf("MOV  E,A"); break;
        case 0x60: printf("MOV  H,B"); break;
        case 0x61: printf("MOV  H,C"); break;
        case 0x62: printf("MOV  H,D"); break;
        case 0x63: printf("MOV  H,E"); break;
        case 0x64: printf("MOV  H,H"); break;
        case 0x65: printf("MOV  H,L"); break;
        case 0x66: printf("MOV  H,M"); break;
        case 0x67: printf("MOV  H,A"); break;
        case 0x68: printf("MOV  L,B"); break;
        case 0x69: printf("MOV  L,C"); break;
        case 0x6a: printf("MOV  L,D"); break;
        case 0x6b: printf("MOV  L,E"); break;
        case 0x6c: printf("MOV  L,H"); break;
        case 0x6d: printf("MOV  L,L"); break;
        case 0x6e: printf("MOV  L,M"); break;
        case 0x6f: printf("MOV  L,A"); break;
        case 0x70: printf("MOV  M,B"); break;
        case 0x71: printf("MOV  M,C"); break;
        case 0x72: printf("MOV  M,D"); break;
        case 0x73: printf("MOV  M,E"); break;
        case 0x74: printf("MOV  M,H"); break;
        case 0x75: printf("MOV  M,L"); break;
        case 0x76: printf("HLT"); break;            //special instruction
        case 0x77: printf("MOV  M,A"); break;
        case 0x78: printf("MOV  A,B"); break;
        case 0x79: printf("MOV  A,C"); break;
        case 0x7a: printf("MOV  A,D"); break;
        case 0x7b: printf("MOV  A,E"); break;
        case 0x7c: printf("MOV  A,H"); break;
        case 0x7d: printf("MOV  A,L"); break;
        case 0x7e: printf("MOV  A,M"); break;
        case 0x7f: printf("MOV  A,A"); break;
        //The following instructions all involve the Z,S,P,CY, and AC flags
        case 0x80: printf("ADD  B"); break;         //A <- A + VAL so A <- A + B
        case 0x81: printf("ADD  C"); break;
        case 0x82: printf("ADD  D"); break;
        case 0x83: printf("ADD  E"); break;
        case 0x84: printf("ADD  H"); break;
        case 0x85: printf("ADD  L"); break;
        case 0x86: printf("ADD  M"); break;
        case 0x87: printf("ADD  A"); break;
        case 0x88: printf("ADC  B"); break;         //Add-with-Carry instruction
        case 0x89: printf("ADC  C"); break;
        case 0x8a: printf("ADC  D"); break;
        case 0x8b: printf("ADC  E"); break;
        case 0x8c: printf("ADC  H"); break;
        case 0x8d: printf("ADC  L"); break;
        case 0x8e: printf("ADC  M"); break;
        case 0x8f: printf("ADC  A"); break;
        case 0x90: printf("SUB  B"); break;            //A <- A - VAL so A <- A - B
        case 0x91: printf("SUB  C"); break;
        case 0x92: printf("SUB  D"); break;
        case 0x93: printf("SUB  E"); break;
        case 0x94: printf("SUB  H"); break;
        case 0x95: printf("SUB  L"); break;
        case 0x96: printf("SUB  M"); break;
        case 0x97: printf("SUB  A"); break;
        case 0x98: printf("SBB  B"); break;             //Subtraction-with-Borrow
        case 0x99: printf("SBB  C"); break;
        case 0x9a: printf("SBB  D"); break;
        case 0x9b: printf("SBB  E"); break;
        case 0x9c: printf("SBB  H"); break;
        case 0x9d: printf("SBB  L"); break;
        case 0x9e: printf("SBB  M"); break;
        case 0x9f: printf("SBB  A"); break;
        case 0xa0: printf("ANA  B"); break;             //AND bit-operation
        case 0xa1: printf("ANA  C"); break;
        case 0xa2: printf("ANA  D"); break;
        case 0xa3: printf("ANA  E"); break;
        case 0xa4: printf("ANA  H"); break;
        case 0xa5: printf("ANA  L"); break;
        case 0xa6: printf("ANA  M"); break;
        case 0xa7: printf("ANA  A"); break;
        case 0xa8: printf("XRA  B"); break;             // Exclusive-Or operation
        case 0xa9: printf("XRA  C"); break;
        //final third assigned to Kevin Zhu
        case 0xaa: printf("XRA  D"); break;
        case 0xab: printf("XRA  E"); break;
        case 0xac: printf("XRA  H"); break;
        case 0xad: printf("XRA  L"); break;
        case 0xae: printf("XRA  M"); break;
        case 0xaf: printf("XRA  A"); break;
        case 0xb0: printf("ORA  B"); break;             // OR operation
        case 0xb1: printf("ORA  C"); break;
        case 0xb2: printf("ORA  D"); break;
        case 0xb3: printf("ORA  E"); break;
        case 0xb4: printf("ORA  H"); break;
        case 0xb5: printf("ORA  L"); break;
        case 0xb6: printf("ORA  M"); break;
        case 0xb7: printf("ORA  A"); break;
        case 0xb8: printf("CMP  B"); break;             // Compare operation (subtract A - specified register)
        case 0xb9: printf("CMP  C"); break;
        case 0xba: printf("CMP  D"); break;
        case 0xbb: printf("CMP  E"); break;
        case 0xbc: printf("CMP  H"); break;
        case 0xbd: printf("CMP  L"); break;
        case 0xbe: printf("CMP  M"); break;
        case 0xbf: printf("CMP  A"); break;
        case 0xc0: printf("RNZ"); break;                // if NZ (zero flag not set), return
        case 0xc1: printf("POP  B"); break;
        case 0xc2: printf("JNZ  $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;
        case 0xc3: printf("JMP  $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;
        case 0xc4: printf("CNZ  $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;      // if NZ, call address
        case 0xc5: printf("PUSH B"); break;
        case 0xc6: printf("ADI  #$%02x", current_op[1]), op_size = 2; break;                        // A = A + byte
        case 0xc7: printf("RST  0"); break;                                                         // CALL $0
        case 0xc8: printf("RZ"); break;                                                             // if Z (zero flag set), return
        case 0xc9: printf("RET"); break;
        case 0xca: printf("JZ   $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;      // if Z jump
        case 0xcb: printf("-"); break;
        case 0xcc: printf("CZ   $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;      // if Z, call address
        case 0xcd: printf("CALL $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;
        case 0xce: printf("ACI  #$%02x", current_op[1]); op_size = 2; break;                        // ADI but with carry
        case 0xcf: printf("RST  1"); break;                                                         // CALL $8
        case 0xd0: printf("RNC"); break;                                                            // if NCY, return
        case 0xd1: printf("POP  D"); break;
        case 0xd2: printf("JNC  $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;
        case 0xd3: printf("OUT"); op_size = 2;  break;
        case 0xd4: printf("CNC  $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;
        case 0xd5: printf("PUSH D"); break;
        case 0xd6: printf("SUI"); op_size = 2; break;
        case 0xd7: printf("RST  2"); break;                                                         // CALL $10
        case 0xd8: printf("RC"); break;
        case 0xd9: printf("-"); break;
        case 0xda: printf("JC   $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;
        case 0xdb: printf("IN"); op_size = 2; break;
        case 0xdc: printf("CC   $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;
        case 0xdd: printf("-"); break;
        case 0xde: printf("SBI"); op_size = 2; break;                                               // A <- A - data - carry
        case 0xdf: printf("RST  3"); break;                                                         // CALL $18
        case 0xe0: printf("RPO"); break;                                                            // PO = parity odd
        case 0xe1: printf("POP  H"); break;
        case 0xe2: printf("JPO  $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;
        case 0xe3: printf("XTHL"); break;                                                           // EXCHANGE: L with SP, and H with SP+1
        case 0xe4: printf("CPO  $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;
        case 0xe5: printf("PUSH H"); break;
        case 0xe6: printf("ANI"); op_size = 2; break;                                               // A <- A AND data
        case 0xe7: printf("RST  4"); break;                                                         // CALL $20
        case 0xe8: printf("RPE"); break;                                                            // PE = parity even
        case 0xe9: printf("PCHL"); break;
        case 0xea: printf("JPE  $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;
        case 0xeb: printf("XCHG"); break;                                                           // EXCHANGE: H with D, L with E
        case 0xec: printf("CPE  $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;
        case 0xed: printf("-"); break;
        case 0xee: printf("XRI"); op_size = 2; break;                                               // A <- A XOR data
        case 0xef: printf("RST  5"); break;                                                         // CALL $28
        case 0xf0: printf("RP"); break;
        case 0xf1: printf("POP  PSW"); break;
        case 0xf2: printf("JP   $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;
        case 0xf3: printf("DI"); break;
        case 0xf4: printf("CP   $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;
        case 0xf5: printf("PUSH PSW"); break;
        case 0xf6: printf("ORI"); op_size = 2; break;
        case 0xf7: printf("RST 6"); break;                                                          // CALL $30
        case 0xf8: printf("RM"); break;
        case 0xf9: printf("SPHL"); break;                                                           // load SP with 16bit from HL
        case 0xfa: printf("JM   $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;
        case 0xfb: printf("EI"); break;
        case 0xfc: printf("CM   $%02x%02x", current_op[2], current_op[1]); op_size = 3; break;
        case 0xfd: printf("-"); break;
        case 0xfe: printf("CPI"); op_size = 2; break;                                               // COPY A <- data
        case 0xff: printf("RST  7"); break;                                                         // CALL $38
    }

    printf("\n");
    return op_size;
}

/* Converts 2 byte little endian unsigned char representation to an integer.
Used for getting addresses from opcode arguments.*/
uint16_t convertFromLE(unsigned char* bytes) {
    // still needs additional testing, should be stored as LE, but may print out as if BE
    uint16_t result = (uint16_t)bytes[0] << 8 |
                      (uint16_t)bytes[1];
    return result;
}


void disassembleRom() {
    FILE* fp = fopen("invaders", "rb");
    if (fp == NULL) {
        fprintf(stderr, "ERROR: could not open file %s\n", "invaders");
        exit(1);
    }

    // open ROM file and read contents to memory, then close file
    fseek(fp, 0L, SEEK_END);
    size_t fsize = ftell(fp);
    fseek(fp, 0L, SEEK_SET);

    unsigned char* romBuffer = (unsigned char*)calloc(fsize + 1, sizeof(char));
    fread(romBuffer, fsize, 1, fp);
    fclose(fp);

    // iterate through entire contents of ROM in memory
    int pc = 0;
    while (pc < fsize) {
        pc += extractOpCode(romBuffer, pc);
    }

}
