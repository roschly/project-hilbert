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

int frontDist(int slr, int sf);
int attSide(int sensor);

int main(void)
{

	init();

	int attachedToWall = 0;
	char attachmentSide = 'N'; // N for null

	while(1) {

		int ir[6];
		ir[0] = getIR(5); //Long range front
		ir[1] = getIR(2); //Short range front
		ir[2] = getIR(1); //Left
		ir[3] = getIR(6); //Right
		ir[4] = getIR(3); // left wall
		ir[5] = getIR(4); // right wall

		// int speed = calcSpeed(ir, 600);
		int sl = ir[2];
		int sr = ir[3];
		int sf = ir[1];
		int slr = ir[0];
		int slw = ir[4];
		int srw = ir[5];


		//printf("sl: %i - sr: %i - sf: %i - slr: %i - slw: %i - srw: %i \n", sl, sr, sf, slr, slw, srw);
		//_delay_ms(2500);


		// TODO: we need floating points for this, and to calibrate each sensor
		//setLeftSpeed(680 - frontDist(slr, sf) - /* 4*sr + 3*sl */ + attSide(slw) /*- attSide(srw)*/ );
		//setRightSpeed(680 - frontDist(slr, sf) - /* 4*sl + 3*sr */ + attSide(srw) /*- attSide(slw)*/ );
		setLeftSpeed(300 - sf - sr + sl + attSide(slw) - attSide(srw));
		setRightSpeed(300 - sf - sl + sr + attSide(srw) - attSide(slw));

		}
	}

int frontDist(int slr, int sf){
	if (slr > 600){
		return 800;
	}
	else {
		return slr;
	}

};

int attSide(int sensor){
	// too close, move away
	if (sensor > 200){
		return sensor;
	}
	// beginning to slip away, move closer
	else if (sensor < 120 && sensor > 30){
		return (-3)*sensor;
	}
	// too far away, no attraction
	else {
		return 0;
	}
}
