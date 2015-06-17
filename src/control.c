#include <math.h>


/// Control table address
#define P_GOAL_POSITION_L		30
#define P_GOAL_POSITION_H		31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING				46

// Motor IDs
#define FOOT_FR 3
#define FOOT_BR 5
#define FOOT_FL 1
#define FOOT_BL 4

#define HIP_FR 6
#define HIP_BR 2
#define HIP_FL 8
#define HIP_BL 7

void init() {
	DDRA  = 0xFC;
	PORTA = 0xFC;

	serial_initialize(57600);
	dxl_initialize( 0, 1 ); // Not using device index
	sei();	// Interrupt Enable

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);	// ADC Enable, Clock 1/64div.
}



// degree: int [-100:100]
void setAngle(int id, int degree){
	/*
	For hips: +/- 60 grader
	for feet: +/ 150 grader
	*/

	if (degree > 100){
		degree = 100;
	}
	if (degree < -100){
		degree = -100;
	}


	int angle;

	//feet
	int maxFootAngle = 150;
	if (id == 1 || id == 3 || id == 4 || id == 5){
		if (id == 5 || id == 3){
			angle = -floor( (degree/100) * maxFootAngle );
		}
		else{
			angle = floor( (degree/100) * maxFootAngle );
		}
	}

	// hips
	int maxHipAngle = 60;
	if (id == 2 || id == 6 || id == 7 || id == 8){
		if (id == 6 || id == 7){
			angle = -floor( (degree/100) * maxHipAngle );
		}
		else {
			angle = floor( (degree/100) * maxHipAngle );
		}
	}


	dxl_write_word(id, 30, angle+512);

}


void setMotorAngle(int id, int angle) {

	if (angle > 1023)
		angle = 1023;

	if (angle < 0)
		angle = 0;

	dxl_write_word(id, 30, angle);

}
void feetNeutral(){
	setMotorAngle(FOOT_FR, 512); // 510
	setMotorAngle(FOOT_FL, 512); // 520
	setMotorAngle(FOOT_BR, 512); // 518
	setMotorAngle(FOOT_BL, 512); // 511

}

void hipNeutral(){
	setMotorAngle(HIP_FR, 512);
	setMotorAngle(HIP_FL, 512);
	setMotorAngle(HIP_BR, 512);
	setMotorAngle(HIP_BL, 512);
}

void posOnBelly(){


}

void posStandingNeutral(){
	feetNeutral();
	hipNeutral();
}


// move leg forward
void moveLegDirection(char leg, char direction, int angle, int speed){

}

/*
void moveForward(int speed){
	moveLeg('FR');
	moveLeg('FL');
	moveLeg('BR');
	moveLeg('BL');
}
*/
