#include "cpu.h"

#include <QtGlobal>

Cpu::Cpu()
{
    //allocate memory for the registers and condition codes struct
    memset(&registers, 0, sizeof(State8080Registers));


    //initializing input values
    input0 = 0b01110000;
    input1 = 0b00010000;
    input2 = 0;
    input3 = 0;

    //initialize output values
    output2 = 0;
    output3 = 0;
    output4 = 0;
    output5 = 0;
    output6 = 0;

    enableInterrupts = false;

   memory = new uint8_t[0x4000];
}

uint8_t Cpu::getLowBits(uint8_t value){
    return (value & 0x0F);
}

uint8_t Cpu::getLowBits(uint16_t value){
    return value & 0x00FF;
}

uint8_t Cpu::getHighBits(uint8_t value){
    return (value & 0xF0) >> 4;
}

uint8_t Cpu::getHighBits(uint16_t value){
    return (value & 0xFF00) >> 8;
}



int Cpu::addBytes(uint8_t byte, uint8_t byte2, bool carryIn, Flags flagVar){
    uint8_t carry = carryIn ? flags.testBits(CARRY_BIT) : 0;

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

int Cpu::ana(uint8_t secondRegister){
    registers.a = registers.a & secondRegister;
    flags.setBits(CARRY_BIT, false);
    flags.calcAllBits(registers.a);
    registers.pc++;
    return 4;
}

int Cpu::anaM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    ana(memory[offset]);
    return 7;
}

int Cpu::xra(uint8_t secondRegister){
    registers.a = registers.a ^ secondRegister;
    flags.setBits(CARRY_BIT, false);
    flags.calcAllBits(registers.a);
    registers.pc++;
    return 4;
}

int Cpu::xraM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    xra(memory[offset]);
    return 7;
}

int Cpu::ora(uint8_t secondRegister){
    registers.a = registers.a | secondRegister;
    flags.setBits(CARRY_BIT, false);
    flags.calcAllBits(registers.a);
    registers.pc++;
    return 4;
}

int Cpu::oraM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    ora(memory[offset]);
    return 7;
}

int Cpu::cmp(uint8_t secondRegister){
    Flags calc(SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT | CARRY_BIT);
    addBytes(registers.a, -secondRegister, false, calc);

    flags.setBits(CARRY_BIT, !flags.testBits(CARRY_BIT));
    registers.pc++;
    return 4;
}

int Cpu::cmpM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    cmp(memory[offset]);
    return 7;
}

int Cpu::rlc(){
    uint8_t value = (registers.a & 0x80) >> 7;
    flags.setBits(CARRY_BIT, value);
    registers.a = registers.a << 1;
    registers.a = registers.a | value;
    registers.pc++;
    return 4;
}

int Cpu::rrc(){
    uint8_t value = (registers.a & 0x01);
    flags.setBits(CARRY_BIT, value);
    registers.a = registers.a >> 1;
    registers.a = registers.a | (value << 7);
    registers.pc++;
    return 4;
}

int Cpu::ral(){
    uint8_t newValue = (registers.a & 0x80) >> 7;
    uint8_t oldValue = flags.testBits(CARRY_BIT);
    flags.setBits(CARRY_BIT, newValue);
    registers.a = registers.a << 1;
    registers.a = registers.a | oldValue;
    registers.pc++;
    return 4;
}

int Cpu::rar(){
    uint8_t newValue = (registers.a & 0x01);
    uint8_t oldValue = flags.testBits(CARRY_BIT);
    flags.setBits(CARRY_BIT, newValue);
    registers.a = registers.a >> 1;
    registers.a = registers.a | (oldValue << 7);
    registers.pc++;
    return 4;
}

int Cpu::ani(){
    ana(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

int Cpu::cma(){
    registers.a = registers.a ^ 0xFF;
    registers.pc++;
    return 4;
}

int Cpu::xri(){
    xra(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

int Cpu::ori(){
    ora(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

int Cpu::cpi(){
    cmp(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

int Cpu::cmc(){
    flags.toggle(CARRY_BIT);
    registers.pc++;
    return 4;
}

int Cpu::stc(){
    flags.setBits(CARRY_BIT);
    registers.pc++;
    return 4;
}

int Cpu::mov(uint8_t &destination, uint8_t source){
    destination = source;
    registers.pc++;
    return 5;
}

int Cpu::movToM(uint8_t source){
    uint16_t offset = (registers.h << 8) | registers.l;
    memory[offset] = source;
    registers.pc++;
    return 7;
}

int Cpu::movMTo(uint8_t &destination){
    uint16_t offset = (registers.h << 8) | registers.l;
    destination = memory[offset];
    registers.pc++;
    return 7;
}

int Cpu::mvi(uint8_t &destination){
    destination = memory[registers.pc+1];
    registers.pc += 2;
    return 7;
}

int Cpu::mviM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    memory[offset] = memory[registers.pc+1];
    registers.pc += 2;
    return 10;
}

int Cpu::lxi(uint8_t &nameRegister, uint8_t &nextRegister){
    nextRegister = memory[registers.pc+1];
    nameRegister = memory[registers.pc+2];
    registers.pc += 3;
    return 10;
}

int Cpu::lxiSP(){
    uint8_t low = memory[registers.pc+1];
    uint8_t high = memory[registers.pc+2];

    registers.sp = (high << 8) | low;
    registers.pc += 3;
    return 10;
}

int Cpu::ldax(uint8_t nameRegister, uint8_t nextRegister){
    uint16_t offset = (nameRegister << 8) | nextRegister;
    registers.a = memory[offset];
    registers.pc++;
    return 7;
}

int Cpu::stax(uint8_t nameRegister, uint8_t nextRegister){
    uint16_t offset = (nameRegister << 8) | nextRegister;
    memory[offset] = registers.a;
    registers.pc++;
    return 7;
}

int Cpu::lhld(){
    uint16_t address = (memory[registers.pc+2] << 8) | memory[registers.pc+1];
    registers.l = memory[address];
    registers.h = memory[address+1];
    registers.pc += 3;
    return 16;
}

int Cpu::shld(){
    uint16_t address = (memory[registers.pc+2] << 8) | memory[registers.pc+1];
    memory[address] = registers.l;
    memory[address+1] = registers.h;
    registers.pc += 3;
    return 16;
}

int Cpu::sta(){
    uint16_t offset = (memory[registers.pc+2] << 8) | memory[registers.pc+1];
    memory[offset] = registers.a;
    registers.pc += 3;
    return 13;
}

int Cpu::lda(){
    uint16_t offset = (memory[registers.pc+2] << 8) | memory[registers.pc+1];
    registers.a = memory[offset];
    registers.pc += 3;
    return 13;
}

int Cpu::pchl(){
    uint16_t value = (registers.h << 8) | registers.l;
    registers.pc = value;
    return 5;
}

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

int Cpu::ei(){
    enableInterrupts = true;
    registers.pc++;
    return 4;
}

int Cpu::di(){
    enableInterrupts = false;
    registers.pc++;
    return 4;
}

int Cpu::hlt(){
    exit(0);
    registers.pc++;
    return 4;
}

int Cpu::nop(){
    registers.pc++;
    return 4;
}

//jmp, call, ret, and push/pop

int Cpu::jmp(){
    uint8_t low = memory[registers.pc+1];
    uint8_t high = memory[registers.pc+2];

    uint16_t address = (high << 8) | low;
    registers.pc = address;
    return 10;
}

int Cpu::conditionalJmp(bool condition){
    if(condition){
        jmp();
    }
    else{
        registers.pc += 3;
    }
    return 10;
}

int Cpu::call(){
    uint16_t returnPC = registers.pc + 3;
    memory[registers.sp-1] = getHighBits(returnPC);
    memory[registers.sp-2] = getLowBits(returnPC);
    registers.sp -= 2;

    uint16_t value = (memory[registers.pc+2] << 8) | memory[registers.pc+1];
    registers.pc = value;
    return 17;
}

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

int Cpu::ret(){
    uint16_t value = (memory[registers.sp+1] << 8) | memory[registers.sp];
    registers.pc = value;
    registers.sp += 2;

    return 10;
}

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

int Cpu::push(uint8_t nameRegister, uint8_t nextRegister){
    memory[registers.sp-1] = nameRegister;
    memory[registers.sp-2] = nextRegister;
    registers.sp -= 2;
    registers.pc++;
    return 11;
}

int Cpu::pushPSW(){
    memory[registers.sp-1] = registers.a;
    memory[registers.sp-2] = flags.getRegisterValue();
    registers.sp -= 2;
    registers.pc++;
    return 11;
}

int Cpu::pop(uint8_t &nameRegister, uint8_t &nextRegister){
    nextRegister = memory[registers.sp];
    nameRegister = memory[registers.sp+1];
    registers.sp += 2;
    registers.pc++;
    return 10;
}

int Cpu::popPSW(){
    flags = Flags(memory[registers.sp]);
    registers.a = memory[registers.sp+1];

    registers.sp += 2;
    registers.pc++;
    return 10;
}

int Cpu::sphl(){
    registers.sp = (registers.h << 8) | registers.l;
    registers.pc++;
    return 5;
}

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

int Cpu::inx(uint8_t &nameRegister, uint8_t &nextRegister){
    uint16_t value = (nameRegister << 8) | nextRegister;
    value++;
    nameRegister = getHighBits(value);
    nextRegister = getLowBits(value);

    registers.pc++;
    return 5;
}

int Cpu::inxSP(){
    registers.sp++;
    registers.pc++;
    return 5;
}

int Cpu::dcx(uint8_t &nameRegister, uint8_t &nextRegister){
    uint16_t value = (nameRegister << 8) | nextRegister;
    value--;
    nameRegister = getHighBits(value);
    nextRegister = getLowBits(value);

    registers.pc++;
    return 5;
}

int Cpu::dcxSP(){
    registers.sp--;
    registers.pc++;
    return 5;
}

int Cpu::inr(uint8_t &regName){
    Flags calcFlags(SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
    regName = addBytes(regName, 1, false, calcFlags);
    registers.pc++;
    return 5;
}

int Cpu::inrM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    return inr(memory[offset]);

}

int Cpu::dcr(uint8_t &regName){
    regName = addBytes(regName, -1, false, SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
    registers.pc++;
    return 5;
}

int Cpu::dcrM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    return dcr(memory[offset]);
}

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

int Cpu::add(uint8_t regName){
    Flags calcFlags(CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);

    registers.a = addBytes(registers.a, regName, false, calcFlags);
    registers.pc++;
    return 4;
}

int Cpu::addM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    add(memory[offset]);
    return 7;
}

int Cpu::adc(uint8_t regName){
    Flags calcFlags(CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);

    registers.a = addBytes(registers.a, regName, true, calcFlags);
    registers.pc++;
    return 4;
}

int Cpu::adcM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    adc(memory[offset]);
    return 7;
}

int Cpu::sub(uint8_t regName){
    Flags calcFlags(CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
    uint8_t operandToCompare = (regName ^ 0xFF) + 1;
    registers.a = addBytes(registers.a, operandToCompare, false, calcFlags);
    flags.setBits(CARRY_BIT, !flags.testBits(CARRY_BIT));
    registers.pc++;
    return 4;
}

int Cpu::subM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    sub(memory[offset]);
    registers.pc++;
    return 7;
}

int Cpu::sbb(uint8_t regName){
    Flags calcFlags(CARRY_BIT | SIGN_BIT | ZERO_BIT | PARITY_BIT | AUX_BIT);
    regName = regName + flags.testBits(CARRY_BIT);
    uint8_t operandToCompare = (regName ^ 0xFF) + 1;
    registers.a = addBytes(registers.a, operandToCompare, false, calcFlags);
    flags.setBits(CARRY_BIT, !flags.testBits(CARRY_BIT));
    registers.pc++;
    return 4;
}

int Cpu::sbbM(){
    uint16_t offset = (registers.h << 8) | registers.l;
    sbb(memory[offset]);
    return 7;
}

int Cpu::adi(){
    add(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

int Cpu::sui(){
    sub(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

int Cpu::aci(){
    adc(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

int Cpu::sbi(){
    sbb(memory[registers.pc+1]);
    registers.pc++;
    return 7;
}

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

int Cpu::out(){
    uint8_t outputValue = memory[registers.pc+1];
    switch(outputValue){
    case 2:
        output2 = registers.a;
        break;
    case 3:
        output3 = registers.a;
        emit writeOnPort3(output3);
        break;
    case 4:
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
    case 5:
        output5 = registers.a;
        emit writeOnPort5(output5);
        break;
    case 6:
        output6 = registers.a;
        break;
    }
    registers.pc += 2;
    return 10;
}


int Cpu::daa(){
    uint8_t lowNibble = getLowBits(registers.a);
    if(lowNibble > 9 || flags.testBits(AUX_BIT)){
        Flags calcFlags(AUX_BIT);
        registers.a = addBytes(registers.a, 6, false, calcFlags);
    }
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

int Cpu::emulateInstruction(){
    return getInstruction(memory[registers.pc]);
}


bool Cpu::generateInterrupt(uint8_t opCode){
    bool success = false;
    if(enableInterrupts){
        enableInterrupts = false;
        getInstruction(opCode);
        success = true;
    }
    return success;
}

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









