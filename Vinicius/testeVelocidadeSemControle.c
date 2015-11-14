#pragma config(I2C_Usage, I2C1, i2cSensors)
#pragma config(Sensor, dgtl1,  leftSolenoid,   sensorDigitalOut)
#pragma config(Sensor, dgtl2,  rightSolenoid,  sensorDigitalOut)
#pragma config(Sensor, dgtl11, rightShooterSensor, sensorQuadEncoder)
#pragma config(Motor,  port1,           leftDrive,     tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           leftDrive1,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port5,           leftIntake,    tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port6,           rightIntake,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port3,           rightShooter1,  tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port4,           rightShooter2,  tmotorVex393_MC29, openLoop, encoderPort, I2C_1)
#pragma config(Motor,  port9,           rightDrive1,   tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          rightDrive,    tmotorVex393_HBridge, openLoop)


void drive();
void intake();
void shooter();
int deadzone;
int wheelSpeed;
int speedReadr, encoderr, lastr, timenow;
int checktime;
task main()
{

	clearTimer(T1);
	while(true)
	{
		drive();
		intake();
		shooter();
		timenow = time1[T1];
		if(timenow>=30){
			encoderr = SensorValue[rightShooterSensor];
    	speedReadr = abs((1000.0*(encoderr - lastr))/timenow); //that is not a unit. You should multiply for a constant. I will leave this way by now.
    	lastr = encoderr;
    	checktime=timenow;
    	clearTimer(T1);
		}

   }
}


void drive()
{
	deadzone=10;

	if(vexRT[Ch3]>deadzone || vexRT[Ch3]<-deadzone)
	{
		motor[leftDrive]=vexRT[Ch3];
		motor[leftDrive1]=vexRT[Ch3];
	}
	else
	{
		motor[leftDrive]=0;
		motor[leftDrive1]=0;
	}
	if(vexRT[Ch2]>deadzone || vexRT[Ch2]<-deadzone)
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

void shooter()
{

	if(vexRT[Btn8U] == 1)
		wheelSpeed = 100;
	else if(vexRT[Btn8R] == 1)
		wheelSpeed = 80;
	else if(vexRT[Btn8D] == 1)
		wheelSpeed = 50;
	else if(vexRT[Btn8L] == 1)
		wheelSpeed = 30;
	if(vexRT[Btn6U] == 1)
	{

		motor[rightShooter1] = wheelSpeed;

		motor[rightShooter2] = wheelSpeed;
	}
	else if(vexRT[Btn6D] == 1)
	{
		motor[rightShooter1] = 0;

		motor[rightShooter2] = 0;
	}
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
		motor[leftIntake] = -120;
		motor[rightIntake] = -120;
  }
	else if (vexRT[Btn5U]!=1 && vexRT[Btn5D]!=1)
	{
		motor[leftIntake] = 0;
		motor[rightIntake] = 0;
	}

}
