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

struct attachment {
	int toWall;
	char side;
	int *sideSensor;
	int *wallSensor;
};

struct sensors {
	int longRange, front, left, right, leftWall, rightWall;
};

struct limits {
	int longFrontCollision, sideCollision, wallCollision, wallDetection, sideDetectionMax, sideDetectionMin;
};

struct sensors s;
struct limits l = {
	.longFrontCollision = 350,
	.sideCollision = 180,
	.wallCollision = 250,
	.wallDetection = 35,
	.sideDetectionMax = 65,
	.sideDetectionMin = 25
};
struct attachment a = {
	.toWall = 0,
	.side = 'N'
};

// prototypes
int onCollisionCourse();
void avoidCollision();
int needWallAdjustment();
void followWall();
void driveStraightAhead();


int main(void) {

	init();

	int bla = 0;

	while(1) {

		// sample sensors
		s.longRange = getIR(3);
		s.front = getIR(4);
		s.left = getIR(6);
		s.right = getIR(2);
		s.leftWall = getIR(1);
		s.rightWall = getIR(5);


		//printf("sl: %i - sr: %i - sf: %i - slr: %i - slw: %i - srw: %i \n", sl, sr, sf, slr, slw, srw);
		//printf("attached?: %i \n", a.toWall);
		//printf("sensor left: %i - side sensor: %i \n", s.left, a.sideSensor);
		//_delay_ms(2500);


		int speed = calculateSpeed(s.longRange, 850); // 650 ?700

		// general protocol, priority list
		// 1. avoid collision
		// 2. followWall
		//		- 2a search for wall
		// 3. drive straight ahead



		if (s.longRange < 50 && bla == 0 || s.longRange < 120 && bla == 1 ){
			if (bla == 0){
				bla = 1;
			}
			else {
				bla = 0;
			}
			moveForward(speed);
		}
		else {


			if ( onCollisionCourse(&s, &l) == 1 ){
				avoidCollision(&s, &l, &a, speed);
			}
			else {
				if ( a.toWall == 1 && needWallAdjustment(&s, &l, &a) == 1 ){
					followWall(&s, &l, &a, speed);
				}
				else {
					driveStraightAhead(speed);
				}
				//driveStraightAhead(&s, &l, &a, speed);
			}
		}

	}
}

// condition
int onCollisionCourse(struct sensors *s, struct limits *l){
	//if ( s->longRange > l->longFrontCollision || s->left > l->sideCollision || s->right > l->sideCollision || s->leftWall > l->wallCollision || s->rightWall > l->wallCollision ){
	if ( s->longRange > l->longFrontCollision /*|| s->left > l->sideCollision || s->right > l->sideCollision */){
		return 1;
	}
	else {
		return 0;
	}
}

// avoid collision
void avoidCollision(struct sensors *s, struct limits *l, struct attachment *a, int speed) {
	if (s->longRange > l->longFrontCollision){
		if (a->toWall == 1){
			if (a->side == 'L'){
				turnRightHard(250);
			}
			if (a->side == 'R'){
				turnLeftHard(250);
			}
		}
		if (a->toWall == 0) {
			a->toWall = 1;
			if (s->left >= s->right){
				a->side = 'L';
				turnRightHard(250);
				//turnRightSoft(speed, 300);
			}
			else {
				a->side = 'R';
				turnLeftHard(250);
				//turnLeftSoft(speed, 300);
			}
		}
	// else
	// TODO: maybe remove this part?
	/*
	if (s->longRange <= l->longFrontCollision){
		// side sensors
		if (s->left > l->sideCollision){
			turnRightHard(250);
			//turnRightSoft(speed, 300);
		}
		if (s->right > l->sideCollision){
			turnLeftHard(250);
			//turnLeftSoft(speed, 300);
		}
	}
	*/

	/*
	// wall sensors
	if (s->leftWall > l->wallCollision){
		turnRightSoft(speed, 150);
		//turnRightSoft(speed, 300);
	}
	if (s->rightWall > l->wallCollision){
		turnLeftSoft(speed, 150);
		//turnLeftSoft(speed, 300);
	}
	*/
	}
}

//
int needWallAdjustment(struct sensors *s, struct limits *l, struct attachment *a){
	int sideSensor = 0;
	int wallSensor = 0;

	if (a->side == 'L'){
		sideSensor = s->left;
		wallSensor = s->leftWall;
	}
	if (a->side == 'R'){
		sideSensor = s->right;
		wallSensor = s->rightWall;
	}

	if (sideSensor > l->sideDetectionMax || sideSensor < l->sideDetectionMin){
	//if (wallSensor < l->wallDetection || sideSensor > l->sideDetectionMax || sideSensor < l->sideDetectionMin){
		return 1;
	}
	else {
		return 0;
	}
}

// follow wall
void followWall(struct sensors *s, struct limits *l, struct attachment *a, int speed){
	int deg = 250;
	int sideSensor = 0;
	int wallSensor = 0;

	if (a->side == 'L'){
		sideSensor = s->left;
		wallSensor = s->leftWall;
	}
	if (a->side == 'R'){
		sideSensor = s->right;
		wallSensor = s->rightWall;
	}

	/*
	// too close to wall
	if (sideSensor > l->sideDetectionMax){
		if (a->side == 'L'){
			turnSoft('R', speed, 200);
		}
		if (a->side == 'R') {
			turnSoft('L', speed, 200);
		}
	}

	// too far from wall
	if (sideSensor < l->sideDetectionMin){
		turnSoft(a->side, speed, 200);
	}
	*/


	// if wall sensor does NOT detect a wall, hard adjustments
	// "search for wall"
	if (wallSensor < l->wallDetection){
		turnHard(a->side, deg);
	}
	// if wall sensor detects a wall, soft adjustments
	else {
		// if sideSensor too close to wall
		// turn soft away
		// too close to wall
		if (sideSensor > l->sideDetectionMax){
			if (a->side == 'L'){
				turnSoft('R', speed, 350);
			}
			if (a->side == 'R') {
				turnSoft('L', speed, 350);
			}
		}

		// too far from wall
		if (sideSensor < l->sideDetectionMin){
			turnSoft(a->side, speed, 200);
		}

	}

}



// drive straight ahead
void driveStraightAhead(int speed){

		moveForward(speed);

};
