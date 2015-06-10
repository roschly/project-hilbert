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

int main(void)
{

	init();

	while(1) {

		int ir[4];
		ir[0] = getIR(5); //Long range front
		ir[1] = getIR(2); //Short range front
		ir[2] = getIR(1); //Left
		ir[3] = getIR(6); //Right

		// int speed = calcSpeed(ir, 600);
		int sl = ir[2];
		int sr = ir[3];

		int speed = calcSpeed(ir, 500);

		// if (sl > sr && max(sl,sr)>10) {
		// 	turnRight(speed);
		// } else if (sl <= sr && max(sl,sr)>10) {
 	// 		turnLeft(speed);
		// }
		
		turnRight(80, 100);	

		// int k = 70;
		// int sl = ((ir[2]+1)*k)/430;  
		// int sr = ((ir[3]+1)*k)/350;

		// printf("%i : %i \n", ir[2], ir[3]);
		// printf("%i : %i \n", sl, sr);
		printf("\n");

		_delay_ms(500);
	}
}
