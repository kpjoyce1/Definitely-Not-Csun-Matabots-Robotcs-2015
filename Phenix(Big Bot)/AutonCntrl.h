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
void set1stIntake(int power);
void set2ndIntake(int power);
void lineFollow();

#define centerOfCamera  150
#define criticalDistance 130
#define drivePower  40
//camera is x-y system where
// x - left-> 0  right->480
// y - far -> 0  close->200



bool onLeft = false;
int AquisitionAttempts = 0;


task autonShooterControl()
{
	while(true)
	{
		flywheel.speedSet = bigBot.autonTargetSpeed;
		shooter_Control(&flyWheel);
	}
}

void Cognition()
{

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
	int numOfVisited = 0;
	for(int i = 0; i < 5; i++)
	{
		if(!balls[i].visited)
		{
			bigBot.currStack = i;
			scanLocation(balls[i].x, balls[i].y);
			balls[i].visited = true;
		}
		else
		{
			numOfVisited++;
		}
	}

	if(numOfVisited == 5)
	{
		scanLocation(60.96 * 3, 60.96 * 6);
	}
}

void AquisitionState()
{
		AquisitionAttempts++;
		if(closestBall.sig != 'N' && closestBall.updated || SensorValue[intakeSensor] < 53)
		{
			int difference = closestBall.x - centerOfCamera;
			float gain = 0.3;
			if(closestBall.x < 50)
			{
				difference = abs(difference * gain) > drivePower ? abs(difference) * gain : drivePower;
				move(difference, gain*difference, 100);
			}
			else if(closestBall.x > 200)
			{
				difference = abs(difference * gain) > drivePower ? abs(difference) * gain : drivePower;
				move(gain*difference, difference, 100);
			}
			if(closestBall.y > 190)
			{
				move(drivePower, drivePower, 100);
				set1stIntake(-127);
			}
			else
			{
				move(drivePower, drivePower, 100);
			}

			if(SensorValue[intakeSensor] < 90)
			{
				set1stIntake(-127);
				set2ndIntake(127);
				move(-drivePower, -drivePower, 10);
				while(SensorValue[intakeSensor] < 90) {}
				set2ndIntake(0);
				set1stIntake(0);
				bigBot.ballCount++;
				if(bigBot.ballCount == 2)
				{
					bigBot.state = Position;
				}
				else
				{
					bigBot.state = Inspection;
				}
				move(0, 0, 100);
			}
		}
		else
		{
			if(closestBall.sig == 'N' && AquisitionAttempts >= 2)
			{
				set1stIntake(0);
				set2ndIntake(0);
			}
		}

		if(AquisitionAttempts > 200)
		{
			bigBot.state = Inspection;
		}
}

void PositionState()
{
	turn(angleCheck(60.96*5, 60.955*5));

	while(!SensorValue[leftBumper] && !SensorValue[rightBumper])
	{
		move(100, 100, 1);
	}

	bigBot.autonTargetSpeed = 100;
	bigBot.state = Discharge;
}

void DischargeState()
{
	for(int i = 0; i < 4; i++)
	{
		wait1Msec(1000);
		set2ndIntake(120);
		wait1Msec(800);
		set2ndIntake(0);
	}
	bigBot.ballCount = 0;
	bigBot.currX = 60.96 * 5 - 40;
	bigBot.currY = 60.96 * 5 - 40;

	bigBot.state = Inspection;
}

void set1stIntake(int power)
{
 	motor[intake1stStage] = power;
}

void set2ndIntake(int power)
{
 	motor[intake2ndStage] = power;
}

void move(int rightPower, int leftPower, int time)
{
	motor[rightDrive] = rightPower;
	motor[rightCenterDrive] = rightPower;

	motor[leftDrive] = leftPower;
	motor[leftCenterDrive] = leftPower;

	time1[T4] = 0;
	while(time1[T4] < time){}

}

void PositioningState()
{
	if(bigBot.currX < bigBot.currY)
	{
		turn(3150);
		onLeft = false;
	}
	else if(bigBot.currX > bigBot.currY)
	{
		turn(1350);
		onLeft = true;
	}

	while(SensorValue[lineFollower] > 200)
	{
		move(40, 40, 1);
	}
	move(-40, -40, 100);
	move(0,0, 20);
	turn(450);
	lineFollow();
	move(0,0, 1);

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

 while(targetDistance - currentDistance > 100)
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
			AquisitionAttempts = 0;
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

 while(targetDistance - currentDistance > 40)
 {
  	int power = 50;

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
			if(SensorValue[lineFollower] > 800)
			{
				move(60, 30, 10);
			}
			else
			{
				move(30, 60, 10);
			}
		}
	}
	else
	{
		while(!SensorValue[leftBumper] && !SensorValue[rightBumper])
	 	{
			if(SensorValue[lineFollower] > 800)
			{
				move(30, 60, 10);
			}
			else
			{
				move(60, 30, 10);
			}
		}
	}
}
