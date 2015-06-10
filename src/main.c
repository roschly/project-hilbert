//##########################################################
//##                      R O B O T I S                   ##
//## CM-510 (Atmega2561) Example code for Dynamixel.      ##
//##                                           2009.11.10 ##
//##########################################################

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <util/delay.h>
#include "dynamixel.h"
#include "serial.h"
#include "defines.c"

void setRightSpeed(int speed) {
	int id = 8;
	if (speed < 0) {
		speed = -1*speed;
	} else {
		speed += 1024;
	}
	dxl_write_word( id , 32, speed);	

}

void setLeftSpeed(int speed) {
	int id = 9;
	if (speed < 0) {
		speed = -1*speed+1024;
	}
	dxl_write_word( id , 32, speed);	

}

int getIR(int port) {
	ADMUX = port;										// ADC Port 1 Select	
	
	IR_LED(port, 1);

	_delay_us(12);				// Short Delay for rising sensor signal
	ADCSRA |= (1 << ADIF);		// AD-Conversion Interrupt Flag Clear
	ADCSRA |= (1 << ADSC);		// AD-Conversion Start
		
	while( !(ADCSRA & (1 << ADIF)) );	// Wait until AD-Conversion complete
	
	IR_LED(port, 0);

	return ADC;
}


int main(void)
{

	DDRA  = 0xFC;
	PORTA = 0xFC;

	serial_initialize(57600);
	dxl_initialize( 0, DEFAULT_BAUDNUM ); // Not using device index
	sei();	// Interrupt Enable	

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);	// ADC Enable, Clock 1/64div.

	// int speed = 0;

	// dxl_write_word( 8, 32, speed+1024 );	
	// dxl_write_word( 9, 32, speed );				


	while(1) {

		int IR[3];
		IR[0] = getIR(1);
		IR[1] = getIR(5);
		IR[2] = getIR(2);

		printf("1: %i - 2: %i - 3: %i \n", IR[0], IR[1], IR[2]);

		_delay_ms(500);
	}
}
