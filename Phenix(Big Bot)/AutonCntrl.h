/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Autonomous Control File
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void SearchState();
void PositioningState();
void turn(float targetRadians);
void move(int rightPower, int leftPower, int time);
void setIntake(int power);
float referenceAngle(float radians);
void lineFollow();
void Roam();

int center = 140;
bool onLeft = false;
int ballCount = 0;
int drivePower = 60;

void SearchState()
{
	while(ballCount < 3)
	{
		if(closestBall.sig != 'N' && closestBall.updated)
		{
			setIntake(127);
			int difference = closestBall.x - center;
			if(closestBall.x < 50)
			{
				difference = abs(difference * 0.5) > 30 ? abs(difference) * 0.5 : 30;
				move(difference, -difference, 100);
			}
			else if(closestBall.x > 200)
			{
				difference = abs(difference * 0.5) > 30 ? abs(difference) * 0.5 : 30;
				move(-difference, difference, 100);
			}
			else
			{
				move(drivePower, drivePower, 100);
			}
			if(closestBall.y > 140)
			{
				setIntake(127);
			}


			closestBall.updated = false;
	  }
	  else
	  {
	  	Roam();
	  }

	  if(SensorValue[leftBumper] || SensorValue[rightBumper])
	  {
	  	move(-45, -45, 1000);
		}

		if(SensorValue[intakeSensor] < 10)
		{
			setIntake(127);
			move(-40, -40, 1);
			while(SensorValue[intakeSensor] < 10) {}
			setIntake(0);
			ballCount++;
		}

	}
	move(0,0, 1);
}

void setIntake(int power)
{
 	motor[intake] = power;
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

float referenceAngle(float radians)
{

	while(radians > 3600)
	{
		radians -= 3600;
	}

	while(radians < 0)
	{
		radians += 3600;
	}

	return radians;
}

void turn(float targetRadians)
{
	float currRadians  = referenceAngle(SensorValue[gyro]);

	float Kp = 0.3;
	float acceptableError = 100;
	float power;

	while(!(currRadians < targetRadians + acceptableError && currRadians > targetRadians  - acceptableError))
	{
		float difference =  currRadians - targetRadians;
		power = abs(difference) * Kp > 20 ? difference * Kp : sgn(difference) * 20;

		motor[rightBack] = power;
		motor[rightCenter] = power;
		motor[rightFront] = power;

		motor[leftBack] = -power;
		motor[leftCenter] = -power;
		motor[leftFront] = -power;

		currRadians = referenceAngle(SensorValue[gyro]);

	}

		power = 0;
		motor[rightBack] = power;
		motor[rightCenter] = power;
		motor[rightFront] = power;

		motor[leftBack] = power;
		motor[leftCenter] = power;
		motor[leftFront] = power;
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

void Roam()
{


	move(40, 80, 100);
}
