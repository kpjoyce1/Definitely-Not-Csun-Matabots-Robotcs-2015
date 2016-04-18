/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Autonomous Control File
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

//void InspectionState();
void AquisitionState();
//void PositionState();
//void DischargeState();
void Turn(float targetDegrees);
float Distance(float x, float y, float x1, float y1);
float angleCheck(float x, float y);
void Move(int rightPower, int leftPower, int time);
void moveToLocation(float x, float y);
void set1stIntake(int intakePower);
void set2ndIntake(int intakePower);
void ApproachBall(float x, float y);
void Stop();
void Unload();
//bool DQCheck();
void intakeSequence();
//void lineFollow();

#define drivePower  50
#define lightValue 2000

#define Saturation_Up  100
#define Saturation_Down  42
//camera is x-y system where
// x - left-> 0  right->480
// y - far -> 0  close->200

bool BallLock = false;
int BallLockXDistance;
int BallLockYDistance
bool onLeft = false;
int AquisitionAttempts = 0;
int fudgeFactor = 1;

task autonShooterControl()
{
	while(true)
	{
		flyWheel.speedSet = bigBot.autonTargetSpeed;
		shooter_Control(&flyWheel);
	}
}

void Cognition()
{

	//if(DQCheck())
	//{
	//	moveToLocation(3*60.96, 3*60.96);
	//}
	//switch(bigBot.state)
	//{
	//case Inspection :
	//	InspectionState();
	//	break;

	//case Aquisition :
	//	AquisitionState();
	//	break;

	//case Position :
	//	PositionState();
	//	break;

	//case Discharge :
	//	DischargeState();
	//	break;
	//}

}

void TrackBall()
{
	if(closestBall.sig == 'G' || closestBall.sig == 'O')
	{
		set1stIntake(-127);
		ApproachBall(closestBall.x, closestBall.y);
	}

}

void intakeSequence()
{
	BallLock = false;
	set1stIntake(-127);
	set2ndIntake(-127);
	//Move(drivePower, drivePower, 0);

	while(SensorValue[intakeSensor] < 150) {}
	Stop();
	bigBot.ballCount++;
	if(bigBot.ballCount >= 1)
	{
		bigBot.state = Position;
	}
	else
	{
		bigBot.state = Inspection;
	}
	set1stIntake(0);
	set2ndIntake(0);
	Stop();
}

void AquisitionState()
{
	if(SensorValue[leftBumper] || SensorValue[rightBumper])
	{
		Move(-100, -100, 500);
		Turn(bigBot.Theta + 180);
	}
	else
	{
		if(closestBall.sig != 'N')
		{
			for(int i = 0; i < 500; i++)
			{
				if(SensorValue[leftBumper] || SensorValue[rightBumper])
				{
					break;
				}
				//targetBall.x = closestBall.x;
				//targetBall.y = closestBall.y;
				set2ndIntake(127);
				float gain = 0.2;


				if(SensorValue[intakeSensor] < 90)
				{
					intakeSequence();
					break;
				}
				wait1Msec(25);
			}
		}
		else if(SensorValue[intakeSensor] < 90)
		{
			intakeSequence();
		}
		else
		{
			bigBot.state = Inspection;
		}
	}
}

void set1stIntake(int intakePower)
{
	motor[intake2ndStage] = intakePower;
}

void set2ndIntake(int intakePower)
{
	motor[intake1stStage] = intakePower;
}

void Move(int rightPower, int leftPower, int time)
{
	motor[rightDrive] = rightPower;
	motor[rightCenterDrive] = rightPower;

	motor[leftDrive] = leftPower;
	motor[leftCenterDrive] = leftPower;

	wait1Msec(time);

}


float angleCheck(float x, float y)
{
	return   radiansToDegrees(atan2((y - bigBot.Y) ,(x - bigBot.X)));
}

void moveToLocation(float x, float y)
{
	float targetAngle = angleCheck(x, y);
	Turn(targetAngle);
	float targetDistance = Distance(x, y, bigBot.X, bigBot.Y);
	float Error = targetDistance;
	float Kpd = 0.5;
	float prevX, prevY;

	prevX = bigBot.X;
	prevY = bigBot.Y;

	while(Error > 2)
	{
		int power = Error * Kpd;

		if((power) < Saturation_Down)
		{
			power = Saturation_Down;
		}
		else if((power) > Saturation_Up)
		{
			power = Saturation_Up;
		}

		motor[rightDrive] = power;
		motor[rightCenterDrive] = power;

		motor[leftDrive] = power;
		motor[leftCenterDrive] = power;

		Error = targetDistance - Distance(prevX, prevY, bigBot.X, bigBot.Y);
		wait1Msec(50);
	}
	motor[rightDrive] = 0;
	motor[rightCenterDrive] = 0;

	motor[leftDrive] = 0;
	motor[leftCenterDrive] = 0;
}


void ApproachBall(float x, float y)
{
	float targetAngle = angleCheck(x, y);
	Turn(targetAngle);
	float targetDistance = Distance(x, y, bigBot.X, bigBot.Y);
	float Error = targetDistance;
	float Kpd = 0.5;
	float prevX, prevY;

	prevX = bigBot.X;
	prevY = bigBot.Y;

	while(Error > 2)// && SensorValue[intakeSensor] > 120)
	{
		int power = Error * Kpd;

		if((power) < Saturation_Down)
		{
			power = Saturation_Down;
		}
		else if((power) > Saturation_Up)
		{
			power = Saturation_Up;
		}

		motor[rightDrive] = power;
		motor[rightCenterDrive] = power;

		motor[leftDrive] = power;
		motor[leftCenterDrive] = power;

		Error = targetDistance - Distance(prevX, prevY, bigBot.X, bigBot.Y);
		wait1Msec(50);
	}
	motor[rightDrive] = 0;
	motor[rightCenterDrive] = 0;

	motor[leftDrive] = 0;
	motor[leftCenterDrive] = 0;
}

float Distance(float x, float y, float x1, float y1)
{
	return sqrt((x-x1)*(x-x1)+(y-y1)*(y-y1));
}

void Turn(float targetDegrees)
{
	float Error = targetDegrees - bigBot.Theta;
	while(Error < -180)
	{
		Error += 360;
	}
	while(Error > 180)
	{
		Error -= 360;
	}
	float pGain = 0.5;
	float samplePeriod = 75;
	float acceptableError = 1;
	float power;
	float previousError = 0;

	while(abs(Error) > acceptableError)
	{
		Error =  targetDegrees - bigBot.Theta;
		while(Error < -180)
		{
			Error += 360;
		}
		while(Error > 180)
		{
			Error -= 360;
		}

		power = pGain * Error;
		power = abs(power) < 50 ? sgn(power) * 50 : power;

		motor[rightDrive] = power;
		motor[rightCenterDrive] = power;

		motor[leftDrive] = -power;
		motor[leftCenterDrive] = -power;

		wait1Msec(samplePeriod);

	}
	Stop();
}

void Stop()
{
	int power = 0;
	motor[rightDrive] = power;
	motor[rightCenterDrive] = power;

	motor[leftDrive] = power;
	motor[leftCenterDrive] = power;
}

void Unload()
{
	wait1Msec(1000);
	set1stIntake(-127);
  set2ndIntake(-127);
	wait1Msec(3000);

	bigBot.ballCount = 0;

	set1stIntake(0);
	set2ndIntake(0);
	bigBot.autonTargetSpeed = 0;
}
