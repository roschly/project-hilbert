#define F_CPU 16000000UL // CM-510 runs at 16 MHz.
#include <avr/io.h>
#include <util/delay.h>
/**
* Example on how to turn on the power LED on the
* CM510 (Atmega2561)
*/
int main() {
	DDRC = 0x11; //Set PC0 as output (others as input)
	while(1) {
		PORTC = 0x00; //Set PC0 to low (turn on led)
		_delay_ms(100); //wait 1000 ms = 1 sec
		PORTC = 0x01; //Set PC0 to high (turn off led)
		_delay_ms(100); //wait 1000 ms = 1 sec
	}
	return 0;
}
