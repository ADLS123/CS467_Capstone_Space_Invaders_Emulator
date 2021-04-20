#include <stdio.h>

#include "helperFunctions.h"



/****************************************************************************************
** Function Name: int parity(int value)
** Parameters: an integer to check the parity of
** Returns: an integer, will return 1 for even parity, returns 0 for odd parity
** Description: Helper function to check and set the parity flag for the emulator. 
		Parity function matches the parity bit setting for the 8080 processor, where 
		even parity is set to 1 and odd parity is set to 0.


** NOTE: Parity function heavily inspired from the code found at:
	URL: https://www.tutorialspoint.com/cplusplus-program-to-find-the-parity-of-a-number-efficiently
****************************************************************************************/
int parity(int value){
	int y;
	y = value ^ (value >> 1);		//sets y to an XOR of value and value right-shifted with 1
	y = y ^ (y >> 2);				//sets y to an XOR of y right-shifted with 2
	y = y ^ (y >> 4);				//repeats the above line with 4, 8, 16
	y = y ^ (y >> 8);
	y = y ^ (y >> 16);
	//checks the rightmost bit
	if (y & 1) {
		return 0;					//returns 0 for odd parity
	}
	return 1;						//returns 1 for even parity
}



/*
//Testing main for whatever helper function is needed
int main() {
	int n = 9;
	int result = parity(n);
	if (result == 1) {
		printf("Even parity\n");
	}
	else {
		printf("Odd parity\n");
	}

	return 0;
}
*/