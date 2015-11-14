#pragma config(UART_Usage, UART1, uartVEXLCD, baudRate19200, IOPins, None, None)
#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, in1,    lineFollower,   sensorLineFollower)
#pragma config(Sensor, in8,    gyro,           sensorGyro)
#pragma config(Sensor, dgtl1,  leftShooterSensor, sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  rightShooterSensor, sensorQuadEncoder)
#pragma config(Sensor, I2C_1,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
#pragma config(Sensor, I2C_2,  ,               sensorQuadEncoderOnI2CPort,    , AutoAssign)
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
#define PROPORTIONAL_GAIN 0.25 //1.25
#define SAMPLE_PERIOD 100
#define INTEGRAL_CONST 90.0 //990
#define SATURATION_UP 120
#define SATURATION_DOWN 0
#define MOTOR_DEADZONE 20

int wheelSpeed = 0;
int DriveMode = -1;
int shootMode = 0;
bool isSwitchingModes = false;
bool isSwitchingSpeeds = false;

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



////////////////////////////////////////////////////////////////////////////////////
//Get rid of these functions after this weekend

void move(int rightPower, int leftPower, int time)
{
		motor[leftBack] = leftPower;
		motor[leftCenter] = leftPower;
		motor[leftFront] = leftPower;

		motor[rightBack] = rightPower;
		motor[rightCenter] = rightPower;
		motor[rightFront] = rightPower;

		time1[T4] = 0;
		while(time1[T4] < time)
		{

		}
}

void setShooterSpeed(int shootPower)
{
	motor[rightShooter] = shootPower;
	motor[leftShooter] = shootPower;
}

void setIntake(int intakePower)
{
	motor[intake] = intakePower;
}

int shootSpeed;
int currShooterSpeed;
int prevEncTick;

void ctrl_shooter()
{

    if(vexRT[Btn8R] == 1)
    {
        shootSpeed = 0;
    }
    else if(vexRT[Btn8U] == 1)
    {
        shootSpeed = 40;
    }
		else if(vexRT[Btn8D] == 1)
    {
        shootSpeed = 100;
		}

		if(vexRT[Btn6D] && !isSwitchingSpeeds)
    {
        shootSpeed += 5;
        isSwitchingSpeeds = true;
    }
    else if(vexRT[Btn6U] && !isSwitchingSpeeds)
    {
        shootSpeed -= 5;
        isSwitchingSpeeds = true;
	  }

    if(!vexRT[Btn6U] && !vexRT[Btn6D])
		{
				isSwitchingSpeeds = false;
		}

		motor[leftShooter] = shootSpeed;
    motor[rightShooter] = shootSpeed;

  	while(time1[T4] <= 100){}
  	int currTick = SensorValue[leftShooterSensor];
  	currShooterSpeed = (currTick - prevEncTick) / (10);

  	prevEncTick = currTick;
  	time1[T4] = 0;
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


}

/*                          UserControl                           */
/*________________________________________________________________*/
int speedRead, speedReadr, encoder, encoderr, last, lastr, timenow;
task usercontrol()
{
	clearTimer(T1);
  clearTimer(T2);
 	SensorValue[leftShooterSensor] = 0;
	bLCDBacklight = true;

  while(true)
    {

    	ctrl_intake();
    	ctrl_ramp();
    	drive();
    	ctrl_shooter();
			timenow = time1[T1];
			if(timenow>300){
				encoder = SensorValue[leftShooterSensor];
	    	speedRead = abs((1000.0*(encoder - last))/timenow); //that is not a unit. You should multiply for a constant. I will leave this way by now.
	    	last = encoder;
				encoderr = SensorValue[rightShooterSensor];
	    	speedReadr = abs((1000.0*(encoderr - lastr))/timenow); //that is not a unit. You should multiply for a constant. I will leave this way by now.
	    	lastr = encoderr;

	    	clearTimer(T1);
			}
		}
}

//////////////////////////////////////////////////////////////////////////////////////////////////
