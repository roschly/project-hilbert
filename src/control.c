void init() {
	DDRA  = 0xFC;
	PORTA = 0xFC;

	serial_initialize(57600);
	dxl_initialize( 0, DEFAULT_BAUDNUM ); // Not using device index
	sei();	// Interrupt Enable

	ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1);	// ADC Enable, Clock 1/64div.
}

int deg2speed(int deg) {
	return deg/100*MAX_SPEED;
}

int calcSpeed(int ir[], int baseSpeed) {
	int speed;

	if (ir[1] > 50) {
		speed = -ir[1];
	} else {
		speed = baseSpeed-ir[0];
		if (speed < 200)
			speed = 200;
	}
	return speed;
}

void setRightSpeed(int speed) {
	int id = 8;

	if (speed > 1023)
		speed = 1023;

	if (speed < -1023)
		speed = -1023;

	if (speed < 0) {
		speed = -1*speed;
	} else {
		speed += 1024;
	}
	dxl_write_word( id , 32, speed);

}

void setLeftSpeed(int speed) {
	int id = 9;

	if (speed > 1023)
		speed = 1023;

	if (speed < -1023)
		speed = -1023;

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

void turnLeftSoft(int speed, int deg) {
	setRightSpeed(speed + deg);
	setLeftSpeed(speed - deg);
}

void turnRightSoft(int speed, int deg) {
	setRightSpeed(speed - deg);
	setLeftSpeed(speed + deg);
}

void turnLeftHard(int deg) {
	setRightSpeed(deg);
	setLeftSpeed(-deg);
}

void turnRightHard(int deg) {
	setRightSpeed(-deg);
	setLeftSpeed(deg);
}


void moveForward(int speed) {
	setRightSpeed(speed);
	setLeftSpeed(speed);
}

void moveBackward(int deg) {
	setRightSpeed(deg2speed(-deg));
	setLeftSpeed(deg2speed(-deg));
}

void bla() {


	// int  k = 70;
	// setRightSpeed((k - sl)*6);
	// setLeftSpeed((k - sr)*6);
}
