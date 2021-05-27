/**************************************************************************************************
    ** File Name: cpu.h
    ** Description: This file contains the Class declaration for the Cpu class, one of four classes
        that make up the Space Invaders Emulator. The Cpu class consists of a State8080Registers
        struct, a Flags object for the different flags, inputs and outputs, and functions for
        all of the Assembly instructions required to emulate Space Invaders.
**************************************************************************************************/

#include <QObject>
#include <cstring>
#include "../flags/flags.h"

#ifndef CPU_H
#define CPU_H



class Cpu : public QObject
{
    Q_OBJECT
public:
    Cpu();                              //default constructor
    ~Cpu();                             //deconstructor
    //registers
    struct State8080Registers{
        uint8_t a;
        uint8_t b;
        uint8_t c;
        uint8_t d;
        uint8_t e;
        uint8_t h;
        uint8_t l;
        uint16_t pc;
        uint16_t sp;
    } registers;

    Flags flags;                        //required flags

    bool enableInterrupts;              //flag for enabling interrupts
    bool twoPlayer;                     //internal flag for 1 or 2 player game
    //inputs and outputs
    uint8_t input0;
    uint8_t input1;
    uint8_t input2;
    uint8_t input3;

    uint8_t output2;
    uint8_t output3;
    uint8_t output4;
    uint8_t output5;
    uint8_t output6;

    //memory
    uint8_t *memory;

    //generateInterrupts function
    bool generateInterrupt(uint8_t opCode);
    uint8_t getLowBits(uint8_t value);
    uint8_t getLowBits(uint16_t value);
    uint8_t getHighBits(uint8_t value);
    uint8_t getHighBits(uint16_t value);
    int addBytes(uint8_t byte, uint8_t byte2, bool carryIn, Flags flagVar);

    //emulating functions
    int emulateInstruction();
    int getInstruction(uint8_t);

    //opcode functions
    //logic op functions
    int ana(uint8_t secondRegister);
    int anaM();
    int xra(uint8_t secondRegister);
    int xraM();
    int ora(uint8_t secondRegister);
    int oraM();
    int cmp(uint8_t secondRegister);
    int cmpM();
    int rlc();
    int rrc();
    int ral();
    int rar();
    int ani();
    int cma();
    int xri();
    int ori();
    int cpi();
    int cmc();
    int stc();
    int mov(uint8_t &destination, uint8_t source);
    int movToM(uint8_t source);
    int movMTo(uint8_t &destination);
    int mvi(uint8_t &destination);
    int mviM();
    int lxi(uint8_t &nameRegister, uint8_t &nextRegister);
    int lxiSP();
    int ldax(uint8_t nameRegister, uint8_t nextRegister);
    int stax(uint8_t nameRegister, uint8_t nextRegister);
    int shld();
    int lhld();
    int sta();
    int lda();
    int pchl();
    int xchg();
    int ei();
    int di();
    int hlt();
    int nop();

    //movement and stack op functions
    int jmp();
    int conditionalJmp(bool condition);
    int call();
    int conditionalCall(bool condition);
    int rst(uint8_t resetNumber);
    int ret();
    int conditionalRet(bool condition);
    int push(uint8_t nameRegister, uint8_t nextRegister);
    int pushPSW();
    int pop(uint8_t &nameRegister, uint8_t &nextRegister);
    int popPSW();
    int sphl();
    int xthl();

    //arithmetic functions
    int inx(uint8_t &nameRegister, uint8_t &nextRegister);
    int inxSP();
    int dcx(uint8_t &nameRegister, uint8_t &nextRegister);
    int dcxSP();
    int inr(uint8_t &regName);
    int inrM();
    int dcr(uint8_t &regName);
    int dcrM();
    int dad(uint8_t nameRegister, uint8_t nextRegister);
    int dadSP();
    int add(uint8_t regName);
    int addM();
    int adc(uint8_t regName);
    int adcM();
    int sub(uint8_t regName);
    int subM();
    int sbb(uint8_t regName);
    int sbbM();
    int adi();
    int sui();
    int aci();
    int sbi();
    int in();
    int out();
    int daa();

signals:
    void writeOnPort3(int raw);                 //used for triggering sounds on port 3
    void writeOnPort5(int raw);                 //used for triggering sounds on port 5
};

#endif // CPU_H
