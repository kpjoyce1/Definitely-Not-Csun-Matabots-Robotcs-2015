#pragma config(Sensor, dgtl1,  leftSolenoid,   sensorDigitalOut)
#pragma config(Sensor, dgtl2,  rightSolenoid,  sensorDigitalOut)
#pragma config(Sensor, dgtl9,  leftShooterSensor, sensorQuadEncoder)
#pragma config(Sensor, dgtl11, rightShooterSensor, sensorQuadEncoder)
#pragma config(Motor,  port1,           leftDrive,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           leftDrive1,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           rightShooter1, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           rightShooter2, tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           leftIntake,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           rightIntake,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port7,           leftShooter1,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port8,           leftShooter2,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port9,           rightDrive1,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          rightDrive,    tmotorVex393_HBridge, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//


//VEX stuff
#pragma competitionControl(Competition)
#pragma autonomousDuration(60)
#pragma userControlDuration(60)

#include "Vex_Competition_Includes.c"

#define PROPORTIONAL_GAIN 1.48

#define CONTROLLER_ZERO 1.75
#define CONTROLLER_POLE 1.18

#define SAMPLE_PERIOD 100
#define INTEGRAL_CONST 896.9 //990

#define SATURATION_UP 120
#define SATURATION_DOWN 0
#define DEADZONE 10
#define MOTOR_DEADZONE 20
//Mano, na boa, quem sabe o que eh essa porra dessa variavel do nome do motor definida no pragma. Agora todo mundo sabe
//http://robotics.stackexchange.com/questions/751/confused-about-the-variables-in-robotc
//for PID
const float A0 = 1;
const float B0 = PROPORTIONAL_GAIN*(1+SAMPLE_PERIOD/(2*INTEGRAL_CONST));
const float B1 = PROPORTIONAL_GAIN*(SAMPLE_PERIOD/(2*INTEGRAL_CONST)-1);

//for compensattor
//const float A0 = (2-CONTROLLER_POLE*SAMPLE_PERIOD)/(2+CONTROLLER_POLE*SAMPLE_PERIOD);
//const float B0 = PROPORTIONAL_GAIN*(2+CONTROLLER_ZERO*SAMPLE_PERIOD)/(2+CONTROLLER_POLE*SAMPLE_PERIOD);
//const float B1 = PROPORTIONAL_GAIN*(-2+CONTROLLER_ZERO*SAMPLE_PERIOD)/(2+CONTROLLER_POLE*SAMPLE_PERIOD);

//for debug purpose
int potencia_motor_esquerdo, potencia_motor_direito;
int velocidade_motor_esquerdo, velocidade_motor_direito;
int autonTargetSpeed;

typedef struct {
  tMotor name1, name2; //this have to be rewied, whatever to be that keywork that identify the motor
  tSensors sensorname;
  int speedSet; //speed desired to the motor
  float lastError; //last error calculated for that motor
  float controller_output; //controller output to the motor
  float speedRead; //speed calculated from the encoder values
  int lastEncoderRead; //last encoder read. Necessary to calulate the speed
} MOTOR_PI;

void initMotor (MOTOR_PI* motorA, tMotor title1, tMotor title2, tSensors sensor);
void drive();
void intake();
void shooter(MOTOR_PI* motorA, MOTOR_PI* motorB);
task autonShooterControl();
void ControlFunction(MOTOR_PI* motorA, MOTOR_PI* motorB);
void PI_Control(MOTOR_PI* motorA);
void ReadSpeed(MOTOR_PI* motorA, int time);

void move(int rightPower, int leftPower, int time);
void setIntake(int power);

int wheelSpeed;
MOTOR_PI motorLeftShooter, motorRightShooter;

bool testStart = false;

/* Pre Auton */
void pre_auton(){
}

/* Autonomous */
task autonomous(){

	bool blueSide = true;
	initMotor(&motorLeftShooter, leftShooter1, leftShooter2, leftShooterSensor);
	initMotor(&motorRightShooter, rightShooter1, rightShooter2, rightShooterSensor);
	clearAll(actOnSensors);
	startTask(autonShooterControl);
	if(blueSide)
	{
		autonTargetSpeed = 72;


		for(int i = 0; i < 6; i++)
		{
			wait1Msec(5000);
			setIntake(120);
			wait1Msec(1500);
			setIntake(0);
		}
	}
	autonTargetSpeed = 0;
	while(true){}

}

/* User Control */
task usercontrol()
{
	initMotor(&motorLeftShooter, leftShooter1, leftShooter2, leftShooterSensor);
	initMotor(&motorRightShooter, rightShooter1, rightShooter2, rightShooterSensor);
  clearAll(actOnSensors);
	clearTimer(T1);
  clearTimer(T2);
	while(true)
	{
		drive();
		intake();
		shooter(&motorLeftShooter, &motorRightShooter);

		if(testStart)
		{
			if(time1[T3] > 100)
			{
				writeDebugStreamLine("%5d,%5d,%5d",time1[T4], motorLeftShooter.speedRead, motorLeftShooter.controller_output);
				time1[T3] = 0;
			}
		}

		velocidade_motor_esquerdo=motorLeftShooter.speedRead;
		potencia_motor_esquerdo=motorLeftShooter.controller_output;
		velocidade_motor_direito=motorRightShooter.speedRead;
		potencia_motor_direito=motorRightShooter.controller_output;
	}
}



void drive()
{

	if(vexRT[Ch3]>DEADZONE || vexRT[Ch3]<-DEADZONE)
	{
		motor[leftDrive]=vexRT[Ch3];
		motor[leftDrive1]=vexRT[Ch3];
	}
	else
	{
		motor[leftDrive]=0;
		motor[leftDrive1]=0;
	}
	if(vexRT[Ch2]>DEADZONE || vexRT[Ch2]<-DEADZONE)
	{
		motor[rightDrive]=vexRT[Ch2];
		motor[rightDrive1]=vexRT[Ch2];
	}
	else
	{
		motor[rightDrive]=0;
		motor[rightDrive1]=0;
	}
}

void shooter(MOTOR_PI* motorA, MOTOR_PI* motorB)
{
	bool RT8U;
	bool RT8D;
/////////////
	if(vexRT[Btn8U] == 1){
		if(!RT8U) wheelSpeed++;
		RT8U=true;
	}
	else RT8U = false;
///////////////////
	if(vexRT[Btn8R] == 1)
	{
		wheelSpeed = 78;

		time1[T4] = 0;
	}
///////////////
	if(vexRT[Btn8L] == 1){
		if(!RT8D) wheelSpeed--;
		RT8D=true;
	}
	else RT8D = false;
///////////////
	if(vexRT[Btn8D] == 1)
		wheelSpeed = 50;

	if(vexRT[Btn6U] == 1)
	{
		(*motorA).speedSet = wheelSpeed;
		(*motorB).speedSet = wheelSpeed;
		testStart = true;
		clearDebugStream();
	}
	else if(vexRT[Btn6D] == 1)
	{
		(*motorA).speedSet = 0;
		(*motorB).speedSet = 0;
		testStart = false;

	}
	ControlFunction(motorA, motorB);
}

void intake()
{
	if(vexRT[Btn5U]==1)
	{
		motor[leftIntake] = 120;
		motor[rightIntake] = 120;
	}
	if(vexRT[Btn5D]==1)
	{
		motor[leftIntake] = 0;//-120
		motor[rightIntake] = 0;//-120
  }
	else if (vexRT[Btn5U]!=1 && vexRT[Btn5D]!=1)
	{
//		motor[leftIntake] = 0;
//		motor[rightIntake] = 0;
	}

}



void ReadSpeed(MOTOR_PI* motorA, int time){
  int encoder;
  encoder = SensorValue[(*motorA).sensorname];
  (*motorA).speedRead = abs(5.88235*(float)(encoder - (*motorA).lastEncoderRead)/((float)time) ); //that is not a unit. You should multiply for a constant. I will leave this way by now.
  (*motorA).lastEncoderRead = encoder;

  //time speed controlleroutput

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

void PI_Control(MOTOR_PI* motorA){
    float erroPast1;
		float output;
    //make the calculation for the controller inputs
    erroPast1 = (*motorA).lastError;
    (*motorA).lastError = (*motorA).speedSet-(*motorA).speedRead;

    //apply control`s law
    (*motorA).controller_output = A0*(*motorA).controller_output+B0*((*motorA).lastError)+B1*erroPast1;

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
    motor[(*motorA).name2] = output;

    //the controler output can`t be less the saturation dowm
    if ( (*motorA).controller_output < SATURATION_DOWN ) (*motorA).controller_output = SATURATION_DOWN;
}
void initMotor (MOTOR_PI* motorA, tMotor title1, tMotor title2, tSensors sensor){
		(*motorA).name1 = title1;
		(*motorA).name2 = title2;
		(*motorA).sensorname = sensor;
    (*motorA).speedSet = 0;
    (*motorA).lastError = 0;
    (*motorA).controller_output = 0;
    (*motorA).speedRead = 0.0;
    (*motorA).lastEncoderRead = 0;
}

task autonShooterControl()
{
	while(true)
	{
		motorLeftShooter.speedSet = autonTargetSpeed;
		motorRightShooter.speedSet = autonTargetSpeed;

		ControlFunction(&motorLeftShooter, &motorRightShooter);

		velocidade_motor_esquerdo=motorLeftShooter.speedRead;
		potencia_motor_esquerdo=motorLeftShooter.controller_output;
		velocidade_motor_direito=motorRightShooter.speedRead;
		potencia_motor_direito=motorRightShooter.controller_output;
	}
}

void move(int rightPower, int leftPower, int time)
{
	motor[rightDrive] = rightPower;
	motor[rightDrive1] = rightPower;
	motor[leftDrive] = leftPower;
	motor[leftDrive1] = leftPower;

	time1[T3] = 0;
	while(time1[T3] < time){}
}

void setIntake(int power)
{
 	motor[rightIntake] = power;
 	motor[leftIntake] = power;
}
