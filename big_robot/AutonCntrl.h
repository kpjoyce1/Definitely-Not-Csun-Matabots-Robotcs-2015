/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Autonomous Control File
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

void SearchState();
void move(int rightPower, int leftPower, int time);
void setIntake(int power);

int center = 140;

int ballCount = 0;
int drivePower = 80;

void SearchState()
{
	while(ballCount < 4)
	{
		if(closestBall.sig != 'N' && closestBall.updated)
		{
			int difference = closestBall.x - center;
			if(closestBall.x < 50)
			{
				difference = abs(difference * 0.6) > 30 ? abs(difference) * 0.6 : 30;
				move(difference, -difference, 100);
			}
			else if(closestBall.x > 200)
			{
				difference = abs(difference * 0.6) > 30 ? abs(difference) * 0.6 : 30;
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

			targetBall.x = closestBall.x;
			targetBall.y = closestBall.y;
			targetBall.sig = closestBall.sig;
			targetBall.updated = true;
			closestBall.updated = false;
	  }
	  else if(targetBall.updated && targetBall.sig != 'N')
	  {
	  	int difference = targetBall.x - center;
			if(targetBall.x < 50)
			{
				difference = abs(difference * 0.6) > 30 ? abs(difference) * 0.6 : 30;
				move(difference, -difference, 100);
			}
			else if(targetBall.x > 200)
			{
				difference = abs(difference * 0.6) > 30 ? abs(difference) * 0.6 : 30;
				move(-difference, difference, 100);
			}
			else
			{
				move(drivePower * 0.8, drivePower * 0.8, 100);
			}
			if(targetBall.y > 140)
			{
				setIntake(127);
			}

			targetBall.updated = false;
		}
	  else
	  {
	  	move(0, 0, 1);
		}

		if(SensorValue[intakeSensor] < 10)
		{
			setIntake(127);
			while(SensorValue[intakeSensor] < 10) {}
			setIntake(0);
			ballCount++;
		}


	}
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
