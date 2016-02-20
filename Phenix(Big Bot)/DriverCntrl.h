/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Driver Control File
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////
#include "phenix_PID.h"

#define DEADZONE 10

int wheelSpeed = 0;
bool upPress, downPress;
MOTOR_PI flyWheel;

void drive();
void cntrl_intake1();
void cntrl_intake2();
void shooter(MOTOR_PI* motorA);

void drive()
{

	int leftPower = abs(vexRT[Ch3]) > DEADZONE ? vexRT[Ch3] : 0;

	motor[leftDrive]=leftPower;
	motor[leftCenterDrive]=leftPower;

	int rightPower = abs(vexRT[Ch2]) > DEADZONE ? vexRT[Ch2] : 0;

	motor[rightDrive]=rightPower;
	motor[rightCenterDrive]=rightPower;
}

void cntrl_intake1()
{
	 motor[intake1stStage] = vexRT[Btn5U] ? 127 : vexRT[Btn5D] ? -127 : 0;
}

void cntrl_intake2()
{
	 motor[intake2ndStage] = vexRT[Btn6U] ? 127 : vexRT[Btn6D] ? -127 : 0;
}

void shooter(MOTOR_PI* motorA)
{

///////////////////
	if(vexRT[Btn8R])
	{
		wheelSpeed = 95;
	}

	if(vexRT[Btn8L])
	{
		if(!downPress)
		{
			wheelSpeed--;
			downPress = true;
		}
	}
	else
	{
		downPress = false;
	}

	if(vexRT[Btn8U])
	{
		if(!upPress)
		{
			wheelSpeed++;
			upPress = true;
		}
	}
	else
	{
		upPress = false;
	}

	if(vexRT[Btn8D])
	{
		wheelSpeed = 0;
	}

		(*motorA).speedSet = wheelSpeed;
		shooter_Control(motorA);
}
