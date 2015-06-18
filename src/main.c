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
//#include "serial.h"
#include "control.c"
#include "zigbee.h"

#define LED_BAT 0x01
#define LED_TxD 0x02
#define LED_RxD 0x04
#define LED_AUX 0x08
#define LED_MANAGE 0x10
#define LED_PROGRAM 0x20
#define LED_PLAY 0x40



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
	int RcvData;

	DDRC  = 0x7F;
	PORTC = 0x7E;

	PORTD &= ~0x80;	//PORT_LINK_PLUGIN = 0;   // no pull up
	PORTD &= ~0x20;	//PORT_ENABLE_RXD_LINK_PC = 0;
	PORTD |= 0x40;	//PORT_ENABLE_RXD_LINK_ZIGBEE = 1;

	zgb_initialize( 0 ); // Not using device index
	sei();

	init();

	standingNeutral();

	_delay_ms(2500);

	while(1)
	{

		if(zgb_rx_check() == 1)
		{
			RcvData = zgb_rx_data();
			if(RcvData & RC100_BTN_1)
				PORTC &= ~LED_MANAGE;
			else
				PORTC |= LED_MANAGE;

			if(RcvData & RC100_BTN_2)
				PORTC &= ~LED_PROGRAM;
			else
				PORTC |= LED_PROGRAM;

			if(RcvData & RC100_BTN_3)
				PORTC &= ~LED_PLAY;
			else
				PORTC |= LED_PLAY;
		}


		//printf("%i - %i - %i - %i - %i - %i - %i - %i \n", dxl_read_word(1,36), dxl_read_word(2,36), dxl_read_word(3,36), dxl_read_word(4,36), dxl_read_word(5,36), dxl_read_word(6,36), dxl_read_word(7,36), dxl_read_word(8,36));
		//_delay_ms(2500);

		//moveForward(timer, 0.4);
		//rotateLeft(timer, 0.3);
		//moveLegForward(&LF, timer, 0.3, 0.5);

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


void moveLegForward(struct leg *l, double timer, double speed, double phase, double hipAngleInterval){
	timer = timer * speed;
	setAngle(l->foot, hipAngleInterval*sin(timer+(Pi*phase)));
	setAngle(l->hip, max(-0.3, 0.5*sin(timer+(Pi*(phase + 0.5)))) );
};

void moveLegBackward(struct leg *l, double timer, double speed, double phase, double hipAngleInterval){
	timer = timer * speed;
	setAngle(l->foot, hipAngleInterval*sin(timer+(Pi*phase)));
	setAngle(l->hip, max(-0.3, 0.5*sin(timer+(Pi*(phase - 0.5)))) );
}

void moveForward(double timer, double speed){
	moveLegForward(&LF, timer, speed, 0.5, 1.0);
	moveLegForward(&RB, timer, speed, 0.5, 1.0);

	moveLegForward(&RF, timer, speed, 1.5, 1.0);
	moveLegForward(&LB, timer, speed, 1.5, 1.0);

};

void turnLeft(double timer, double speed, double degree){
	moveLegForward(&LF, timer, speed, 0.5, degree);
	moveLegForward(&RB, timer, speed, 0.5, 1.0);

	moveLegForward(&RF, timer, speed, 1.5, 1.0);
	moveLegForward(&LB, timer, speed, 1.5, degree);
};

void turnRight(double timer, double speed, double degree){
	moveLegForward(&LF, timer, speed, 0.5, 1.0);
	moveLegForward(&RB, timer, speed, 0.5, degree);

	moveLegForward(&RF, timer, speed, 1.5, degree);
	moveLegForward(&LB, timer, speed, 1.5, 1.0);
};


void rotateRight(double timer, double speed){
	moveLegBackward(&LF, timer, speed, 0.5, 1.0);
	moveLegForward(&RB, timer, speed, 0.5, 1.0);

	moveLegForward(&RF, timer, speed, 1.5, 1.0);
	moveLegBackward(&LB, timer, speed, 1.5, 1.0);
};

void rotateLeft(double timer, double speed){
	moveLegForward(&LF, timer, speed, 0.5, 1.0);
	moveLegBackward(&RB, timer, speed, 0.5, 1.0);

	moveLegBackward(&RF, timer, speed, 1.5, 1.0);
	moveLegForward(&LB, timer, speed, 1.5, 1.0);
};
