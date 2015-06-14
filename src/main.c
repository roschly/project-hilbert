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


struct limits {
	int longFrontCollision, sideCollision, wallCollision, wallDetection, sideDetectionMax, sideDetectionMin;
};
struct limits l = {
	.longFrontCollision = 500,
	.sideCollision = 300,
	.wallCollision = 300,
	.wallDetection = 50,
	.sideDetectionMax = 200,
	.sideDetectionMin = 100
};

struct sensors {
	int longRange, front, left, right, leftWall, rightWall;
};

struct sensors s;

int main(void)
{



	init();

	int attachedToWall = 0;
	char attachmentSide = 'N'; // N for null


	while(1) {

		s.longRange = getIR(3);

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

		int speed = 500;

		driveStraightAhead(&s, &l, speed);




		}
	}

/*
	// avoid collision
void avoidCollision(struct sensors *s, struct limits *l, struct attachment *a) {
	// if long range sensor OR sensor left/right too high
	// turn hard left/right
	if (s->longRange > l->longFrontCollision && a->toWall == 1){
		// get highest sensor left/right and turn opposite
		if (s->left > s->right){
			turnRightHard(300);
		}
		else {
			turnLeftHard(300);
		}
	}
	// first time encountering a wall, attach to it
	if (s->longRange > l->longFrontCollision && a->toWall == 0){
		a->toWall = 1;
		if (s->left > s->right){
			a->side = 'L';
			a->sideSensor = &s->left;
			a->wallSensor = &s->leftWall;
			turnRightHard(300);
		}
		else {
			a->side = 'R';
			a->sideSensor = &s->right;
			a->wallSensor = &s->rightWall;
			turnLeftHard(300);
		}
	}

	// side sensors
	if (s->left > l->sideCollision){
		turnRightHard(300);
	}
	if (s->right > l->sideCollision){
		turnLeftHard(300);
	}

	// wall sensors
	if (s->leftWall > l->wallCollision){
		turnRightHard(300);
	}
	if (s->rightWall > l->wallCollision){
		turnLeftHard(300);
	}
}
*/
// drive straight ahead
void driveStraightAhead(struct sensors *s, struct limits *l, int speed){
	if (s->longRange > l->longFrontCollision){
		moveForward(speed - s->longRange);
	}
	else {
		moveForward(speed);
	}
}
