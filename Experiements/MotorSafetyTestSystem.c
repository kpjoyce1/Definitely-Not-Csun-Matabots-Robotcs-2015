#pragma config(Sensor, in1,    Ball,           sensorAnalog)
#pragma config(Motor,  port2,           SMLL,          tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port3,           SMLR,          tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           SMRL,          tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port5,           SMRR,          tmotorVex393_MC29, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//
/*
System works through the usage of simultaneous tasks.
One task continously manages the current motor power as well as the desired direction of control.  It then generates a value to apply to the current motor power level.
The other task applies the changes to motor power at set intervals.
This allows a gradual increase or decrease in motor speed so as to ease strain on the internal systems of the motors and prevent them from stalling.
The system also enables the implementation of a short power drop after firing so as to keep the motors from being damaged by the rapid drop in wheel speed.


*/
int changeAmount = 20;
int change;
int timeSegment = 250;
int motorSpeed = 0;
bool ready;
bool fired;

task ControlMoniter()
{
	while(true)
	{
		int speedUp = vexRT[Btn6U];
		int slowDown = vexRT[Btn6D];
		ready = false;
		if(speedUp && !slowDown)
		{
			if(motorSpeed <= 120)
				change = changeAmount;
			else
				change = 0;
		}
		else if(slowDown && !speedUp)
		{
			if(motorSpeed >= -120)
				change = (-1)*changeAmount;
			else
				change = 0;
		}
		else
		{
			if(motorSpeed > 10)
			{
				change = -10;
			}
			else if(motorSpeed < -10)
			{
				change = 10;
			}
			else
			{
				change = 0 - motorSpeed;
			}
		}
		if(motorSpeed > 120)
		{
			motorSpeed = 120;
		}
		if(motorSpeed < -120)
		{
			motorSpeed = -120;
		}
		if(Ball && !fired)
		{
		motorSpeed = motorSpeed/2;
		fired = true;
		}
		else if(abs(motorSpeed) >= 120 && fired)
		{
		fired = false;
		}

		ready = true;
		wait1Msec(1);
	}
}



task main()
{
	startTask(ControlMoniter);
	while(true)
	{
		while(ready == false)
		{}
		motorSpeed = motorSpeed + change;

		motor[SMLL] = motorSpeed;
		motor[SMLR] = motorSpeed;
		motor[SMRL] = motorSpeed;
		motor[SMRR] = motorSpeed;
		wait1Msec(timeSegment);

	}





}
