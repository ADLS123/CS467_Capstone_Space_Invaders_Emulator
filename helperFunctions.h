/****************************************************************************************
** File Name: helperFunctions.h
** Authors: Devin Swanson
** Description: This file contains function declarations for the helper functions for 
		the Space Invaders ROM
****************************************************************************************/


#ifndef HELPERFUNCTIONS_H
#define HELPERFUNCTIONS_H

int parity(int value);		//checks parity of argument value
int setCarry(int value);
int setDoubleCarry(int value);
int zero(int value);		//function for setting the zero flag
#endif
