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

	int deltaSl = 0;
	int deltaSr = 0;
	int count = 0;

	while(1) {
		count++;

		int ir[4];
		ir[0] = getIR(5); //Long range front
		ir[1] = getIR(2); //Short range front
		ir[2] = getIR(1); //Left
		ir[3] = getIR(6); //Right

		// int speed = calcSpeed(ir, 600);
		int sl = ir[2];
		int sr = ir[3];
		int sf = ir[1];
		int slr = ir[0];




		//int speed = calcSpeed(ir, 500);

		/*
		if (sl > sr && max(sl,sr)>10) {
		 	turnRightSoft(speed);
		} else if (sl <= sr && max(sl,sr)>10) {
 	 		turnLeftSoft(speed);
		}
		*/

		// Collision avoidance
		int speed = 300;
		moveForward(speed);

		if (sf > 200) {
			int start = count;
			int right = (sl > sr) ? 1 : 0;
			while (count < start+300) {
				moveBackward(speed/2);
				count++;
			}
			start = count;
			while (count < start+50) {
				if (right) {
					turnRightHard(300);
				} else {
					turnLeftHard(300);
				}
				count++;
			}
		}

		if ((deltaSl - sl) > 50) {
			int start = count;
			while (count < start+60) {
				if (count > start+30) {
					turnLeftSoft(speed, 100);
				}
				count ++;
				_delay_ms(500);
			}
			moveForward(speed);
		}

		if ((deltaSr - sr) > 50) {
			int start = count;
			while (count < start+60) {
				if (count > start+30) {
					turnRightSoft(speed, 100);
				}
				count ++;
				_delay_ms(500);
			}
			moveForward(speed);
		}

		if (sf > 30){
			if (sl > sr){
				turnRightHard(600);
			}
			else{
				turnLeftHard(600);
			}
		}

		if (slr > 200 && max(sl,sr)>10){
			if (sl > sr) {
				turnRightSoft(speed, 150);
			} else {
				turnLeftSoft(speed, 150);
			}
		}

		deltaSl = sl;
		deltaSr = sr;

		_delay_ms(500);
	}
}
