//##########################################################
//##                      R O B O T I S                   ##
//## CM-510 (Atmega2561) Example code for Dynamixel.      ##
//##                                           2009.11.10 ##
//##########################################################

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
//#include <math.h>

#include "dynamixel.h"
#include "serial.h"
#include "control.c"


int timer = 0.0;

int main(void){

	init();

	/*
	setAngle(FFR, 0);
	setAngle(FFL, 0);
	setAngle(FBR, 0);
	setAngle(FBL, 0);


	setAngle(HFR, -100);
	setAngle(HFL, 100);
	setAngle(HBR, 100);
	setAngle(HBL, -100);
	*/

	/*
	//_delay_ms(1000);
	setAngle(FBR, 0);
	setAngle(HBR, -100);

	_delay_ms(2500);
	setAngle(FBR, 100);
	setAngle(HBR, 0);

	_delay_ms(2500);
	setAngle(FBR, 0);
	setAngle(HBR, 100);
	*/

	while(1)
	{


		//dxl_write_word(1, 30, sin(timer/100)*50+512);


		setAngle(1, sin(timer/100)*50 );


		//printf("%i - %i - %i - %i - %i - %i - %i - %i \n", dxl_read_word(1,36), dxl_read_word(2,36), dxl_read_word(3,36), dxl_read_word(4,36), dxl_read_word(5,36), dxl_read_word(6,36), dxl_read_word(7,36), dxl_read_word(8,36));
		// hips
		//printf("%i - %i - %i - %i \n", dxl_read_word(2,36), dxl_read_word(6,36), dxl_read_word(7,36), dxl_read_word(8,36));
		//printf("Floor: %i \n", (int)floor(2.3) );
		//_delay_ms(2500);

		//moveForward(speed);



	}

	return 0;
}



ISR(TIMER1_COMPA_vect) {
	OCR1A += 249; //additional 1 ms for next interrupt
	timer += 1.0;
}
