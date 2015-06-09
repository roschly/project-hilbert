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

/// Control table address
#define P_GOAL_POSITION_L		30
#define P_GOAL_POSITION_H		31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING				46

// Defulat setting
#define DEFAULT_BAUDNUM		1 // 1Mbps
#define DEFAULT_ID			1

#define		ADC_PORT_1	1
#define		ADC_PORT_2	2
#define		ADC_PORT_3	3
#define		ADC_PORT_4	4
#define		ADC_PORT_5	5
#define		ADC_PORT_6	6

void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);

int main(void)
{
	int go = 1;

	DDRA  = 0xFC;
	PORTA = 0xFC;

	serial_initialize(57600);
	dxl_initialize( 0, DEFAULT_BAUDNUM ); // Not using device index
	sei();	// Interrupt Enable	

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);	// ADC Enable, Clock 1/64div.
	ADMUX = ADC_PORT_1;										// ADC Port 1 Select	

	int speed = 512;

	dxl_write_word( 8, 32, speed+1024 );	
	dxl_write_word( 9, 32, speed );				


	while(go) {
		PORTA &= ~0x80;			// ADC Port 1 IR ON

		_delay_us(12);				// Short Delay for rising sensor signal
		ADCSRA |= (1 << ADIF);		// AD-Conversion Interrupt Flag Clear
		ADCSRA |= (1 << ADSC);		// AD-Conversion Start
			
		while( !(ADCSRA & (1 << ADIF)) );	// Wait until AD-Conversion complete
		PORTA = 0xFC;

		if (ADC > 20) {
			printf("Wall detected! %i\n", ADC);
			go = 0;
		}

		_delay_ms(20);
	}

	dxl_write_word( 8, 32, 0 );	
	dxl_write_word( 9, 32, 0 );
}
