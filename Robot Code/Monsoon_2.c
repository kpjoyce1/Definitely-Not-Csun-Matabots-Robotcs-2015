#pragma config(Motor,  port1,           manipulatorRight1, tmotorVex393, openLoop)
#pragma config(Motor,  port2,           chassisLeftBack, tmotorVex393, openLoop)
#pragma config(Motor,  port3,           chassisLeftFront, tmotorVex393, openLoop)
#pragma config(Motor,  port4,           chassisRightBack, tmotorVex393, openLoop)
#pragma config(Motor,  port5,           chassisRightFront, tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port6,           manipulatorLeft1, tmotorVex393, openLoop)
#pragma config(Motor,  port7,           manipulatorLeft2, tmotorVex393, openLoop)
#pragma config(Motor,  port8,           endEffectorClaw, tmotorVex393, openLoop)
#pragma config(Motor,  port9,           endEffectorWrist, tmotorVex393, openLoop)
#pragma config(Motor,  port10,          manipulatorRight2, tmotorVex393, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
Programmer: Nick Monson
Last updated: 10/3/2014
Program: Controls for the 'practice bot'
*/

//void driveControlsTank();
void arcadeDrive();
void liftControls();
void claw();

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!


int driveRight;
int driveLeft;

int lift;

void pre_auton()
{
}
task autonomous()
{
  //Execute Arduino Autonomous
}

task usercontrol()
{
  while (true)
  {
		//driveControlsTank();
  	arcadeDrive();
  	liftControls();
		claw();
  }
}

void claw()
{
	if(vexRT[Btn5U]==1)
		motor[endEffectorClaw] = 50; //adjust to what you need
	else
		if(vexRT[Btn5D]==1)
			motor[endEffectorClaw] = -50;
	else
		motor[endEffectorClaw] = 0;

	if(vexRT[Btn6U]==1)
		motor[endEffectorWrist] = 50; //adjust to what you need
	else
		if(vexRT[Btn6D]==1)
			motor[endEffectorWrist] = -50;
	else
		motor[endEffectorWrist] = 0;


}

void liftControls()
{
		lift = vexRT[Ch3];

		if(lift <= 10 && lift >= -10)
		{
			motor[manipulatorRight1] = 0;
			motor[manipulatorRight2] = 0;
			motor[manipulatorLeft1] = 0;
			motor[manipulatorLeft2] = 0;
		}
		else
		{
			motor[manipulatorRight1] = lift;
			motor[manipulatorRight2] = lift;
			motor[manipulatorLeft1] = -lift;
			motor[manipulatorLeft2] = -lift;
		}
}


void arcadeDrive()
{
	int fwd, lSpeed, rSpeed;
	int turn;
	int driveCh1 = vexRT[Ch1];
	int driveCh2 = vexRT[Ch2];

  if ((driveCh1 >= -10) && (driveCh1 <= 10)){
    fwd = 0;
  }
  else if(driveCh1 > 10){
    fwd = (127/117) * driveCh1 - (1270/117);
  }
  else if(driveCh1 < -10){
    fwd = (127/117) * driveCh1 + (1270/117);
  }
  if((driveCh2 >= -10) && (driveCh2 <= 10)){
    turn = 0;
  }
  else if(driveCh2 > 10){
    turn = (127/117) * driveCh2 - (1270/117);
  }
  else if(driveCh2 < -10){
    turn =  (127/117) * driveCh2 + (1270/117);
  }

  lSpeed = fwd + turn;
  rSpeed = fwd - turn;

  if(lSpeed > 127) lSpeed = 127;
  else if(lSpeed < -127) lSpeed = -127;

  if(rSpeed > 127) rSpeed = 127;
  else if(rSpeed < -127) rSpeed = -127;

			motor[chassisRightBack] = -rSpeed;
			motor[chassisRightFront] = -rSpeed;
			motor[chassisLeftBack] = lSpeed;
			motor[chassisLeftFront] = lSpeed;
}


void driveControlsTank()
{
		driveRight = vexRT[Ch2];
		driveLeft = vexRT[Ch3];

		if(driveRight <= 10 && driveRight >= -10)
		{
			motor[chassisRightBack] = 0;
			motor[chassisRightFront] = 0;
		}
		else
		{
			motor[chassisRightBack] = driveRight;
			motor[chassisRightFront] = driveRight;
		}

		if(driveLeft <= 10 && driveLeft >= -10)
		{
			motor[chassisLeftBack] = 0;
			motor[chassisLeftFront] = 0;
		}
		else
		{
			motor[chassisLeftBack] = driveLeft;
			motor[chassisLeftFront] = driveLeft;
		}
}
