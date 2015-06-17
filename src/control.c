//


/// Control table address
#define P_GOAL_POSITION_L		30
#define P_GOAL_POSITION_H		31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING				46

// Motor IDs
#define FFR 3
#define FBR 5
#define FFL 1
#define FBL 4

#define HFR 6
#define HBR 2
#define HFL 8
#define HBL 7

void init() {
	// set timer
	TCCR1B = (1<<CS11)|(1<<CS10); //64 Prescaler
 	OCR1A = 249; //Equals delay 1 ms
 	TIMSK1 = (1<<OCIE1A); //Enable Output Compare Match Interrupt
 	TCNT1 = 0; //reset timer/counter 1
 	sei(); //Enables global interrupts

	DDRA  = 0xFC;
	PORTA = 0xFC;

	serial_initialize(57600);
	dxl_initialize( 0, 1 ); // Not using device index
	sei();	// Interrupt Enable

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);	// ADC Enable, Clock 1/64div.
}



// degree: int [-100:100]
void setAngle(int id, double degree){
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


	double angle;

	//feet
	int maxFootAngleInterval = 150;
	if (id == 1 || id == 3 || id == 4 || id == 5){
		if (id == 5 || id == 3){
			angle = (degree/100) * maxFootAngleInterval;
		}
		else{
			angle = (degree/100) * maxFootAngleInterval;
		}
	}

	// hips
	int maxHipAngleInterval = 60;
	if (id == 2 || id == 6 || id == 7 || id == 8){
		if (id == 6 || id == 7){
			angle = (degree/100) * maxHipAngleInterval;
		}
		else {
			angle = (degree/100) * maxHipAngleInterval;
		}
	}


	dxl_write_word(id, 30, angle+512);

}

/*
// onBelly, standing
void setPredefinedPos(int pos){
	// on belly
	if (pos == 1){
		setAngle
	}

	// standing neutral
	if (pos == 2){

	}
}



void posStandingNeutral(){
	feetNeutral();
	hipNeutral();
}
*/

// move leg forward
void moveLeg(char leg, char direction, int angle, int speed){


}

/*
void moveForward(int speed){
	moveLeg('FR');
	moveLeg('FL');
	moveLeg('BR');
	moveLeg('BL');
}
*/
