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



int main(void) {

	init();

	// Long Path Ahead
	// is there a long clear path ahead, measured by the DMS
	int LPA = 0;

	while(1) {

		// sample sensors
		s.longRange = getIR(3);
		s.front = getIR(4);
		s.left = getIR(6);
		s.right = getIR(2);
		s.leftWall = getIR(1);
		s.rightWall = getIR(5);


		int speed = calculateSpeed(s.longRange, 850); // 650 ?700

		if (s.longRange < 50 && LPA == 0 || s.longRange < 110 && LPA == 1 ){
			if (LPA == 0){
				LPA = 1;
			}
			else {
				LPA = 0;
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
					moveForward(speed);
				}
			}
		}
	}
}

// condition
int onCollisionCourse(struct sensors *s, struct limits *l){
	if ( s->longRange > l->longFrontCollision){
		return 1;
	}
	else {
		return 0;
	}
}

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
			}
			else {
				a->side = 'R';
				turnLeftHard(250);
			}
		}
	}
}

// condition
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
		return 1;
	}
	else {
		return 0;
	}
}

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

	// if wall sensor does NOT detect a wall, hard adjustments
	// "search for wall"
	if (wallSensor < l->wallDetection){
		turnHard(a->side, deg);
	}
	// if wall sensor detects a wall, soft adjustments
	else {
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
