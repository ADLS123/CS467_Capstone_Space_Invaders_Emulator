#include <stdint.h>

#ifndef FLAGS_H
#define FLAGS_H


const uint8_t SIGN_BIT = 0b10000000;
const uint8_t ZERO_BIT = 0b01000000;
const uint8_t AUX_BIT = 0b00010000;
const uint8_t PARITY_BIT = 0b00000100;
const uint8_t CARRY_BIT = 0b00000001;

const uint8_t EMPTY_FLAG = 0b00000010;

class Flags
{
public:
    Flags();
    Flags(uint8_t bits);

    uint8_t getRegisterValue();
    void setBits(uint8_t bits);
    void setBits(uint8_t bits, bool value);
    void clearBits(uint8_t bits);
    void toggle(uint8_t bits);
    bool testBits(uint8_t bits);

    uint8_t parity(uint8_t val);
    void evenParity(uint8_t val);
    void sign(uint8_t val);
    void zero(uint8_t val);
    void calcAllBits(uint8_t value);

private:
    uint8_t conditionBits;
};

#endif // FLAGS_H
