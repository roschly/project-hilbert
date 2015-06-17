//##########################################################
//##                      R O B O T I S                   ##
//## CM-510 (Atmega2561) Example code for Dynamixel.      ##
//##                                           2009.11.10 ##
//##########################################################

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#include "dynamixel.h"
#include "serial.h"
#include "control.c"

// Motor IDs
#define FOOT_FR 1
#define FOOT_BR 2
#define FOOT_FL 3
#define FOOT_BL 4

#define HIP_FR 5
#define HIP_BR 6
#define HIP_FL 7
#define HIP_BL 8

void PrintCommStatus(int CommStatus);
void PrintErrorCode(void);

int main(void)
{
	unsigned short GoalPos[2] = {200, 450};
	//unsigned short GoalPos[2] = {0, 4095}; // for EX series
	int index = 0;
	int id = 1;
	int bMoving, wPresentPos;
	int CommStatus;

	init();

	setAngle(FOOT_FR, 200);
	setAngle(FOOT_FL ,200);

	while(1)
	{
		// // Check moving done
		// bMoving = dxl_read_byte( id, P_MOVING );
		// CommStatus = dxl_get_result();
		// if( CommStatus == COMM_RXSUCCESS )
		// {
		// 	if( bMoving == 0 )
		// 	{
		// 		// Change goal position
		// 		if( index == 0 )
		// 			index = 1;
		// 		else
		// 			index = 0;

		// 		// Write goal position
		// 		setRightFrontFoot(GoalPos[index]);
		// 		setRightBackFoot(GoalPos[!index]);		
		// 	}
			
		// 	PrintErrorCode();
			
		// 	// Read present position
		// 	wPresentPos = dxl_read_word( id, P_PRESENT_POSITION_L );
		// 	printf( "%d   %d\n",GoalPos[index], wPresentPos );
		// }
		// else
		// 	PrintCommStatus(CommStatus);
		
	}

	return 0;
}

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

