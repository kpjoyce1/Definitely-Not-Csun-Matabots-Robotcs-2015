#pragma config(Motor,  port2,           leftDrive,     tmotorVex393, openLoop)
#pragma config(Motor,  port3,           leftLift,      tmotorVex393, openLoop)
#pragma config(Motor,  port4,           rightLift,     tmotorVex393, openLoop)
#pragma config(Motor,  port5,           leftInTake,    tmotorVex393, openLoop)
#pragma config(Motor,  port6,           rightInTake,   tmotorVex393, openLoop)
#pragma config(Motor,  port9,           rightDrive,    tmotorVex393, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define speed 120
#define deadZone 10

void tank();
void lift();
void inTake();
//void arcade();

task main()
{
	while(true)
	{
		tank();
		//arcade();
		lift();
		inTake();
	}
}

void tank()
{
	int Y1 = vexRT[Ch3];
	int Y2 = vexRT[Ch2];
	if(abs(Y1) < deadZone)
	{
		Y1 = 0;
	}
	if(abs(Y2) < deadZone)
	{
		Y2 = 0;
	}
	motor[leftDrive] = Y1;
	motor[rightDrive] = Y2;
}

/*
void arcade()
{
	int Y1 = vexRT[Ch3];
	int X1 = vexRT[Ch4];
	if(abs(Y1) < deadZone)
	{
		Y1 = 0;
	}
	if(abs(X1) < deadZone)
	{
		X1 = 0;
	}
	motor[leftDrive] = Y1 + X1;
	motor[rightDrive] = Y1 - X1;
}
*/

void lift()
{
	int up = vexRT[Btn5U];
	int down = vexRT[Btn5D];
	int liftSpeed;
	if(up == 1)
	{
		liftSpeed = speed;
	}
	else if(down == 1)
	{
		liftSpeed = -speed;
	}
	else
	{
		liftSpeed = 0;
	}
	motor[leftLift] = liftSpeed;
	motor[rightLift] = liftSpeed;
}

void inTake()
{
	int open = vexRT[Btn6U];
	int close = vexRT[Btn6D];
	int inTakeSpeed;
	if(open == 1)
	{
		inTakeSpeed = speed;
	}
	else if(close == 1)
	{
		inTakeSpeed = -speed;
	}
	else
	{
		inTakeSpeed = 0;
	}
	motor[leftInTake] = inTakeSpeed;
	motor[rightInTake] = inTakeSpeed;
}
