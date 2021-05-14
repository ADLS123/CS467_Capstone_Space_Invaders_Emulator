/**************************************************************************************************
 ** File Name: QCpu.cpp
 ** Description: Contains the member function definitions for the Qcpu Class.
**************************************************************************************************/
#include "QCpu.h"



Qcpu::Qcpu(){
    //initialize cpu registers
    registers = init8080CPU();
    ports = initSIMachine();

    registers->int_enable = 0;      //sets interrupt enable flag to false

}


/* Reads from the machine ports into the CPU A register*/
void Qcpu::machineIN() {
    uint8_t input = registers->memory[registers->pc + 1];
    switch (input) {
        case 1:
            registers->a = ports->input1;
            break;
        case 2:
            registers->a = ports->input2;
            break;
        case 3:
            registers->a = ports->input3;
        break;
    }

}


/* Writes out from the CPU to the machine ports */
void Qcpu::machineOUT() {
    uint8_t output = registers->memory[registers->pc + 1];
    switch (output) {
        case 2:		// write to shift amount from bits 0-2 of A register
            ports->output2 = registers->a;
            break;
        case 3:
            ports->output3 = registers->a;
            emit writeOnPort3(ports->output3);
            break;
        case 4:		// write to shift registers from A register
        {
            ports->shiftLo = ports->shiftHi;
            ports->shiftHi = registers->a;
            break;
        }
        case 5:
            ports->output5 = registers->a;
            emit writeOnPort5(ports->output5);
            break;
    }
}
