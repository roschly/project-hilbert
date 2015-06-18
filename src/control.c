

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
 	//sei(); //Enables global interrupts

	DDRA  = 0xFC;
	PORTA = 0xFC;

	//serial_initialize(57600);
	dxl_initialize( 0, 1 ); // Not using device index
	//sei();	// Interrupt Enable

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);	// ADC Enable, Clock 1/64div.
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

void standingNeutral(){
	// feet
	setAngle(FFR, 0);
	setAngle(FFL, 0);
	setAngle(FBR, 0);
	setAngle(FBL, 0);

	// hips
	setAngle(HFR, 0);
	setAngle(HFL, 0);
	setAngle(HBR, 0);
	setAngle(HBL, 0);
}


// move leg forward
