#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(Sensor, in8,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  rightDriveEnc,  sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftDriveEnc,   sensorQuadEncoder)
#pragma config(Motor,  port1,           motorA,        tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           rightDrive,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           leftDrive,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          motorB,        tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/* Things to do
		-Implement a PID movement function
		-Implement a distance function that takes into account the gearRatios and radius of wheel
		-Implement a function that finds the distance between two points
		-Implement a function that turns to an angle
		-Implement a function that changes the turretAngle to the proper angle to shoot at a specific distance
		-Implement vision tracking or visual recognition, most likely on an arduino
*/

//VEX stuff
#pragma competitionControl(Competition)
#pragma autonomousDuration(60)
#pragma userControlDuration(60)

#include "Vex_Competition_Includes.c"

//Define static values
#define PI 3.14159265358979323846264338327
#define DEADZONE  10
#define speed 127

typedef struct robot
{
  /* Struct to hold robots x, y, and theta relative to a starting position */
  float currX, currY;
  float prevX, prevY;  /* Keep prev data for PID purposes although not used yet */
  float currTheta, prevTheta;
  float turretAngle;
}robot;

typedef struct encoder
{
  /* Struct to hold encoder information and address */
  int currTick;
  int prevTick;
  float gearRatio;
}encoder;

typedef struct Gyroscope
{
	int currReading;
	int prevReading;
	int cummulativeDrift;
	float driftConstant;
}Gyroscope;


/*  Movement types

Planning on adding movements
for holonomic drive
                    */

const int	Forward = 11;
const int  Reverse = -11;
//const int  TurnLeft = 9;
//const int  TurnRight = -9;
//const int  Stop = 10;




//User Control Tasks and Methods
void drive();



//Autonomous Tasks and Methods
//void move(int x, int y);
void turn(float targetRadians);

//Mapping Tasks and Methods
/*
  Mapping will be using an x,y, theta system
  The initial start of the vehicle will determine
  the x direction with the reference angle along
  the x axis, perpendicular to this will be the
  y axis. The initial position will be measured
  and after this the position will be updated.
  Currently the method of updating will be through
  gyroscope and drive encoders
*/

task updatePosition();
void zeroOutSensors();
int average(float value1, float value2);
int getTicks();
void initializeSturctures(robot *pos, encoder *left, encoder *right, Gyroscope *scope);
float ticksToCm(int ticks);

/*  Initialize robot position  */
struct robot robotBase;


/*  Initialize encoders  */
struct encoder leftEnc, rightEnc;

/*  Initialize gyroscope */
struct Gyroscope gyroscope;

/* Pre Auton */
void pre_auton(){

}


/*                          Autonomous                            */
/*________________________________________________________________*/

task autonomous(){

	startTask(updatePosition);

	turn(PI/2.);

	wait1Msec(2000);

	turn(0);

}


/*                          User Control                          */
/*________________________________________________________________*/

task usercontrol()
{
  zeroOutSensors();

	initializeSturctures(robotBase, leftEnc, rightEnc, gyroscope);

  startTask(updatePosition);
	bLCDBacklight = true;

  while(true)
    {
    	clearLCDLine(0);
  		clearLCDLine(1);

    	drive();

    	char angle[20];
    	sprintf(angle, "Angle: %.5f", robotBase.currTheta);
  		displayLCDCenteredString(1, angle);

			char position[20];
      sprintf(position, "%.2f,%.2f",robotBase.currX, robotBase.currY);
      displayLCDCenteredString(0, position);
      wait1Msec(30);
    }
}



/*                             Funtions                           */
/*________________________________________________________________*/

void zeroOutSensors()
{
  SensorValue[gyro] = 0;
  SensorValue[leftDriveEnc] = 0;
  SensorValue[rightDriveEnc] = 0;
}

void initializeSturctures(robot *pos, encoder *left, encoder *right, Gyroscope *scope)
{
	pos->currX = 0;
	pos->currY = 0;
	pos->prevX = 0;
	pos->prevY = 0;
	pos->currTheta = 0;
	pos->prevTheta = 0;

	left->currTick = 0;
	left->prevTick = 0;
	left->gearRatio = 1;

	right->currTick = 0;
	right->prevTick = 0;
	right->gearRatio = 1;

	scope->currReading = 0;
	scope->prevReading = 0;

	scope->cummulativeDrift = 0;
	scope->driftConstant = 1;

}

void drive()
{
  int Y1 = abs(vexRT[Ch3]) < DEADZONE ? 0 : vexRT[Ch3];
  int X2 = abs(vexRT[Ch1]) < DEADZONE ? 0 : vexRT[Ch1];
  motor[leftDrive] =  Y1 + X2;    /*   leftPower  */
  motor[rightDrive] = - Y1 + X2;   /*   rightPower */
}

void turn(float targetRadians)
{
	/*
		Autonomous Function
		PID Attempt
		P- Difference in the angle
		I- May not be necessary in this instance
		D- Derivative will be change in angle since last update
	*/

	float Kp = 80.;
	float Kd = 80.;

	while(!(robotBase.currTheta < targetRadians + 0.004 && robotBase.currTheta > targetRadians - 0.004))
	{
			float proportion = targetRadians - robotBase.currTheta;

			float derivative = (robotBase.currTheta - robotBase.prevTheta) / 50.;

			float output = Kp * proportion + Kd * derivative;

			motor[leftDrive] = output;

			motor[rightDrive] = output;

			time1[T4] = 0;

			while(time1[T4] < 50){}

	}




}

task updatePosition()
{
  /*
    Use movement type to define how to get distance
    Use gyro to define the angle
    Create vector and add to original values
   */

  while(true){
    leftEnc.currTick = SensorValue[leftDriveEnc];
    rightEnc.currTick = SensorValue[rightDriveEnc];


    int distance = ticksToCm(getTicks());//Get distance

    robotBase.currTheta = SensorValue[gyro] / 10. * PI / 180.;//Get angle

    robotBase.currX +=  cos(robotBase.currTheta) * distance;
    robotBase.currY +=  sin(robotBase.currTheta) * distance;

    time1[T3] = 0;

    while(time1[T3] < 80){} /*  Wait for 80 miliseconds to update */

    robotBase.prevTheta = robotBase.currTheta;
    leftEnc.prevTick = leftEnc.currTick;
    rightEnc.prevTick = rightEnc.currTick;
  }
}


int getTicks()
{

  int driveConfig  = (sgn(motor[leftDrive]) * 10 - sgn(motor[rightDrive]));

	if(driveConfig == Forward || driveConfig == Reverse){
    return average((leftEnc.currTick - leftEnc.prevTick), (rightEnc.currTick - rightEnc.prevTick));
  }
  else //if(driveConfig == TurnLeft || driveConfig == TurnRight || driveConfig == Stop)
  {
    return 0; /* Only angle should change unless stopped */
  }

}

int average(float value1, float value2)
{
  return (value1 + value2) / 2.;
}

float ticksToCm(int ticks)
{
		return 2. * PI * (5.08) * (float)(ticks) / 360.;
}
