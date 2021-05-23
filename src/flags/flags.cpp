#include "flags.h"

Flags::Flags()
{
    conditionBits = EMPTY_FLAG;
}

Flags::Flags(uint8_t bits){
    conditionBits = EMPTY_FLAG | bits;
}

void Flags::setBits(uint8_t bits){
    conditionBits = conditionBits | bits;
}

void Flags::setBits(uint8_t bits, bool value){
    value ? setBits(bits) : clearBits(bits);
}

void Flags::clearBits(uint8_t bits){
    conditionBits = conditionBits & (bits ^ 0xFF);
}

void Flags::toggle(uint8_t bits){
    conditionBits = conditionBits ^ bits;
}

bool Flags::testBits(uint8_t bits){
    uint8_t results = conditionBits & bits;
    return results == bits;
}

uint8_t Flags::parity(uint8_t val){
    uint8_t count = 0;
    while(val){
        if((val & 1) == 1){
            count++;
        }
        val >>= 1;
    }
    return count;
}

void Flags::evenParity(uint8_t val){
    setBits(PARITY_BIT, parity(val) % 2 == 0);
}

void Flags::sign(uint8_t val){
    setBits(SIGN_BIT, (val & SIGN_BIT)!= 0);
}

void Flags::zero(uint8_t val){
    setBits(ZERO_BIT, val == 0);
}

void Flags::calcAllBits(uint8_t val){
    zero(val);
    sign(val);
    evenParity(val);
}

uint8_t Flags::getRegisterValue(){
    return conditionBits;
}
