
/// Control table address
#define P_GOAL_POSITION_L		30
#define P_GOAL_POSITION_H		31
#define P_PRESENT_POSITION_L	36
#define P_PRESENT_POSITION_H	37
#define P_MOVING				46

// Motor IDs
#define FOOT_FR 1
#define FOOT_BR 2
#define FOOT_FL 3
#define FOOT_BL 4

#define HIP_FR 5
#define HIP_BR 6
#define HIP_FL 7
#define HIP_BL 8

void init() {
	DDRA  = 0xFC;
	PORTA = 0xFC;

	serial_initialize(57600);
	dxl_initialize( 0, 1 ); // Not using device index
	sei();	// Interrupt Enable

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);	// ADC Enable, Clock 1/64div.
}

void setAngle(int id, int angle) {

	if (angle > 450)
		angle = 450;

	if (angle < -0)
		angle = -0;

	dxl_write_word( id , 30, angle);

}