#pragma config(Sensor, in1,    gyro,           sensorGyro)
#pragma config(Sensor, in6,    lineFollower,   sensorLineFollower)
#pragma config(Sensor, dgtl1,  barSensor,      sensorSONAR_cm)
#pragma config(Sensor, dgtl3,  leftBumper,     sensorTouch)
#pragma config(Sensor, dgtl4,  rightBumper,    sensorTouch)
#pragma config(Sensor, dgtl5,  intakeSensor,   sensorSONAR_mm)
#pragma config(Sensor, dgtl7,  leftDriveEnc,   sensorQuadEncoder)
#pragma config(Sensor, dgtl9,  rightDriveEnc,  sensorQuadEncoder)
#pragma config(Sensor, dgtl11, shooterEnc,     sensorQuadEncoder)
#pragma config(Motor,  port1,           rightCenterDrive, tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           rightDrive,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           shooterTop,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           intake2ndStage, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           shooterMid,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port7,           intake1stStage, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           shooterBot,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           leftDrive,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          leftCenterDrive, tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#include "Vex_Competition_Includes.c"
#include "phenix_UART.h"
#include "phenix_PID.h"
#include "xytheta.h"
#include "DriverCntrl.h"
#include "AutonCntrl.h"

/* Autonomous */
void pre_auton()
{

}

task autonomous()
{
	//clear sensors
	clearAll(actOnSensors);

	//start x, y, theta system
	startTask(updatePosition);

	//PID autonomous beginning
	shooter_MotorInit(&flyWheel, shooterTop, shooterMid, shooterBot, shooterEnc);
	startTask(autonShooterControl);
	bigBot.autonTargetSpeed = 0;

	//start pixy communication
 	configureSerial();
	startTask(UARTReceive);

	//Initial positioning
	setBallPositions();
	InitialPositioning();

	//move to the center of the field
  moveToLocation(2*60.96-30, 3*60.96-30);

  //Bot is initially searching for balls
	bigBot.state = Inspection;

	while(true)
	{
		Cognition();
	}

}

/* User Control */
task usercontrol()
{

	shooter_MotorInit(&flyWheel, shooterTop, shooterMid, shooterBot, shooterEnc);

	while(true)
	{
		drive();
		cntrl_intake1();
		cntrl_intake2();
		shooter(&flyWheel);
	}
}
