

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
	sei();	// Interrupt Enable

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);	// ADC Enable, Clock 1/64div.
}

// move leg forward
