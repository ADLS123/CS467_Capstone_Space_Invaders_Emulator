/**************************************************************************************************
** File Name: ioPorts.c
** Description: This file contains function declarations for handling the calls to the IO ports.
**************************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "ioPorts.h"
#include "../cpu/8080State.h"


StateSIMachine* initSIMachine() {
    StateSIMachine* machine = (StateSIMachine*)calloc(1, sizeof(StateSIMachine));
    return machine;
}
