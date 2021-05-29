/**************************************************************************************************
    ** File Name: flags.cpp
    ** Description: This file contains the member function definitions for the Flags class.
**************************************************************************************************/

#include "flags.h"

/**************************************************************************************************
    ** Function Name: Flags::Flags()
    ** Description: Default constructor for an object of the Flags class, initializes the flags
        by setting them all to 0.
**************************************************************************************************/
Flags::Flags()
{
    conditionBits = EMPTY_FLAG;
}


/**************************************************************************************************
    ** Function Name: Flags::Flags()
    ** Description: Default constructor for an object of the Flags class, initializes the flags
        by setting them all to 0.
**************************************************************************************************/
Flags::Flags(uint8_t bits){
    conditionBits = EMPTY_FLAG | bits;
}

//sets the bits in a flag
void Flags::setBits(uint8_t bits){
    conditionBits = conditionBits | bits;
}

//sets or clears the bits in a flag based on the truth value of a condition
void Flags::setBits(uint8_t bits, bool value){
    value ? setBits(bits) : clearBits(bits);
}

//clears the bits in a flag
void Flags::clearBits(uint8_t bits){
    conditionBits = conditionBits & (bits ^ 0xFF);
}

//toggles the bits in a flag
void Flags::toggle(uint8_t bits){
    conditionBits = conditionBits ^ bits;
}

//test to see if a flag is set
bool Flags::testBits(uint8_t bits){
    uint8_t results = conditionBits & bits;
    return results == bits;
}

//counts the parity in a value
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

//sets the parity flag based on if value is even or not
void Flags::evenParity(uint8_t val){
    setBits(PARITY_BIT, parity(val) % 2 == 0);
}

//sets the sign bit
void Flags::sign(uint8_t val){
    setBits(SIGN_BIT, (val & SIGN_BIT)!= 0);
}

//sets the zero bit
void Flags::zero(uint8_t val){
    setBits(ZERO_BIT, val == 0);
}

//used to calculate zero, sign, and parity bit
void Flags::calcAllBits(uint8_t val){
    zero(val);
    sign(val);
    evenParity(val);
}

//getter for the flag register values
uint8_t Flags::getRegisterValue(){
    return conditionBits;
}
