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
int sticky(int sensor);
double scaleSensor(int sensor);

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
		int sl = getIR(6);
		int sr = getIR(2);
		int sf = getIR(4);
		int slr = getIR(3);
		int slw = getIR(1);
		int srw = getIR(5);


		//printf("sl: %i - sr: %i - sf: %i - slr: %i - slw: %i - srw: %i \n", sl, sr, sf, slr, slw, srw);
		//_delay_ms(2500);


		// long path
		// slr < ca 60-100,

		// srw 50-100 > slw

		// slr deadzone is ca 640
		// sf deadzone is ca 175

		//setLeftSpeed(620.0 - frontDist(slr, sf) - 2.5*sr + 3.5*sl + 1.0*slw  /* + attSide(slw) - attSide(srw)*/ );
		//setRightSpeed(620.0 - frontDist(slr, sf) - 2.5*sl + 3.5*sr + 1.0*srw /* + attSide(srw) - attSide(slw)*/ );
		//setLeftSpeed(300 - sf - sr + sl + attSide(slw) - attSide(srw));
		//setRightSpeed(300 - sf - sl + sr + attSide(srw) - attSide(slw));

		//setLeftSpeed(680 - frontDist(slr, sf) - 1.5*sr + 1.5*sl + 0.5*slw);
		//setRightSpeed(680 - frontDist(slr, sf) - 1.5*sl + 1.5*sr + 0.5*srw);

		//setLeftSpeed(MAX_SPEED - frontDist(slr, sf) -3.5*scaleSensor(sr) + 2.0*scaleSensor(sl) -1.5*srw + sticky(slw) );
		//setRightSpeed(MAX_SPEED - frontDist(slr, sf) -3.5*scaleSensor(sl) + 2.0*scaleSensor(sr) -1.5*slw + sticky(srw) );

		setLeftSpeed(MAX_SPEED - frontDist(slr, sf) - 4.5*sr + 1.5*sl + sticky(slw) );
		setRightSpeed(MAX_SPEED - frontDist(slr, sf) - 4.5*sl + 1.5*sr + sticky(srw) );


		}
	}

int frontDist(int slr, int sf){
	if (slr > 600 ){
		return MAX_SPEED + 50;
	}
	else {
		return slr;
	}

};

int sticky(int sensor){
	// too close, move away
	if (sensor > 200){
		return 0.7*sensor;
	}
	// beginning to slip away, move closer
	else if (sensor < 150 && sensor > 30){
		return (-1.5)*sensor;
	}
	// too far away, no attraction
	else {
		return 0;
	}
}

double scaleSensor(int sensor){
	if (sensor > 250){
		return 0.5*sensor;
	}
	else{
		return sensor;
	}
}
