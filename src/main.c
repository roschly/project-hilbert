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

		int ir[5];
		ir[0] = getIR(5); //Long range front
		ir[1] = getIR(2); //Short range front
		ir[2] = getIR(1); //Left
		ir[3] = getIR(6); //Right
		ir[4] = getIR(3); // left wall

		// int speed = calcSpeed(ir, 600);
		int sl = ir[2];
		int sr = ir[3];
		int sf = ir[1];
		int slr = ir[0];
		int slw = ir[4];

		//int speed = calcSpeed(ir, 500);

		/*
		if (sl > sr && max(sl,sr)>10) {
		 	turnRightSoft(speed);
		} else if (sl <= sr && max(sl,sr)>10) {
 	 		turnLeftSoft(speed);
		}
		*/

		//printf("sl: %i - sr: %i - sf: %i - slr: %i - slw: %i \n", sl, sr, sf, slr, slw);
		//_delay_ms(2500);

		int speed = 300;


		// if on collision course
		if (slr > 350){
			// set speed to 0
			moveForward(0);
			while ( getIR(5) > 150 ){
				turnRightHard(350);
			}
		}
		// if not on collision course

		else {
			moveForward(speed);

			// if in proximity of wall
			if (slw > 50){
				// if too close to wall
				if (sl > 75){
					turnRightSoft(speed, 200);
				}
				// if too far from wall
				if (sl < 30 ){
					turnLeftSoft(speed, 200);
				}
				// if
			}
			// if NOT in proximity of wall
			else{
				while (getIR(5) < 150 && getIR(1) < 80){
					turnLeftSoft(speed, 180);
				}
			}


/*
			// if wall disappeared
			if ( slw < 50){
					while (getIR(1) < 80){
						turnLeftSoft(speed, 180);
					}
			}
*/

		}





		// if slw < 20, turn left hard






		/*
		// Collision avoidance
		int speed = 300;
		moveForward(speed);



		if (sf > 500){
			moveBackward(300);
		}

		if (sf > 30){
			if (sl > sr){
				turnRightHard(600);
			}
			else{
				turnLeftHard(600);
			}
		}

		if (slr > 100){
			if (sl > sr && max(sl,sr)>10) {
				turnRightSoft(speed, 250);
			} else {
				turnLeftSoft(speed, 250);
			}
		}
		*/

		// int k = 70;
		// int sl = ((ir[2]+1)*k)/430;
		// int sr = ((ir[3]+1)*k)/350;

		//printf("%i \n", sf);


		//printf("\n");


	}
}
