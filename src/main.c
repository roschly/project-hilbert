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

int isNeutral = 0;

struct leg {
	int foot, hip;
	double lastCmdFoot, lastCmdHip;
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

	// _delay_ms(2500);

	char inpChar = 'n';

	while(1)
	{
		CommStatus = dxl_get_result();


		//printf("%i - %i - %i - %i - %i - %i - %i - %i \n", dxl_read_word(1,36), dxl_read_word(2,36), dxl_read_word(3,36), dxl_read_word(4,36), dxl_read_word(5,36), dxl_read_word(6,36), dxl_read_word(7,36), dxl_read_word(8,36));
		//_delay_ms(2500);

		if (serial_get_qstate() != 0) {
			inpChar = serial_get_queue();
		}

		switch (inpChar) {
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
				turnLeft(timer, 0.3, 0.5);
				break;
			case 'e':
				turnRight(timer, 0.3, 0.5);
				break;
			default:
		/*		if (!isNeutral)  {
					standingNeutral(&LF, &RF, &LB, &RB);
				}*/
				break;
		};
	}

	return 0;
};



ISR(TIMER1_COMPA_vect) {
	OCR1A += 249; //additional 1 ms for next interrupt
	timer += 0.02;
};

/*int moveLogic(double lastCmd, double footPos) {

	if (lastCmd >= 0 || footPos >= 0) {
		return max(lastCmd, footPos) - min(lastCmd, footPos);
	}

	if (lastCmd < 0 && footPos < 0) {
		return max(abs(lastCmd), abs(footPos)) - min(abs(lastCmd), abs(footPos));
	}

	return 0;

}*/


void moveLegForward(struct leg *l, double timer, double speed, double phase, double footAngleInterval){
	double stdTime = fmod(timer*speed, 2.0*Pi);
	//timer = timer * speed;
	timer = stdTime;

	double footTol = 0.05;

	double footPos = footAngleInterval*sin(timer+(Pi*phase));

	// if (moveLogic(l->lastCmdFoot, footPos) >= footTol) {
		setAngle(l->foot, footPos);
	// }

	setAngle(l->hip, max(-0.3, 0.5*sin(timer+(Pi*(phase + 0.5)))) );

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


void standingNeutral(struct leg *LF, struct leg *RF, struct leg *LB, struct leg *RB){
	// feet
/*	setAngle(LF->foot, 0);
	setAngle(RF->foot, 0);
	setAngle(LB->foot, 0);
	setAngle(RB->foot, 0);

	LF->lastCmdFoot = 0;
	RF->lastCmdFoot = 0;
	LB->lastCmdFoot = 0;
	RB->lastCmdFoot = 0;

	// hips
	setAngle(LF->hip, 0);
	setAngle(RF->hip, 0);
	setAngle(LB->hip, 0);
	setAngle(RB->hip, 0);
	
	LF->lastCmdHip = 0;
	RF->lastCmdHip = 0;
	LB->lastCmdHip = 0;
	RB->lastCmdHip = 0;
*/
	isNeutral = 1;
}

// degree: int [-100:100]
void setAngle(int id, double degree){
	/*
	For hips: +/- 60 grader
	for feet: +/ 150 grader
	*/

	if (degree > 1){
		degree = 1;
	}
	if (degree < -1){
		degree = -1;
	}


	double angle;

	//feet
	int maxFootAngleInterval = 165; // 150
	if (id == 1 || id == 3 || id == 4 || id == 5){
		angle = degree * maxFootAngleInterval;
		if (id == 5 || id == 3){
			angle = -angle;
		}
	}

	// hips
	int maxHipAngleInterval = 60;
	if (id == 2 || id == 6 || id == 7 || id == 8){
		angle = degree * maxHipAngleInterval;
		if (id == 6 || id == 7){
				angle = -angle;
		}
	}


	dxl_write_word(id, 30, angle+512);

}


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
