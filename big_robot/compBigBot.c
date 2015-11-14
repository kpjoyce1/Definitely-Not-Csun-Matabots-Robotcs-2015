#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    lineFollower,   sensorLineFollower)
#pragma config(Sensor, in8,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  rightShooterSensor, sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftShooterSensor, sensorQuadEncoder)
#pragma config(Sensor, I2C_1,  leftDriveSensor,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  rightDriveSensor,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Motor,  port1,           leftBack,      tmotorVex393_HBridge, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port2,           leftCenter,    tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           leftFront,     tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           ramp,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           leftShooter,   tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port6,           rightShooter,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           intake,        tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port8,           rightFront,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           rightCenter,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          rightBack,     tmotorVex393_HBridge, openLoop, reversed, encoderPort, I2C_2)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

/*
#pragma config(Sensor, dgtl1,  rightDriveEnc,  sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftDriveEnc,   sensorQuadEncoder)
#pragma config(Sensor, dgtl7,  leftDriveEnc2,  sensorQuadEncoder)
#pragma config(Sensor, dgtl10, rightDriveEnc2, sensorQuadEncoder)
*/

//VEX stuff
#pragma competitionControl(Competition)
#pragma autonomousDuration(60)
#pragma userControlDuration(60)

#include "Vex_Competition_Includes.c"

#define PI 3.1415
#define DEADZONE 20
#define MAXSPEED 120
#define MINSPEED 0
#define NearShot 50
#define FarShot  95
#define PROPORTIONAL_GAIN 1.25 //1.25
#define SAMPLE_PERIOD 100
#define INTEGRAL_CONST 990.0 //990
#define SATURATION_UP 120
#define SATURATION_DOWN 0
#define MOTOR_DEADZONE 20
#define Kp 0.5
#define SAMPLEPERIODDRIVE 20
#define ACCURACYDRIVE 0.05

const float B0 = PROPORTIONAL_GAIN*(1+SAMPLE_PERIOD/(2*INTEGRAL_CONST));
const float B1 = PROPORTIONAL_GAIN*(SAMPLE_PERIOD/(2*INTEGRAL_CONST)-1);

int potencia_motor_esquerdo, potencia_motor_direito;
int velocidade_motor_esquerdo, velocidade_motor_direito;

int line = 0;

int wheelSpeed = 0;
int DriveMode = -1;
int shootMode = 0;
bool isSwitchingModes = false;
bool isSwitchingSpeeds = false;
//Sturctures
////////////////////////////////////////////////////////////////////////////////////
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
  float gearEfficiency;
}encoder;

typedef struct Gyroscope
{
	int currReading;
	int prevReading;
	int cummulativeDrift;
	float driftConstant;
}Gyroscope;


typedef struct {
  tMotor name1; //this have to be rewied, whatever to be that keywork that identify the motor
  tSensors sensorname;
  int speedSet; //speed desired to the motor
  float lastError; //last error calculated for that motor
  float controller_output; //controller output to the motor
  float speedRead; //speed calculated from the encoder values
  int lastEncoderRead; //last encoder read. Necessary to calulate the speed
} MOTOR_PI;


typedef struct {
  tMotor name1, name2, name3; //this have to be rewied, whatever to be that keywork that identify the motor
  tSensors encoder, gyroscope;
  float positionSet; //speed desired to the motor
  float lastError; //last error calculated for that motor
  float controller_output; //controller output to the motor
  float positionRead; //speed calculated from the encoder values
  int lastEncoderRead; //last encoder read. Necessary to calulate the speed
} MOTOR_DRIVE;


////////////////////////////////////////////////////////////////////////////////////////////////////////////

void initMotor (MOTOR_PI* motorA, tMotor title1, tSensors sensor);
void initDriveMotor (MOTOR_DRIVE* motorA, tMotor title1, tMotor title2, tMotor title3, tSensors encoder, tSensors gyroscope);
void shooter(MOTOR_PI* motorA, MOTOR_PI* motorB);
void ControlFunction(MOTOR_PI* motorA, MOTOR_PI* motorB);
void PI_Control(MOTOR_PI* motorA);
void ReadSpeed(MOTOR_PI* motorA, int time);

//Initialize structures
//////////////////////////////////////////////////////////////////////

MOTOR_PI motorLeftShooter, motorRightShooter;
MOTOR_DRIVE motorRightDrive, motorLeftDrive;
//////////////////////////////////////////////////////////////////////

void drive()
{

	if(vexRT[Btn7L] && !isSwitchingModes)//change modes
	{

			DriveMode = -DriveMode;
			isSwitchingModes = true;
	}
	else if(vexRT[Btn7L] == 0)
	{
			isSwitchingModes = false;
	}

	if(DriveMode == 1) //arcade drive
	{
		int y = abs(vexRT[Ch3]) < DEADZONE ? 0 : vexRT[Ch3];
		int x = abs(vexRT[Ch1]) < DEADZONE ? 0 : vexRT[Ch1];

		motor[leftBack] = y + x;
		motor[leftCenter] = y + x;
		motor[leftFront] = y + x;

		motor[rightBack] = y - x;
		motor[rightCenter] = y - x;
		motor[rightFront] = y - x;


	}
	else if(DriveMode == -1) //tank drive
	{
		int rightPower = abs(vexRT[Ch2]) < DEADZONE ? 0 : vexRT[Ch2];
		int leftPower  = abs(vexRT[Ch3]) < DEADZONE ? 0 : vexRT[Ch3];

		motor[leftBack] = leftPower;
		motor[leftCenter] = leftPower;
		motor[leftFront] = leftPower;

		motor[rightBack] = rightPower;
		motor[rightCenter] = rightPower;
		motor[rightFront] = rightPower;
	}
}

////////////////////////////////////////////////////////////////////////////////////

void ctrl_intake()
{
	int power_in = vexRT[Btn5U] ? 127 : vexRT[Btn5D] ? -127 : 0;

	motor[intake] = power_in;

}

////////////////////////////////////////////////////////////////////////////////////6

void ReadSpeed(MOTOR_PI* motorA, int time){
  int encoder;
  encoder = SensorValue[(*motorA).sensorname];
  (*motorA).speedRead = abs(11.1*(float)(encoder - (*motorA).lastEncoderRead)/((float)time) ); //that is not a unit. You should multiply for a constant. I will leave this way by now.
  (*motorA).lastEncoderRead = encoder;
}

void ControlFunction(MOTOR_PI* motorA, MOTOR_PI* motorB){
	int storeTime;
	storeTime=time1[T1];
	if(storeTime >= SAMPLE_PERIOD/2){
		ReadSpeed(motorA, storeTime);
		ReadSpeed(motorB, storeTime);
		clearTimer(T1);
	}
	if(time1[T2] >= SAMPLE_PERIOD){
		PI_Control(motorA);
  	PI_Control(motorB);
		clearTimer(T2);
	}
}


float output;

void ControlDriveFunction(MOTOR_DRIVE* motorA, MOTOR_DRIVE* motorB){
	int sampleA1 = 0;
	int sampleA2 = 0;
	int sampleB1 = 0;
	int sampleB2 = 0;
	//float output;
	float proportion;
	clearTimer(T3);

	while((abs(sampleA1-sampleA2) > ACCURACYDRIVE*motorA->positionSet || abs(sampleB1 - sampleB2) > ACCURACYDRIVE*motorB->positionSet)
		     || (sampleA1<0.5*motorA->positionSet || sampleA1<0.5*motorB->positionSet)){

	  if( time1[T3]  > SAMPLEPERIODDRIVE)
		{
			sampleA2=sampleA1;
			sampleA1 = nMotorEncoder[motorA->encoder];
			sampleB2=sampleB1;
			sampleB1 = -nMotorEncoder[motorB->encoder];
			time1[T3] = 0;
  	}


		proportion = motorA->positionSet - SensorValue[motorA->encoder];
		motorA->controller_output = Kp * proportion;

		if (motorA->controller_output > 1) output = motorA->controller_output+MOTOR_DEADZONE;
		else if(abs(motorA->controller_output) < 1) output=0;
		else output = motorA->controller_output - MOTOR_DEADZONE; // if (motorA->controller_output < -1)
		motor[motorA->name1] = output;
		motor[motorA->name2] = output;
		motor[motorA->name3] = output;

	  proportion = motorB->positionSet + SensorValue[motorB->encoder];
	  motorB->controller_output = Kp * proportion;

		if (motorB->controller_output > 1) output = motorB->controller_output+MOTOR_DEADZONE;
		else if(abs(motorB->controller_output) < 1) output=0;
		else output = motorB->controller_output - MOTOR_DEADZONE;
		motor[motorB->name1] = output;
		motor[motorB->name2] = output;
		motor[motorB->name3] = output;
	}
	motor[motorA->name1] = 0;
	motor[motorA->name2] = 0;
	motor[motorA->name3] = 0;
	motor[motorB->name1] = 0;
	motor[motorB->name2] = 0;
	motor[motorB->name3] = 0;
}

void PI_Control(MOTOR_PI* motorA){
    float erroPast1;
		float output;
    //make the calculation for the controller inputs
    erroPast1 = (*motorA).lastError;
    (*motorA).lastError = (*motorA).speedSet-(*motorA).speedRead;

    //apply control`s law
    (*motorA).controller_output = (*motorA).controller_output+B0*((*motorA).lastError)+B1*erroPast1;

   	// The motor is having a problema when stopping. The controler output does not goes to 0, so it is supplying the motor with power, eventhough it is
		// not running. This is meant to solve this problem
		if ((*motorA).speedSet == 0) (*motorA).controller_output = 0;

    //apply saturation of the controller
    if ( (*motorA).controller_output > SATURATION_UP ) output = SATURATION_UP;
    else if ( (*motorA).controller_output < SATURATION_DOWN ) output = SATURATION_DOWN;
    else if ( (*motorA).controller_output < MOTOR_DEADZONE && (*motorA).controller_output > -MOTOR_DEADZONE ) output = 0;
		else output = (*motorA).controller_output;

    //set power in the motor
    motor[(*motorA).name1] = output;

    //the controler output can`t be less the saturation dowm
    if ( (*motorA).controller_output < SATURATION_DOWN ) (*motorA).controller_output = SATURATION_DOWN;
}
void initMotor (MOTOR_PI* motorA, tMotor title1, tSensors sensor){
		(*motorA).name1 = title1;
		(*motorA).sensorname = sensor;
    (*motorA).speedSet = 0;
    (*motorA).lastError = 0;
    (*motorA).controller_output = 0;
    (*motorA).speedRead = 0.0;
    (*motorA).lastEncoderRead = 0;
}

void initDriveMotor (MOTOR_DRIVE* motorA, tMotor title1, tMotor title2, tMotor title3, tSensors encoder, tSensors gyroscope){
		(*motorA).name1 = title1;
		(*motorA).name2 = title2;
		(*motorA).name3 = title3;
		(*motorA).encoder = encoder;
		(*motorA).gyroscope = gyroscope;
    (*motorA).positionSet = 0;
    (*motorA).lastError = 0;
    (*motorA).controller_output = 0;
    (*motorA).positionRead = 0.0;
    (*motorA).lastEncoderRead = 0;
}


bool RT8U;
bool RT8D;

////////////////////////////////////////////////////////////////////////////////////
void shooter(MOTOR_PI* motorA, MOTOR_PI* motorB)
{

/////////////
	if(vexRT[Btn8U] == 1){
		if(!RT8U) wheelSpeed++;
		RT8U=true;
	}
	else RT8U = false;
///////////////////
	if(vexRT[Btn8R] == 1)
		wheelSpeed = 88;
///////////////////
	if(vexRT[Btn8D] == 1){
		if(!RT8D) wheelSpeed--;
		RT8D=true;
	}
	else RT8D = false;
///////////////////
	if(vexRT[Btn8L] == 1)
		wheelSpeed = 62;

	if(vexRT[Btn6U] == 1)
	{
		(*motorA).speedSet = wheelSpeed;
		(*motorB).speedSet = wheelSpeed;
	}
	else if(vexRT[Btn6D] == 1)
	{
		(*motorA).speedSet = 0;
		(*motorB).speedSet = 0;
	}
	ControlFunction(motorA, motorB);
}

////////////////////////////////////////////////////////////////////////////////////
//Get rid of these functions after this weekend

int autonTargetSpeed; //global targetSpeed set in autonous

task autonShooterControl()
{
	while(true)
	{
		motorLeftShooter.speedSet = autonTargetSpeed;
		motorRightShooter.speedSet = autonTargetSpeed;

		ControlFunction(&motorLeftShooter, &motorRightShooter);
	}
}

void autonMove(int targetDistance,MOTOR_DRIVE* motorA, MOTOR_DRIVE* motorB)
{

	SensorValue[motorA->encoder] = 0;
	SensorValue[motorB->encoder] = 0;

	motorA->positionSet = targetDistance;
	motorB->positionSet = targetDistance;

	ControlDriveFunction(motorA, motorB);

}

void move(int rightPower, int leftPower, int time)
{
	motor[rightBack] = rightPower;
	motor[rightCenter] = rightPower;
	motor[rightFront] = rightPower;

	motor[leftBack] = leftPower;
	motor[leftCenter] = leftPower;
	motor[leftFront] = leftPower;

	time1[T4] = 0;
	while(time1[T4] < time){}


}


void setIntake(int power)
{
 	motor[intake] = power;
}
////////////////////////////////////////////////////////////////////////////////////

void ctrl_ramp()
{
	int power_in = vexRT[Btn7U] ? 127 : vexRT[Btn7D] ? -127 : 0;

	motor[ramp] = power_in;

}



//Main Code
//////////////////////////////////////////////////////////////////////////////////////

/* Pre Auton */
void pre_auton(){

}

/*                          Autonomous                            */
/*________________________________________________________________*/

task autonomous(){

	bool blueSide = true;
  initMotor(&motorLeftShooter, leftShooter, leftShooterSensor);
	initMotor(&motorRightShooter, rightShooter, rightShooterSensor);
	initDriveMotor(&motorLeftDrive, leftFront, leftCenter, leftBack, leftDriveSensor, gyro);
	initDriveMotor(&motorRightDrive, rightFront, rightCenter, rightBack, rightDriveSensor, gyro);


	clearTimer(T1);
  clearTimer(T2);
  clearTimer(T3);
	clearAll(actOnSensors);
	startTask(autonShooterControl);
	if(blueSide)
	{
		autonTargetSpeed = 88;
		move(50, 0, 1800);//started at 1600
		move(0, 0, 0);
		wait1Msec(500);
		for(int i = 0; i < 4; i++)
		{
			wait1Msec(3000);
			setIntake(120);
			wait1Msec(1000);
			setIntake(0);
		}
	}
	else
	{
		autonTargetSpeed = 88;
		move(0, 50, 1800);//started at 1600
		move(0, 0, 0);
		wait1Msec(500);
		for(int i = 0; i < 4; i++)
		{
			wait1Msec(3000);
			setIntake(120);
			wait1Msec(1000);
			setIntake(0);
		}
	}





}

/*                          UserControl                           */
/*________________________________________________________________*/

task usercontrol()
{
  initMotor(&motorLeftShooter, leftShooter, leftShooterSensor);
	initMotor(&motorRightShooter, rightShooter, rightShooterSensor);

	clearTimer(T1);
  clearTimer(T2);

	bLCDBacklight = true;

  while(true)
    {
    	shooter(&motorLeftShooter, &motorRightShooter);
    	ctrl_intake();
    	ctrl_ramp();
    	drive();

    	velocidade_motor_esquerdo=motorLeftShooter.speedRead;
			potencia_motor_esquerdo=motorLeftShooter.controller_output;
			velocidade_motor_direito=motorRightShooter.speedRead;
			potencia_motor_direito=motorRightShooter.controller_output;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////