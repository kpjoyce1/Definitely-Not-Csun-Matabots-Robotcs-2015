/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Autonomous Control File
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void InspectionState();
void AquisitionState();
void PositionState();
void DischargeState();
void turn(float targetDegrees);
float Distance(float x, float y);
float angleCheck(float x, float y);
void move(int rightPower, int leftPower, int time);
void moveToLocation(float x, float y);
void scanLocation(float x, float y);
void set1stIntake(int intakePower);
void set2ndIntake(int intakePower);
bool DQCheck();
void intakeSequence();
void lineFollow();

#define centerOfCamera  150
#define criticalDistance 130
#define drivePower  50
#define lightValue 2000
//camera is x-y system where
// x - left-> 0  right->480
// y - far -> 0  close->200



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

	if(DQCheck())
	{
		moveToLocation(3*60.96, 3*60.96);
	}
	switch(bigBot.state)
	{
			case Inspection :
				InspectionState();
				break;

			case Aquisition :
				AquisitionState();
				break;

			case Position :
				PositionState();
				break;

			case Discharge :
				DischargeState();
				break;
	}

}

void InspectionState()
{

	if(closestBall.sig != 'N' && Distance(balls[bigBot.currStack].x,balls[bigBot.currStack].y) < 60)
	{
		bigBot.state = Aquisition;
	}
	else
	{

	int numVisited = 0;
	for(int i = 0; i < 5; i++)
	{
		if(!balls[i].visited)
		{
			bigBot.currStack = i;
			moveToLocation(balls[i].x, balls[i].y);
			if(Distance(balls[i].x, balls[i].y) < 30)
			{
				balls[i].visited = true;
			}
			if(closestBall.sig != 'N')
			{
				bigBot.state = Aquisition;
			}
			break;
		}
		else
		{
			numVisited++;
		}
	}
	if(numVisited == 5)
	{
		scanLocation(60.96*3, 60.96*3);
	}
	}
}

void intakeSequence()
{
				set1stIntake(-127);
				set2ndIntake(127);
				move(drivePower, drivePower, 0);

				while(SensorValue[intakeSensor] < 100) {}
				move(0,0, 300);
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
				move(-drivePower, -drivePower, 100);
}

void AquisitionState()
{
		if(SensorValue[leftBumper] || SensorValue[rightBumper])
		{
			move(-100, -100, 500);
			turn(bigBot.currTheta + 180);
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
				targetBall.x = closestBall.x;
				targetBall.y = closestBall.y;
				set2ndIntake(127);
				float gain = 0.2;

					if(targetBall.x < 60)
					{
						move(drivePower, drivePower*gain, 200);
					}
					else if(targetBall.x > 200)
					{
						move(drivePower*gain, drivePower, 200);
					}
					else
					{
						move(drivePower, drivePower, 200);
					}

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

void PositionState()
{

	turn(angleCheck(5*60.96, 5*60.96));
	//fudgeFactor+=3;
	move(drivePower, drivePower, 0);

	while(!SensorValue[leftBumper] && !SensorValue[rightBumper]) {}

	if(!SensorValue[leftBumper])
	{
		while(!SensorValue[leftBumper])
		{
			move(0, 127, 1);
		}
	}
	else if(!SensorValue[rightBumper])
	{
		while(!SensorValue[leftBumper])
		{
			move(127, 0, 1);
		}
	}
	//bar sensor
	// 77 - 124

	if(SensorValue[barSensor] > 125)
	{
		move(-100, -100, 500);
		move(50, 30, 40);
		while(!SensorValue[leftBumper] && !SensorValue[rightBumper]) {}
		move(0,0,0);
	}

	else if(SensorValue[barSensor] < 77)
	{
	move(-100, -100, 500);
	move(30, 50, 40);
	while(!SensorValue[leftBumper] && !SensorValue[rightBumper]) {}
	move(0,0,0);
	}


	bigBot.autonTargetSpeed = 120;
	bigBot.state = Discharge;

}

void DischargeState()
{
	for(int i = 0; i < 4; i++)
	{
		wait1Msec(1500);
		set1stIntake(-127);
		wait1Msec(500);
		set1stIntake(0);
	}

	move(-drivePower, -drivePower, 1000);
	bigBot.autonTargetSpeed = 0;
	bigBot.ballCount = 0;

	bigBot.state = Inspection;
}

void set1stIntake(int intakePower)
{
 	motor[intake1stStage] = intakePower;
}

void set2ndIntake(int intakePower)
{
 	motor[intake2ndStage] = intakePower;
}

void move(int rightPower, int leftPower, int time)
{
	motor[rightDrive] = rightPower;
	motor[rightCenterDrive] = rightPower;

	motor[leftDrive] = leftPower;
	motor[leftCenterDrive] = leftPower;

	wait1Msec(time);

}

bool DQCheck()
{
	return Distance(60.96*1,60.96*6) < 60.96*2.5
}

float angleCheck(float x, float y)
{
	return   radiansToDegrees( atan2((y - bigBot.currY) ,(x - bigBot.currX)));
}


void scanLocation(float x, float y)
{
 float targetAngle = angleCheck(x, y);
 turn(targetAngle);

 float startX = bigBot.currX;
 float startY = bigBot.currY;
 float targetDistance = Distance(x,y);
 float currentDistance = 0;

 while(targetDistance - currentDistance > 40 && !DQCheck())
 {
  	int power = 50;

		motor[rightDrive] = power;
		motor[rightCenterDrive] = power;

		motor[leftDrive] = power;
		motor[leftCenterDrive] = power;

		wait1Msec(100);
		if(closestBall.sig != 'N')
		{
			bigBot.state = Aquisition;
			move(0,0,0);
			break;
		}
		currentDistance = Distance(startX, startY);
 }


 		motor[rightDrive] = 0;
		motor[rightCenterDrive] = 0;

		motor[leftDrive] = 0;
		motor[leftCenterDrive] = 0;
}

void moveToLocation(float x, float y)
{
 float targetAngle = angleCheck(x, y);
 turn(targetAngle);
 float startX = bigBot.currX;
 float startY = bigBot.currY;
 float targetDistance = Distance(x,y);
 float currentDistance = 0;

 while(targetDistance - currentDistance > 100)
 {
   	if(SensorValue[leftBumper] || SensorValue[rightBumper] || DQCheck())
   	{
   		move(-100, -100, 0);
   		break;
   	}
  	int power = drivePower;

		motor[rightDrive] = power;
		motor[rightCenterDrive] = power;

		motor[leftDrive] = power;
		motor[leftCenterDrive] = power;

		wait1Msec(150);

		currentDistance = Distance(startX, startY);
 }

 		motor[rightDrive] = 0;
		motor[rightCenterDrive] = 0;

		motor[leftDrive] = 0;
		motor[leftCenterDrive] = 0;
}

float Distance(float x, float y)
{
	return sqrt((x-bigBot.currX)*(x-bigBot.currX)+(y-bigBot.currY)*(y-bigBot.currX));
}

void turn(float targetDegrees)
{
	targetDegrees = referenceAngle(targetDegrees * 10);
	float pGain = 0.060;
	float iGain = 1.01;
	float samplePeriod = 100;
	int Saturation_Up = 100;
	int Saturation_Down = 42;
	float acceptableError = 50;
	float power;
	float previousError = 0;
	bool toggle = false;
	float currentError;

	while(!(bigBot.currTheta < targetDegrees + acceptableError && bigBot.currTheta > targetDegrees  - acceptableError))
	{
		//In VGOD we trust
		// output = output +  lastError + currentError
		//(*motorA).controller_output = A0*(*motorA).controller_output+B0*((*motorA).errorK0speed)+B1*errorK1speed;
		//power = power + X1 * lastError  + X2 * error;
		currentError =  bigBot.currTheta - targetDegrees;
		power = pGain * currentError;
		if(abs(power) < Saturation_Down)
		{
			power = sgn(power)*Saturation_Down;
		}
		else if(abs(power) > Saturation_Up)
		{
			power = sgn(power)*Saturation_Up;
		}

		motor[rightDrive] = power;
		motor[rightCenterDrive] = power;

		motor[leftDrive] = -power;
		motor[leftCenterDrive] = -power;

		previousError = currentError;
		wait1Msec(samplePeriod);

	}

		power = 0;
		motor[rightDrive] = power;
		motor[rightCenterDrive] = power;

		motor[leftDrive] = power;
		motor[leftCenterDrive] = power;
}

void lineFollow()
{

	if(!onLeft)
	{

	 	while(!SensorValue[leftBumper] && !SensorValue[rightBumper])
	 	{
			if(SensorValue[lineFollower] > lightValue)
			{
				move(40, 10, 10);
			}
			else
			{
				move(10, 40, 10);
			}
		}
	}
	else
	{
		while(!SensorValue[leftBumper] && !SensorValue[rightBumper])
	 	{
			if(SensorValue[lineFollower] > lightValue)
			{
				move(10, 40, 10);
			}
			else
			{
				move(40, 10, 10);
			}
		}
	}
}
