/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// X, Y, Theta System File
//
// -Uses timer T4
/////////////////////////////////////////////////////////////////////////////////////////////////////

//Movement Types
//const int	Forward = 11;
//const int Reverse = -11;
const int  TurnLeft = 9;
const int  TurnRight = -9;
//const int Idle = 0;

task updatePosition();
int average(float value1, float value2);
int getTicks();
float ticksToCm(int ticks);
void InitialPositioning();

typedef struct
{
  /* Struct to hold robots x, y, and theta relative to a starting position */
  float currX, currY;
  float prevX, prevY;  /* Keep prev data for PID purposes although not used yet */
  float currTheta, prevTheta;
  float turretAngle;
}robot;

typedef struct
{
  /* Struct to hold encoder information and address */
  int currTick;
  int prevTick;
  float gearRatio;
}encoder;

/*  Initialize robot position  */
robot bigBot;

/*  Initialize encoders  */
encoder leftEnc, rightEnc;


task updatePosition()
{
  /*
    Use movement type to define how to get distance
    Use gyro to define the angle
    Create vector and add to original values
   */

  while(true){
    leftEnc.currTick = SensorValue[leftDriveEnc];
    rightEnc.currTick = SensorValue[rightDriveEnc];
		bigBot.currTheta = SensorValue[gyro] / 10. * PI / 180.;//Get angle;

    int distance = ticksToCm(getTicks());//Get distance

		bigBot.currX +=  cos(bigBot.currTheta) * distance;
		bigBot.currY +=  sin(bigBot.currTheta) * distance;

		time1[T4] = 0;
    while(time1[T4] < 100){} /*  Wait for 100 miliseconds to update */

    bigBot.prevTheta = bigBot.currTheta;
    leftEnc.prevTick = leftEnc.currTick;
    rightEnc.prevTick = rightEnc.currTick;
  }
}


int getTicks()
{

  int driveConfig  = (sgn(motor[leftCenter]) * 10 + sgn(motor[rightCenter]));

	if(driveConfig != TurnLeft && driveConfig != TurnRight){
    return average((leftEnc.currTick - leftEnc.prevTick), (rightEnc.currTick - rightEnc.prevTick));
  }
  else //if(driveConfig == TurnLeft || driveConfig == TurnRight || driveConfig == Stop)
  {
    return 0; /* Only angle should change unless stopped */
  }

}

int average(float value1, float value2)
{
  return (value1 + value2) / 2.;
}

float ticksToCm(int ticks)
{
		return 2. * PI * (5.08) * (float)(ticks) / 360.; //2 * pi * r * ratio of angle ticks : 360
}

void InitialPositioning()
{
	SensorValue[gyro] = 0;
	SensorValue[rightDriveEnc] = 0;
	SensorValue[leftDriveEnc] = 0;

	bigBot.currX =  35.56;
	bigBot.currY =  96.52;
	bigBot.currTheta = 0;

}
