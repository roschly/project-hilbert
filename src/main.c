//##########################################################
//##                      R O B O T I S                   ##
//## CM-510 (Atmega2561) Example code for Dynamixel.      ##
//##                                           2009.11.10 ##
//##########################################################

#define F_CPU 16000000UL

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include <math.h>

#include "defines.c"
#include "dynamixel.h"
#include "serial.h"
#include "control.c"


double timer = 0.0;

struct leg {
	int foot, hip;
};

struct leg LF = {
	.foot = 1,
	.hip = 8
};

struct leg RF = {
	.foot = 3,
	.hip = 6
};

struct leg LB = {
	.foot = 4,
	.hip = 7
};

struct leg RB = {
	.foot = 5,
	.hip = 2
};


int main(void){

	init();


	standingNeutral();

	_delay_ms(2500);

	while(1)
	{
		//printf("%i - %i - %i - %i - %i - %i - %i - %i \n", dxl_read_word(1,36), dxl_read_word(2,36), dxl_read_word(3,36), dxl_read_word(4,36), dxl_read_word(5,36), dxl_read_word(6,36), dxl_read_word(7,36), dxl_read_word(8,36));
		//_delay_ms(2500);

		//moveForward(timer, 0.3);
		moveLegForwardAsync(&LF, timer, 0.3, 0.5);

	}

	return 0;
};



ISR(TIMER1_COMPA_vect) {
	OCR1A += 249; //additional 1 ms for next interrupt
	timer += 0.02;
};

void moveLegForwardAsync(struct leg *l, double timer, double speed, double phase){
	timer = timer * speed;

	double stdTime = fmod(timer, 2.0*Pi);

	if (stdTime < 0.5*Pi){
		setAngle(l->foot, sin(4*(stdTime + (Pi*phase) )) );
		setAngle(l->hip, max(-0.3, 0.5*sin(4*(stdTime+(Pi*(phase - 0.5))))) );
	}
	else {
		setAngle(l->foot, sin(timer+(Pi*phase)));
		setAngle(l->hip, max(-0.3, 0.5*sin(timer+(Pi*(phase - 0.5)))) );
	}


};


void moveLegForward(struct leg *l, double timer, double speed, double phase){
	timer = timer * speed;
	setAngle(l->foot, sin(timer+(Pi*phase)));
	setAngle(l->hip, max(-0.3, 0.5*sin(timer+(Pi*(phase - 0.5)))) );
};

void moveForward(double timer, double speed){
	moveLegForward(&LF, timer, speed, 0.5);
	moveLegForward(&RB, timer, speed, 0.5);

	moveLegForward(&RF, timer, speed, 1.5);
	moveLegForward(&LB, timer, speed, 1.5);

};
