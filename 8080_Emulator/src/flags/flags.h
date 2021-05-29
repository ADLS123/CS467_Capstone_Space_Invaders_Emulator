/**************************************************************************************************
    ** File Name: flags.h
    ** Description: This file contains the Class declaration for the Flags class, which make up
        the different flags in the Intel 8080 Processor.
**************************************************************************************************/

#include <stdint.h>

#ifndef FLAGS_H
#define FLAGS_H

//flags set by their binary to closely resemble the architecture
const uint8_t SIGN_BIT = 0b10000000;
const uint8_t ZERO_BIT = 0b01000000;
const uint8_t AUX_BIT = 0b00010000;
const uint8_t PARITY_BIT = 0b00000100;
const uint8_t CARRY_BIT = 0b00000001;

const uint8_t EMPTY_FLAG = 0b00000010;          //what the empty flag register is set to

class Flags
{
public:
    Flags();                                    //default constructor
    Flags(uint8_t bits);                        //parameterized constructor

    uint8_t getRegisterValue();                 //getter for conditionBits
    void setBits(uint8_t bits);                 //set a flag regardless
    void setBits(uint8_t bits, bool value);     //set flags conditionally
    void clearBits(uint8_t bits);               //clears a flag
    void toggle(uint8_t bits);                  //toggles a flag
    bool testBits(uint8_t bits);                //checks flag for specific condition

    uint8_t parity(uint8_t val);                //counting parity
    void evenParity(uint8_t val);               //checks for even parity
    void sign(uint8_t val);                     //checks sign of of a value
    void zero(uint8_t val);                     //checks if value is zero
    void calcAllBits(uint8_t value);            //Calculate all flags from single value

private:
    uint8_t conditionBits;
};

#endif // FLAGS_H
