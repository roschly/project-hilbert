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

	int attachedToWall = 0;
	char attachmentSide = 'N'; // N for null


	while(1){

		//printf("front: %i \n", s.front);
		//_delay_ms(2500);

		int ir[6];
		ir[0] = getIR(5); //Long range front
		ir[1] = getIR(2); //Short range front
		ir[2] = getIR(1); //Left
		ir[3] = getIR(6); //Right
		ir[4] = getIR(3); // left wall
		ir[5] = getIR(4); // right wall


		// int speed = calcSpeed(ir, 600);
		int sl = getIR(6);
		int sr = getIR(2);
		int sf = getIR(4);
		int slr = getIR(3);
		int slw = getIR(1);
		int srw = getIR(5);

		int speed = 600;

		// if not on collision course AND NOT attachedToWall
		if (slr <= 350 && attachedToWall == 0) {
			moveForward(speed);

			// TODO: avoid collision based on side sensors
		}

		// when first in front-proximity of wall
		if (slr > 350 && attachedToWall == 0){
			attachedToWall = 1;
			//
			// if closer to left wall, attach to it
			//
			if (sl > sr){
				attachmentSide = 'L';
			}
			if (sl <= sr){
				attachmentSide = 'R';
			}
		}

		if (attachmentSide == 'L'){
			// LEFT LEFT
			// if on collision course
			if (slr > 350){
				// set speed to 0
				moveForward(0);
				// get sensor and turn away from it
				while ( getIR(5) > 150 ){
					turnRightHard(500);
				}
			}

			// if not on collision course AND attachedToWall
			if (slr <= 350 && attachedToWall == 1) {
				moveForward(speed);

				// if in proximity of wall
				if (slw > 50 && sl > 20){
					// if too close to wall
					if (sl > 75 ){
						turnRightSoft(speed, 200);
					}
					// if too far from wall
					if (sl < 30 ){
						turnLeftSoft(speed, 200);
					}
					// if
				}
				// if NOT in proximity of wall
				if (slw <= 50){
					while (getIR(5) < 150 && getIR(1) < 80){
						turnLeftSoft(speed, 180);
					}
				}
			}
		}

		if (attachmentSide == 'R'){
			// RIGHT RIGHT
			// if on collision course
			if (slr > 350){
				// set speed to 0
				moveForward(0);
				// get sensor and turn away from it
				while ( getIR(5) > 150 ){
					turnLeftHard(500);
				}
			}

			// if not on collision course AND attachedToWall
			if (slr <= 350 && attachedToWall == 1) {
				moveForward(speed);

				// if in proximity of wall
				if (srw > 50 && sr > 20){
					// if too close to wall
					if (sr > 75 ){
						turnLeftSoft(speed, 200);
					}
					// if too far from wall
					if (sr < 30 ){
						turnRightSoft(speed, 200);
					}
					// if
				}
				// if NOT in proximity of wall
				if (srw <= 50){
					while (getIR(5) < 150 && getIR(6) < 80){
						turnRightSoft(speed, 180);
					}
				}
			}
		}
	}
}
