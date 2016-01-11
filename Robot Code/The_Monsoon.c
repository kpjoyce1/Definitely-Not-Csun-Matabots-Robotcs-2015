/*
Programmer: Nick MonsonT
Last updated: 4/1/2014
Program: Controls for the 'back up bot'
*/


#pragma config(Motor,  port2,           driveRightBack, tmotorVex393, openLoop)
#pragma config(Motor,  port3,           driveLeftBack, tmotorVex393, openLoop)
#pragma config(Motor,  port8,           driveRightFront, tmotorVex393, openLoop)
#pragma config(Motor,  port9,           driveLeftFront, tmotorVex393, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void controls();

#include "Vex_Competition_Includes.c"   //Main competition background code...do not modify!


int driveRight;
int driveLeft;

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
		controls();
  }
}

void controls()
{
		driveRight = vexRT[Ch2];
		driveLeft = vexRT[Ch3];

		if(driveRight <= 10 && driveRight >= -10)
		{
			motor[driveRightBack] = 0;
			motor[driveRightFront] = 0;
		}
		else
		{
			motor[driveRightBack] = driveRight;
			motor[driveRightFront] = driveRight;
		}

		if(driveLeft <= 10 && driveLeft >= -10)
		{
			motor[driveLeftBack] = 0;
			motor[driveLeftFront] = 0;
		}
		else
		{
			motor[driveLeftBack] = driveLeft;
			motor[driveLeftFront] = -driveLeft;
		}
}