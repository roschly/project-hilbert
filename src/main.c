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

	DDRC  = 0x7F;
	PORTC = 0x7E;

	PORTD &= ~0x80;	//PORT_LINK_PLUGIN = 0;   // no pull up
	PORTD &= ~0x20;	//PORT_ENABLE_RXD_LINK_PC = 0;
	PORTD |= 0x40;	//PORT_ENABLE_RXD_LINK_ZIGBEE = 1;

	init();

	//standingNeutral();
	int CommStatus;

	_delay_ms(2500);

	char bla = 'n';

	while(1)
	{
		CommStatus = dxl_get_result();


		//printf("%i - %i - %i - %i - %i - %i - %i - %i \n", dxl_read_word(1,36), dxl_read_word(2,36), dxl_read_word(3,36), dxl_read_word(4,36), dxl_read_word(5,36), dxl_read_word(6,36), dxl_read_word(7,36), dxl_read_word(8,36));
		//_delay_ms(2500);

		//moveForward(timer, 0.35);
		//rotateLeft(timer, 0.3);
		//moveLegForward(&LF, timer, 0.3, 0.5);



		/*
		if (std_getchar() != 'w'){
			bla = std_getchar();
		}
		*/

		bla = 'w';
		//std_putchar(bla);

		switch (bla) {
			case 'w':
				moveForward(timer, 0.3);
				break;
			case 's':
				moveBackward(timer, 0.3);
				break;
			case 'a':
				rotateLeft(timer, 0.3);
				break;
			case 'd':
				rotateRight(timer, 0.3);
				break;
			case 'q':
				turnLeft(timer, 0.3, 0.1);
				break;
			case 'e':
				turnRight(timer, 0.3, 0.1);
				break;
			case 'n':
				standingNeutral();
				break;
		};

		/*
		std_putchar(std_getchar());
		if (std_getchar() == 'w'){
			setAngle(LF.foot, 1.0);
		}
		else if(std_getchar() == 'w'){

		}
		else {
			setAngle(LF.foot, -1.0);
		}
		*/

	}

	return 0;
};



ISR(TIMER1_COMPA_vect) {
	OCR1A += 249; //additional 1 ms for next interrupt
	timer += 0.02;
};


void moveLegForward(struct leg *l, double timer, double speed, double phase, double footAngleInterval){
	double stdTime = fmod(timer*speed, 2.0*Pi);
	//timer = timer * speed;
	timer = stdTime;



	//if ( dxl_read_byte(l->foot, 46) == 0 ){
		setAngle(l->foot, footAngleInterval*sin(timer+(Pi*phase)));

		// if front legs
		if (l->hip == 6 || l->hip == 8){
			setAngle(l->hip, max(0, 0.2*sin(timer+(Pi*(phase + 0.5)))) );
		}
		else {
			setAngle(l->hip, max(-0.3, 0.5*sin(timer+(Pi*(phase + 0.5)))) );
		}
	//}

};

void moveLegBackward(struct leg *l, double timer, double speed, double phase, double footAngleInterval){
	timer = timer * speed;
	setAngle(l->foot, footAngleInterval*sin(timer+(Pi*phase)));
	setAngle(l->hip, max(-0.3, 0.5*sin(timer+(Pi*(phase - 0.5)))) );
}

void moveForward(double timer, double speed){
	moveLegForward(&LF, timer, speed, 0.5, 1.0);
	moveLegForward(&RB, timer, speed, 0.5, 1.0);

	moveLegForward(&RF, timer, speed, 1.5, 1.0);
	moveLegForward(&LB, timer, speed, 1.5, 1.0);

};

void moveBackward(double timer, double speed){
	moveLegBackward(&LF, timer, speed, 0.5, 1.0);
	moveLegBackward(&RB, timer, speed, 0.5, 1.0);

	moveLegBackward(&RF, timer, speed, 1.5, 1.0);
	moveLegBackward(&LB, timer, speed, 1.5, 1.0);

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


/*
// Print communication result
void PrintCommStatus(int CommStatus)
{
	switch(CommStatus)
	{
	case COMM_TXFAIL:
		printf("COMM_TXFAIL: Failed transmit instruction packet!\n");
		break;

	case COMM_TXERROR:
		printf("COMM_TXERROR: Incorrect instruction packet!\n");
		break;

	case COMM_RXFAIL:
		printf("COMM_RXFAIL: Failed get status packet from device!\n");
		break;

	case COMM_RXWAITING:
		printf("COMM_RXWAITING: Now recieving status packet!\n");
		break;

	case COMM_RXTIMEOUT:
		printf("COMM_RXTIMEOUT: There is no status packet!\n");
		break;

	case COMM_RXCORRUPT:
		printf("COMM_RXCORRUPT: Incorrect status packet!\n");
		break;

	default:
		printf("This is unknown error code!\n");
		break;
	}
}
*/

/*
// Print error bit of status packet
void PrintErrorCode()
{
	if(dxl_get_rxpacket_error(ERRBIT_VOLTAGE) == 1)
		printf("Input voltage error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_ANGLE) == 1)
		printf("Angle limit error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERHEAT) == 1)
		printf("Overheat error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_RANGE) == 1)
		printf("Out of range error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_CHECKSUM) == 1)
		printf("Checksum error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_OVERLOAD) == 1)
		printf("Overload error!\n");

	if(dxl_get_rxpacket_error(ERRBIT_INSTRUCTION) == 1)
		printf("Instruction code error!\n");
}
*/
