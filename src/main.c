//##########################################################
//##                      R O B O T I S                   ##
//## CM-510 (Atmega2561) Example code for Dynamixel.      ##
//##                                           2009.11.10 ##
//##########################################################

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>
#include "dynamixel.h"
#include "serial.h"
#include "defines.c"
#include "control.c"


void turnRight() {
	int count = 0;
	int start = count;
	while (count < start+400) {
		turnRightHard(300);
		count++;
	}
}

int main(void)
{

	init();
	while(1) {
		
}

