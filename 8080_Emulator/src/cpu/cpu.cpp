/**************************************************************************************************
    ** File Name: cpu.cpp
    ** Description: Contains the member function definitions for the Cpu Class.
**************************************************************************************************/
#include "cpu.h"


/**************************************************************************************************
    ** Function Name: Cpu::Cpu()
    ** Description: The default constructor for an object of the Cpu class.
**************************************************************************************************/
Cpu::Cpu()
{
    //allocate memory for the registers and condition codes struct
    memset(&registers, 0, sizeof(State8080Registers));


    //initializing input values
    input0 = 0b01110000;
    input1 = 0b00001000;
    input2 = 0;
    input3 = 0;

    //initialize output values
    output2 = 0;
    output3 = 0;
    output4 = 0;
    output5 = 0;
    output6 = 0;

    enableInterrupts = false;       //disabling interrupts to being
    twoPlayer = false;              //setting to 1 player
    memory = new uint8_t[0x4000];    //allocate memory for the RAM
}


/**************************************************************************************************
    ** Function Name: Cpu::~Cpu()
    ** Description: Deconstructor for the Cpu class, frees up the memory allocated for the RAM.
**************************************************************************************************/
Cpu::~Cpu(){
    delete memory;
}


/**************************************************************************************************
    ** Function Name: uint8_t Cpu::getLowBits(uint8_t value)
    ** Arguments: An unsigned 8 bit value
    ** Returns: The lower 4 bits of the argument value
    ** Description: This function takes a 8-bit value, ands it with 1111 to get the lower 4 bits
            of the value.
**************************************************************************************************/
uint8_t Cpu::getLowBits(uint8_t value){
    return (value & 0x0F);
}


/**************************************************************************************************
    ** Function Name: uint8_t Cpu::getLowBits(uint16_t value)
    ** Arguments: An unsigned 16 bit value
    ** Returns: The lower 8 bits of the argument value
    ** Description: This function takes a 8-bit value, ands it with 1111 1111 to get the lower
        8 bits of the value.
**************************************************************************************************/
uint8_t Cpu::getLowBits(uint16_t value){
    return value & 0x00FF;
}


/**************************************************************************************************
    ** Function Name: uint8_t Cpu::getLowBits(uint8_t value)
    ** Arguments: An unsigned 8 bit value
    ** Returns: The higher 4 bits of the argument value
    ** Description: This function takes a 8-bit value, ands it with 1111 0000 to get the higher
        4 bits of the value.
**************************************************************************************************/
uint8_t Cpu::getHighBits(uint8_t value){
    return (value & 0xF0) >> 4;
}


/**************************************************************************************************
    ** Function Name: uint8_t Cpu::getLowBits(uint16_t value)
    ** Arguments: An unsigned 16 bit value
    ** Returns: The higher 8 bits of the argument value
    ** Description: This function takes a 16-bit value, ands it with 1111 1111 0000 0000 to get
        the higher 8 bits of the value.
**************************************************************************************************/
uint8_t Cpu::getHighBits(uint16_t value){
    return (value & 0xFF00) >> 8;
}


/**************************************************************************************************
    ** Function Name: int Cpu::addBytes(uint8_t byte, uint8_t byte2, bool carryIn, Flags, flagVar)
    ** Description: Checks a bool condition to check the carry bit, sets the aux bit if the sum
            of the lower bits of the 2 parameter values + the carry bit is less than 0x10.
            Then sets the carry bit to the sum if of the two parameter bytes + the carry flag is
            less than 0x100. The function then sets the rest of the flags as appropriate.
**************************************************************************************************/
int Cpu::addBytes(uint8_t byte, uint8_t byte2, bool carryIn, Flags flagVar){

    uint8_t carry;
    if(carryIn){
        carry = flags.testBits(CARRY_BIT);
    }
    else{
        carry = 0;
    }

    if(flagVar.testBits(AUX_BIT)){
        uint8_t sum = getLowBits(byte) + getLowBits(byte2) + carry;
        flags.setBits(AUX_BIT, sum >= 0x10);
    }

    uint16_t sum = byte + byte2 + carry;
    if(flagVar.testBits(CARRY_BIT)){
        flags.setBits(CARRY_BIT, sum >= 0x100);
    }

    //set rest of flags
    if(flagVar.testBits(ZERO_BIT)){
        flags.zero(sum);
    }
    if(flagVar.testBits(SIGN_BIT)){
        flags.sign(sum);
    }
    if(flagVar.testBits(PARITY_BIT)){
        flags.evenParity(sum);
    }
    return sum;
}

//Opcode functions
/*NOTES:
        - The M register is the location in memory pointed to by the combined register HL.
        For example: If HL = 0xABCD, then the M register would be at memory[0xABCD]

        - The 8080 CPU is little endian, meaning that higher order 8 bits of 16 bit integers
        are stored after the lower order bits.
        For example: The value 0xABCD would be stored as 0xCDAB in memory
*/

//AND instruction for a parameter register with the a register.
int Cpu::ana(uint8_t secondRegister){
    registers.a = registers.a & secondRegister;
    flags.setBits(CARRY_BIT, false);
    flags.calcAllBits(registers.a);
    registers.pc++;
    return 4;
}

//ana function but for the special "M Register"
int Cpu::anaM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    ana(memory[offset]);
    return 7;
}

//XOR instruction for a parameter register with the a register
int Cpu::xra(uint8_t secondRegister){
    registers.a = registers.a ^ secondRegister;
    flags.setBits(CARRY_BIT, false);
    flags.calcAllBits(registers.a);
    registers.pc++;
    return 4;
}

//xra function for the special "M Register"
int Cpu::xraM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    xra(memory[offset]);
    return 7;
}

//OR instruction for parameter register and the a register
int Cpu::ora(uint8_t secondRegister){
    registers.a = registers.a | secondRegister;
    flags.setBits(CARRY_BIT, false);
    flags.calcAllBits(registers.a);
    registers.pc++;
    return 4;
}

//ora function for the special "M Register"
int Cpu::oraM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    ora(memory[offset]);
    return 7;
}

//compares the a register and the parameter register to see which value held is larger
int Cpu::cmp(uint8_t secondRegister){
    Flags calc(SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT | CARRY_BIT);
    addBytes(registers.a, -secondRegister, false, calc);

    flags.setBits(CARRY_BIT, !flags.testBits(CARRY_BIT));
    registers.pc++;
    return 4;
}

//cmp function for special "M Register"
int Cpu::cmpM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    cmp(memory[offset]);
    return 7;
}

//the following functions rotate the bits in the accumulator register (a register)

/**************************************************************************************************
 ** Function name: rlc()
 ** Description: Sets the carry bit equal to the high order bit of the accumulaotr, accumulator is
        rotated one bit position to the left, with the high order bit being transferred to the low
        order bit position.
**************************************************************************************************/
int Cpu::rlc(){
    uint8_t value = (registers.a & 0x80) >> 7;
    flags.setBits(CARRY_BIT, value);
    registers.a = registers.a << 1;
    registers.a = registers.a | value;
    registers.pc++;
    return 4;
}


/**************************************************************************************************
 ** Function name: rrc()
 ** Description: Sets the carry bit equal to the low order bit of the accumulator, accumulator is
        rotated one bit position to the right, with the low order bit being transferred to the high
        order bit position.
**************************************************************************************************/
int Cpu::rrc(){
    uint8_t value = (registers.a & 0x01);
    flags.setBits(CARRY_BIT, value);
    registers.a = registers.a >> 1;
    registers.a = registers.a | (value << 7);
    registers.pc++;
    return 4;
}


/**************************************************************************************************
 ** Function name: ral()
 ** Description: Contents of accumulator rotated one bit position to the left, high-order bit of
        accumulator replaces the Carry bit, while Carry bit replaces high order bit of accumulator.
**************************************************************************************************/
int Cpu::ral(){
    uint8_t newValue = (registers.a & 0x80) >> 7;
    uint8_t oldValue = flags.testBits(CARRY_BIT);
    flags.setBits(CARRY_BIT, newValue);
    registers.a = registers.a << 1;
    registers.a = registers.a | oldValue;
    registers.pc++;
    return 4;
}


/**************************************************************************************************
 ** Function name: rar()
 ** Description: Contents of accumulator rotated one bit position to the right, low-order bit of
        accumulator replaces the Carry bit, while Carry bit replaces high order bit of accumulator.
**************************************************************************************************/
int Cpu::rar(){
    uint8_t newValue = (registers.a & 0x01);
    uint8_t oldValue = flags.testBits(CARRY_BIT);
    flags.setBits(CARRY_BIT, newValue);
    registers.a = registers.a >> 1;
    registers.a = registers.a | (oldValue << 7);
    registers.pc++;
    return 4;
}

//special ana function with an immediate value
int Cpu::ani(){
    ana(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

//produces one's complement of register a
int Cpu::cma(){
    registers.a = registers.a ^ 0xFF;
    registers.pc++;
    return 4;
}

//special xri function with an immediate value
int Cpu::xri(){
    xra(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

//special ori function with an immediate value
int Cpu::ori(){
    ora(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

//special compare with an immediate value
int Cpu::cpi(){
    cmp(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

//toggles the carry bit
int Cpu::cmc(){
    flags.toggle(CARRY_BIT);
    registers.pc++;
    return 4;
}

//sets the carry bit
int Cpu::stc(){
    flags.setBits(CARRY_BIT);
    registers.pc++;
    return 4;
}

//MOV instruction, puts the value in the source register into the destination register
int Cpu::mov(uint8_t &destination, uint8_t source){
    destination = source;
    registers.pc++;
    return 5;
}

//mov function for moving values into the special "M Register"
int Cpu::movToM(uint8_t source){
    uint16_t offset = (registers.h << 8) | registers.l;
    memory[offset] = source;
    registers.pc++;
    return 7;
}

//mov function for moving value in the "M Register" into another register
int Cpu::movMTo(uint8_t &destination){
    uint16_t offset = (registers.h << 8) | registers.l;
    destination = memory[offset];
    registers.pc++;
    return 7;
}

//mov function for an immediate value
int Cpu::mvi(uint8_t &destination){
    destination = memory[registers.pc+1];
    registers.pc += 2;
    return 7;
}

//mvi function for the "M Register"
int Cpu::mviM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    memory[offset] = memory[registers.pc+1];
    registers.pc += 2;
    return 10;
}

//loads values from memory into two target registers
int Cpu::lxi(uint8_t &nameRegister, uint8_t &nextRegister){
    nextRegister = memory[registers.pc+1];
    nameRegister = memory[registers.pc+2];
    registers.pc += 3;
    return 10;
}

//loads values from memory into sp
int Cpu::lxiSP(){
    uint8_t low = memory[registers.pc+1];
    uint8_t high = memory[registers.pc+2];

    registers.sp = (high << 8) | low;
    registers.pc += 3;
    return 10;
}

//loads the value of memory set at the offset of two registers values into a
int Cpu::ldax(uint8_t nameRegister, uint8_t nextRegister){
    uint16_t offset = (nameRegister << 8) | nextRegister;
    registers.a = memory[offset];
    registers.pc++;
    return 7;
}

//stores value held in a into memory
int Cpu::stax(uint8_t nameRegister, uint8_t nextRegister){
    uint16_t offset = (nameRegister << 8) | nextRegister;
    memory[offset] = registers.a;
    registers.pc++;
    return 7;
}

//loads value from memory into registers l and h
int Cpu::lhld(){
    uint16_t address = (memory[registers.pc+2] << 8) | memory[registers.pc+1];
    registers.l = memory[address];
    registers.h = memory[address+1];
    registers.pc += 3;
    return 16;
}

//stores values held in registers l and h into memory
int Cpu::shld(){
    uint16_t address = (memory[registers.pc+2] << 8) | memory[registers.pc+1];
    memory[address] = registers.l;
    memory[address+1] = registers.h;
    registers.pc += 3;
    return 16;
}

//stores a register value into memory
int Cpu::sta(){
    uint16_t offset = (memory[registers.pc+2] << 8) | memory[registers.pc+1];
    memory[offset] = registers.a;
    registers.pc += 3;
    return 13;
}

//loads value from memory into a register
int Cpu::lda(){
    uint16_t offset = (memory[registers.pc+2] << 8) | memory[registers.pc+1];
    registers.a = memory[offset];
    registers.pc += 3;
    return 13;
}

//sets pc to the summation of registers h and l
int Cpu::pchl(){
    uint16_t value = (registers.h << 8) | registers.l;
    registers.pc = value;
    return 5;
}

//swaps the values in d with h, and the values in e with l registers
int Cpu::xchg(){
    uint16_t registerDE = (registers.d << 8) | registers.e;
    uint16_t registerHL = (registers.h << 8) | registers.l;

    registers.d = getHighBits(registerHL);
    registers.e = getLowBits(registerHL);
    registers.h = getHighBits(registerDE);
    registers.l = getLowBits(registerDE);
    registers.pc++;
    return 5;
}

//special instruction, enables interrupts for the 8080
int Cpu::ei(){
    enableInterrupts = true;
    registers.pc++;
    return 4;
}

//special instruction, disables interrupts for the 8080
int Cpu::di(){
    enableInterrupts = false;
    registers.pc++;
    return 4;
}

//special instruction
int Cpu::hlt(){
    exit(0);
    registers.pc++;
    return 4;
}

//special instruction, just for padding
int Cpu::nop(){
    registers.pc++;
    return 4;
}

//jmp, call, ret, and push/pop

// jumps the pc to the specified address
int Cpu::jmp(){
    uint8_t low = memory[registers.pc+1];
    uint8_t high = memory[registers.pc+2];

    uint16_t address = (high << 8) | low;
    registers.pc = address;
    return 10;
}

// jumps depending on whether certain flags are set
int Cpu::conditionalJmp(bool condition){
    if(condition){
        jmp();
    }
    else{
        registers.pc += 3;
    }
    return 10;
}

// pushes current PC location onto stack, and moves PC to specified call address
int Cpu::call(){
    uint16_t returnPC = registers.pc + 3;
    memory[registers.sp-1] = getHighBits(returnPC);
    memory[registers.sp-2] = getLowBits(returnPC);
    registers.sp -= 2;

    uint16_t value = (memory[registers.pc+2] << 8) | memory[registers.pc+1];
    registers.pc = value;
    return 17;
}

// CALL depending on whether certain flags are set
int Cpu::conditionalCall(bool condition){
    if(condition){
        call();
        return 17;
    }
    else{
        registers.pc += 3;
        return 11;
    }
}

// CALL to specific reset address depending on the RST opcode
int Cpu::rst(uint8_t resetNumber){
    uint8_t low = getLowBits(registers.pc);
    uint8_t high = getHighBits(registers.pc);

    memory[registers.sp-1] = high;
    memory[registers.sp-2] = low;
    registers.sp -= 2;

    uint16_t reset = resetNumber << 3;
    registers.pc = reset;

    return 11;
}

// Pops the 16 bit return address off top of stack, move PC to that return address
int Cpu::ret(){
    uint16_t value = (memory[registers.sp+1] << 8) | memory[registers.sp];
    registers.pc = value;
    registers.sp += 2;

    return 10;
}

// RET depending on whether certain flags are set
int Cpu::conditionalRet(bool condition){
    if(condition){
        ret();
        return 11;
    }
    else{
        registers.pc++;
        return 5;
    }
}

// Push a 16 bit int (from 2 registers) onto the stack
int Cpu::push(uint8_t nameRegister, uint8_t nextRegister){
    memory[registers.sp-1] = nameRegister;
    memory[registers.sp-2] = nextRegister;
    registers.sp -= 2;
    registers.pc++;
    return 11;
}

// PUSH a 16 bit value PSW, representing the A register combined with the state of all flags, onto the stack
int Cpu::pushPSW(){
    memory[registers.sp-1] = registers.a;
    memory[registers.sp-2] = flags.getRegisterValue();
    registers.sp -= 2;
    registers.pc++;
    return 11;
}

// Pop the 16 bit int off the stack into 2 registers
int Cpu::pop(uint8_t &nameRegister, uint8_t &nextRegister){
    nextRegister = memory[registers.sp];
    nameRegister = memory[registers.sp+1];
    registers.sp += 2;
    registers.pc++;
    return 10;
}

// POP the 16 bit int off the stack, updating the flags and the A register
int Cpu::popPSW(){
    flags = Flags(memory[registers.sp]);
    registers.a = memory[registers.sp+1];

    registers.sp += 2;
    registers.pc++;
    return 10;
}

// Set the stack pointer to the memory address stored in the combined HL register
int Cpu::sphl(){
    registers.sp = (registers.h << 8) | registers.l;
    registers.pc++;
    return 5;
}

// Exchange the 16 bit value stored in the combined HL register with the 16 bit value at the top of the stack
int Cpu::xthl(){
    uint8_t tempH = registers.h;
    uint8_t tempL = registers.l;

    registers.l = memory[registers.sp];
    registers.h = memory[registers.sp+1];

    memory[registers.sp] = tempL;
    memory[registers.sp+1] = tempH;

    registers.pc++;
    return 18;
}

//arithmetic functions

// Increments 16 bit int from 2 registers by 1
int Cpu::inx(uint8_t &nameRegister, uint8_t &nextRegister){
    uint16_t value = (nameRegister << 8) | nextRegister;
    value++;
    nameRegister = getHighBits(value);
    nextRegister = getLowBits(value);

    registers.pc++;
    return 5;
}

// Increments SP and PC
int Cpu::inxSP(){
    registers.sp++;
    registers.pc++;
    return 5;
}

// Decrements 16 bit int from 2 registers by 1
int Cpu::dcx(uint8_t &nameRegister, uint8_t &nextRegister){
    uint16_t value = (nameRegister << 8) | nextRegister;
    value--;
    nameRegister = getHighBits(value);
    nextRegister = getLowBits(value);

    registers.pc++;
    return 5;
}

// Decrements SP and PC
int Cpu::dcxSP(){
    registers.sp--;
    registers.pc++;
    return 5;
}

// Increments a register by 1, sets flags
int Cpu::inr(uint8_t &regName){
    Flags calcFlags(SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
    regName = addBytes(regName, 1, false, calcFlags);
    registers.pc++;
    return 5;
}

// INR on the M register
int Cpu::inrM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    return inr(memory[offset]);

}

// Decrements a register by 1, sets flags
int Cpu::dcr(uint8_t &regName){
    regName = addBytes(regName, -1, false, SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
    registers.pc++;
    return 5;
}

// DCR on the M register
int Cpu::dcrM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    return dcr(memory[offset]);
}

// Adds two 16 bit ints together (2 register + 2 register), sets carry flag
int Cpu::dad(uint8_t nameRegister, uint8_t nextRegister){
    uint16_t hl = (registers.h << 8) | registers.l;
    uint16_t value = (nameRegister << 8) | nextRegister;
    uint32_t sum = hl + value;
    if(sum & 0x10000){
        flags.setBits(CARRY_BIT);
    }
    registers.l = getLowBits((uint16_t)sum);
    registers.h = getHighBits((uint16_t)sum);

    registers.pc++;
    return 10;
}

// Adds SP to the 16 bit int stored in HL
int Cpu::dadSP(){
    uint16_t hl = (registers.h << 8) | registers.l;
    uint32_t result = registers.sp + hl;
    if(result & 0x10000){
        flags.setBits(CARRY_BIT);
    }
    registers.l = getLowBits((uint16_t)result);
    registers.h = getHighBits((uint16_t)result);

    registers.pc++;
    return 10;
}

// Adds 1 register to the A register
int Cpu::add(uint8_t regName){
    Flags calcFlags(CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);

    registers.a = addBytes(registers.a, regName, false, calcFlags);
    registers.pc++;
    return 4;
}

// ADD on the M register
int Cpu::addM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    add(memory[offset]);
    return 7;
}

// Adds 1 register and the carry flag to the A register
int Cpu::adc(uint8_t regName){
    Flags calcFlags(CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);

    registers.a = addBytes(registers.a, regName, true, calcFlags);
    registers.pc++;
    return 4;
}

// ADC with the M register
int Cpu::adcM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    adc(memory[offset]);
    return 7;
}

// Subtracts 1 register from the A register
int Cpu::sub(uint8_t regName){
    Flags calcFlags(CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
    uint8_t operandToCompare = (regName ^ 0xFF) + 1;
    registers.a = addBytes(registers.a, operandToCompare, false, calcFlags);
    flags.setBits(CARRY_BIT, !flags.testBits(CARRY_BIT));
    registers.pc++;
    return 4;
}

// SUB with the M register
int Cpu::subM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    sub(memory[offset]);
    registers.pc++;
    return 7;
}

// Subtracts 1 register and the carry flag from the A register
int Cpu::sbb(uint8_t regName){
    Flags calcFlags(CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
    regName = regName + flags.testBits(CARRY_BIT);
    uint8_t operandToCompare = (regName ^ 0xFF) + 1;
    registers.a = addBytes(registers.a, operandToCompare, false, calcFlags);
    flags.setBits(CARRY_BIT, !flags.testBits(CARRY_BIT));
    registers.pc++;
    return 4;
}

// SBB with the M register
int Cpu::sbbM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    sbb(memory[offset]);
    return 7;
}

// Adds an 8 bit int stored in memory to the A register
int Cpu::adi(){
    add(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

// Subtracts an 8 bit int stored in memory from the A register
int Cpu::sui(){
    sub(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

// Adds an 8 bit int stored in memory and the carry flag to the A register
int Cpu::aci(){
    adc(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

// Subtracts an 8 bit int stored in memory and the carry flag from the A register
int Cpu::sbi(){
    sbb(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

// special instruction to read from machine into A register, handles input
int Cpu::in(){
    uint8_t inputValue = memory[registers.pc+1];
    switch(inputValue){
    case 1:
        registers.a = input1;
        break;
    case 2:
        registers.a = input2;
        break;
    case 3:
        registers.a = input3;
        break;
    }
    registers.pc += 2;
    return 10;
}

// special instruction to write to machine from A register for output
int Cpu::out(){
    uint8_t outputValue = memory[registers.pc+1];
    switch(outputValue){
    case 2:                                 //set bit shift amount
        output2 = registers.a;
        break;
    case 3:                                 //play sounds
        output3 = registers.a;
        emit writeOnPort3(output3);         //emits signal that there was a write on port 3
        break;
    case 4:                                 //bit shift data in A register based on bit shift amount
    {
        output4 = registers.a;
        uint8_t offset = output2 & 7;
        uint16_t shiftReg = 0;
        shiftReg >> 8;
        uint16_t shiftIn = output4 << 8;
        shiftReg = shiftReg | shiftIn;

        uint16_t bitMask = 0xFF00 >> offset;
        input3 = (shiftReg & bitMask) >> (8 - offset);
        break;
    }
    case 5:                                 //play sounds
        output5 = registers.a;
        emit writeOnPort5(output5);         //emits signal that there was a write on port 5
        break;
    case 6:                                 //resets watchdog circuit
        output6 = registers.a;
        break;
    }
    registers.pc += 2;
    return 10;
}

// Special instruction, only one to use aux carry flag
int Cpu::daa(){
    // increment lower nibble of A register by 6 if conditions met
    // set aux flag based on result of the increment
    uint8_t lowNibble = getLowBits(registers.a);
    if(lowNibble > 9 || flags.testBits(AUX_BIT)){
        Flags calcFlags(AUX_BIT);
        registers.a = addBytes(registers.a, 6, false, calcFlags);
    }
    // increment higher nibble of A register by 6 if conditions met
    // set carry flag based on result
    if(getHighBits(registers.a) > 9 || flags.testBits(CARRY_BIT)){
        bool oldValue = flags.testBits(CARRY_BIT);

        Flags calcFlags(SIGN_BIT | ZERO_BIT | PARITY_BIT | CARRY_BIT);
        registers.a = addBytes(registers.a, 96, false, calcFlags);

        if(!flags.testBits(CARRY_BIT)){
            flags.setBits(CARRY_BIT, oldValue);
        }
    }
    registers.pc++;
    return 4;
}

// helper function called to emulate any 8080 binary opcode
int Cpu::emulateInstruction(){
    return getInstruction(memory[registers.pc]);
}

// Generates a video interrupt (either RST 0 or RST 1 opcodes)
bool Cpu::generateInterrupt(uint8_t opCode){
    bool success = false;
    if(enableInterrupts){
        enableInterrupts = false;
        getInstruction(opCode);
        success = true;
    }
    return success;
}

// Reads binary opcode and calls corresponding function to handle opcode
int Cpu::getInstruction(uint8_t operation){
    switch(operation){
        case 0x00:
            return nop();
        case 0x01:
            return lxi(registers.b, registers.c);
        case 0x02:
            return stax(registers.b, registers.c);
        case 0x03:
            return inx(registers.b, registers.c);
        case 0x04:
            return inr(registers.b);
        case 0x05:
            return dcr(registers.b);
        case 0x06:
            return mvi(registers.b);
        case 0x07:
            return rlc();
        case 0x08:
            return nop();
        case 0x09:
            return dad(registers.b, registers.c);
        case 0x0A:
            return ldax(registers.b, registers.c);
        case 0x0B:
            return dcx(registers.b, registers.c);
        case 0x0C:
            return inr(registers.c);
        case 0x0D:
            return dcr(registers.c);
        case 0x0E:
            return mvi(registers.c);
        case 0x0F:
            return rrc();
        case 0x10:
            return nop();
        case 0x11:
            return lxi(registers.d, registers.e);
        case 0x12:
            return stax(registers.d, registers.e);
        case 0x13:
            return inx(registers.d, registers.e);
        case 0x14:
            return inr(registers.d);
        case 0x15:
            return dcr(registers.d);
        case 0x16:
            return mvi(registers.d);
        case 0x17:
            return ral();
        case 0x18:
            return nop();
        case 0x19:
            return dad(registers.d, registers.e);
        case 0x1A:
            return ldax(registers.d, registers.e);
        case 0x1B:
            return dcx(registers.d, registers.e);
        case 0x1C:
            return inr(registers.e);
        case 0x1D:
            return dcr(registers.e);
        case 0x1E:
            return mvi(registers.c);
        case 0x1F:
            return rar();
        case 0x20:
            return nop();
        case 0x21:
            return lxi(registers.h, registers.l);
        case 0x22:
            return shld();
        case 0x23:
            return inx(registers.h, registers.l);
        case 0x24:
            return inr(registers.h);
        case 0x25:
            return dcr(registers.h);
        case 0x26:
            return mvi(registers.h);
        case 0x27:
            return daa();
        case 0x28:
            return nop();
        case 0x29:
            return dad(registers.h, registers.l);
        case 0x2A:
            return lhld();
        case 0x2B:
            return dcx(registers.h, registers.l);
        case 0x2C:
            return inr(registers.l);
        case 0x2D:
            return dcr(registers.l);
        case 0x2E:
            return mvi(registers.l);
        case 0x2F:
            return cma();
        case 0x30:
            return nop();
        case 0x31:
            return lxiSP();
        case 0x32:
            return sta();
        case 0x33:
            return inxSP();
        case 0x34:
            return inrM();
        case 0x35:
            return dcrM();
        case 0x36:
            return mviM();
        case 0x37:
            return stc();
        case 0x38:
            return nop();
        case 0x39:
            return dadSP();
        case 0x3A:
            return lda();
        case 0x3B:
            return dcxSP();
        case 0x3C:
            return inr(registers.a);
        case 0x3D:
            return dcr(registers.a);
        case 0x3E:
            return mvi(registers.a);
        case 0x3F:
            return cmc();
        case 0x40:
            return mov(registers.b, registers.b);
        case 0x41:
            return mov(registers.b, registers.c);
        case 0x42:
            return mov(registers.b, registers.d);
        case 0x43:
            return mov(registers.b, registers.e);
        case 0x44:
            return mov(registers.b, registers.h);
        case 0x45:
            return mov(registers.b, registers.l);
        case 0x46:
            return movMTo(registers.b);
        case 0x47:
            return mov(registers.b, registers.a);
        case 0x48:
            return mov(registers.c, registers.b);
        case 0x49:
            return mov(registers.c, registers.c);
        case 0x4A:
            return mov(registers.c, registers.d);
        case 0x4B:
            return mov(registers.c, registers.e);
        case 0x4C:
            return mov(registers.c, registers.h);
        case 0x4D:
            return mov(registers.c, registers.l);
        case 0x4E:
            return movMTo(registers.c);
        case 0x4F:
            return mov(registers.c, registers.a);
        case 0x50:
            return mov(registers.d, registers.b);
        case 0x51:
            return mov(registers.d, registers.c);
        case 0x52:
            return mov(registers.d, registers.d);
        case 0x53:
            return mov(registers.d, registers.e);
        case 0x54:
            return mov(registers.d, registers.h);
        case 0x55:
            return mov(registers.d, registers.l);
        case 0x56:
            return movMTo(registers.d);
        case 0x57:
            return mov(registers.d, registers.a);
        case 0x58:
            return mov(registers.e, registers.b);
        case 0x59:
            return mov(registers.e, registers.c);
        case 0x5A:
            return mov(registers.e, registers.d);
        case 0x5B:
            return mov(registers.e, registers.e);
        case 0x5C:
            return mov(registers.e, registers.h);
        case 0x5D:
            return mov(registers.e, registers.l);
        case 0x5E:
            return movMTo(registers.e);
        case 0x5F:
            return mov(registers.e, registers.a);
        case 0x60:
            return mov(registers.h, registers.b);
        case 0x61:
            return mov(registers.h, registers.c);
        case 0x62:
            return mov(registers.h, registers.d);
        case 0x63:
            return mov(registers.h, registers.e);
        case 0x64:
            return mov(registers.h, registers.h);
        case 0x65:
            return mov(registers.h, registers.l);
        case 0x66:
            return movMTo(registers.h);
        case 0x67:
            return mov(registers.h, registers.a);
        case 0x68:
            return mov(registers.l, registers.b);
        case 0x69:
            return mov(registers.l, registers.c);
        case 0x6A:
            return mov(registers.l, registers.d);
        case 0x6B:
            return mov(registers.l, registers.e);
        case 0x6C:
            return mov(registers.l, registers.h);
        case 0x6D:
            return mov(registers.l, registers.l);
        case 0x6E:
            return movMTo(registers.l);
        case 0x6F:
            return mov(registers.l, registers.a);
        case 0x70:
            return movToM(registers.b);
        case 0x71:
            return movToM(registers.c);
        case 0x72:
            return movToM(registers.d);
        case 0x73:
            return movToM(registers.e);
        case 0x74:
            return movToM(registers.h);
        case 0x75:
            return movToM(registers.l);
        case 0x76:
            return hlt();
        case 0x77:
            return movToM(registers.a);
        case 0x78:
            return mov(registers.a, registers.b);
        case 0x79:
            return mov(registers.a, registers.c);
        case 0x7A:
            return mov(registers.a, registers.d);
        case 0x7B:
            return mov(registers.a, registers.e);
        case 0x7C:
            return mov(registers.a, registers.h);
        case 0x7D:
            return mov(registers.a, registers.l);
        case 0x7E:
            return movMTo(registers.a);
        case 0x7F:
            return mov(registers.a, registers.a);
        case 0x80:
            return add(registers.b);
        case 0x81:
            return add(registers.c);
        case 0x82:
            return add(registers.d);
        case 0x83:
            return add(registers.e);
        case 0x84:
            return add(registers.h);
        case 0x85:
            return add(registers.l);
        case 0x86:
            return addM();
        case 0x87:
            return add(registers.a);
        case 0x88:
            return adc(registers.b);
        case 0x89:
            return adc(registers.c);
        case 0x8A:
            return adc(registers.d);
        case 0x8B:
            return adc(registers.e);
        case 0x8C:
            return adc(registers.h);
        case 0x8D:
            return adc(registers.l);
        case 0x8E:
            return adcM();
        case 0x8F:
            return adc(registers.a);
        case 0x90:
            return sub(registers.b);
        case 0x91:
            return sub(registers.c);
        case 0x92:
            return sub(registers.d);
        case 0x93:
            return sub(registers.e);
        case 0x94:
            return sub(registers.h);
        case 0x95:
            return sub(registers.l);
        case 0x96:
            return subM();
        case 0x97:
            return sub(registers.a);
        case 0x98:
            return sbb(registers.b);
        case 0x99:
            return sbb(registers.c);
        case 0x9A:
            return sbb(registers.d);
        case 0x9B:
            return sbb(registers.e);
        case 0x9C:
            return sbb(registers.h);
        case 0x9D:
            return sbb(registers.l);
        case 0x9E:
            return sbbM();
        case 0x9F:
            return sbb(registers.a);
        case 0xA0:
            return ana(registers.b);
        case 0xA1:
            return ana(registers.c);
        case 0xA2:
            return ana(registers.d);
        case 0xA3:
            return ana(registers.e);
        case 0xA4:
            return ana(registers.h);
        case 0xA5:
            return ana(registers.l);
        case 0xA6:
            return anaM();
        case 0xA7:
            return ana(registers.a);
        case 0xA8:
            return xra(registers.b);
        case 0xA9:
            return xra(registers.c);
        case 0xAA:
            return xra(registers.d);
        case 0xAB:
            return xra(registers.e);
        case 0xAC:
            return xra(registers.h);
        case 0xAD:
            return xra(registers.l);
        case 0xAE:
            return xraM();
        case 0xAF:
            return xra(registers.a);
        case 0xB0:
            return ora(registers.b);
        case 0xB1:
            return ora(registers.c);
        case 0xB2:
            return ora(registers.d);
        case 0xB3:
            return ora(registers.e);
        case 0xB4:
            return ora(registers.h);
        case 0xB5:
            return ora(registers.l);
        case 0xB6:
            return oraM();
        case 0xB7:
            return ora(registers.a);
        case 0xB8:
            return cmp(registers.b);
        case 0xB9:
            return cmp(registers.c);
        case 0xBA:
            return cmp(registers.d);
        case 0xBB:
            return cmp(registers.e);
        case 0xBC:
            return cmp(registers.h);
        case 0xBD:
            return cmp(registers.l);
        case 0xBE:
            return cmpM();
        case 0xBF:
            return cmp(registers.a);
        case 0xC0:
            return conditionalRet(!flags.testBits(ZERO_BIT));
        case 0xC1:
            return pop(registers.b, registers.c);
        case 0xC2:
            return conditionalJmp(!flags.testBits(ZERO_BIT));
        case 0xC3:
            return jmp();
        case 0xC4:
            return conditionalCall(!flags.testBits(ZERO_BIT));
        case 0xC5:
            return push(registers.b, registers.c);
        case 0xC6:
            return adi();
        case 0xC7:
            return rst(0);
        case 0xC8:
            return conditionalRet(flags.testBits(ZERO_BIT));
        case 0xC9:
            return ret();
        case 0xCA:
            return conditionalJmp(flags.testBits(ZERO_BIT));
        case 0xCB:
            return jmp();
        case 0xCC:
            return conditionalCall(flags.testBits(ZERO_BIT));
        case 0xCD:
            return call();
        case 0xCE:
            return aci();
        case 0xCF:
            return rst(1);
        case 0xD0:
            return conditionalRet(!flags.testBits(CARRY_BIT));
        case 0xD1:
            return pop(registers.d, registers.e);
        case 0xD2:
            return conditionalJmp(!flags.testBits(CARRY_BIT));
        case 0xD3:
            return out();
        case 0xD4:
            return conditionalCall(!flags.testBits(CARRY_BIT));
        case 0xD5:
            return push(registers.d, registers.e);
        case 0xD6:
            return sui();
        case 0xD7:
            return rst(2);
        case 0xD8:
            return conditionalRet(flags.testBits(CARRY_BIT));
        case 0xD9:
            return ret();
        case 0xDA:
            return conditionalJmp(flags.testBits(CARRY_BIT));
        case 0xDB:
            return in();
        case 0xDC:
            return conditionalCall(flags.testBits(CARRY_BIT));
        case 0xDD:
            return call();
        case 0xDE:
            return sbi();
        case 0xDF:
            return rst(3);
        case 0xE0:
            return conditionalRet(!flags.testBits(PARITY_BIT));
        case 0xE1:
            return pop(registers.h, registers.l);
        case 0xE2:
            return conditionalJmp(!flags.testBits(PARITY_BIT));
        case 0xE3:
            return xthl();
        case 0xE4:
            return conditionalCall(!flags.testBits(PARITY_BIT));
        case 0xE5:
            return push(registers.h, registers.l);
        case 0xE6:
            return ani();
        case 0xE7:
            return rst(4);
        case 0xE8:
            return conditionalRet(flags.testBits(PARITY_BIT));
        case 0xE9:
            return pchl();
        case 0xEA:
            return conditionalJmp(flags.testBits(PARITY_BIT));
        case 0xEB:
            return xchg();
        case 0xEC:
            return conditionalCall(flags.testBits(PARITY_BIT));
        case 0xED:
            return call();
        case 0xEE:
            return xri();
        case 0xEF:
            return rst(5);
        case 0xF0:
            return conditionalRet(!flags.testBits(SIGN_BIT));
        case 0xF1:
            return popPSW();
        case 0xF2:
            return conditionalJmp(!flags.testBits(SIGN_BIT));
        case 0xF3:
            return di();
        case 0xF4:
            return conditionalCall(!flags.testBits(SIGN_BIT));
        case 0xF5:
            return pushPSW();
        case 0xF6:
            return ori();
        case 0xF7:
            return rst(6);
        case 0xF8:
            return conditionalRet(flags.testBits(SIGN_BIT));
        case 0xF9:
            return sphl();
        case 0xFA:
            return conditionalJmp(flags.testBits(SIGN_BIT));
        case 0xFB:
            return ei();
        case 0xFC:
            return conditionalCall(flags.testBits(SIGN_BIT));
        case 0xFD:
            return call();
        case 0xFE:
            return cpi();
        case 0xFF:
            return rst(7);
    }
    return 0;
}


