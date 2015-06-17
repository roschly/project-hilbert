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
#include "control.c"

// Motor IDs
#define FFR 3
#define FBR 5
#define FFL 1
#define FBL 4

#define HFR 6
#define HBR 2
#define HFL 8
#define HBL 7

void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);

int main(void)
{


	init();

	//setAngle(FOOT_FR, 0);
	//setAngle(FOOT_FL, 0);

	//feetNeutral();
	//hipNeutral();
	setAngle(FFR, 0);
	setAngle(FFL, 0);
	setAngle(FBR, 0);
	setAngle(FBL, 0);


	setAngle(HFR, -100);
	setAngle(HFL, 100);
	setAngle(HBR, 100);
	setAngle(HBL, -100);

	while(1)
	{

		//printf("%i - %i - %i - %i - %i - %i - %i - %i \n", dxl_read_word(1,36), dxl_read_word(2,36), dxl_read_word(3,36), dxl_read_word(4,36), dxl_read_word(5,36), dxl_read_word(6,36), dxl_read_word(7,36), dxl_read_word(8,36));
		// hips
		//printf("%i - %i - %i - %i \n", dxl_read_word(2,36), dxl_read_word(6,36), dxl_read_word(7,36), dxl_read_word(8,36));
		//_delay_ms(2500);

		//moveForward(speed);



	}

	return 0;
}
