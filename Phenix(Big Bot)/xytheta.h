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

typedef enum{
	Inspection = 0,
	Aquisition = 1,
	Position   = 2,
	Discharge  = 3
}AutonState;

typedef struct
{
		int x;
		int y;
		bool visited;
}ballPosition;

typedef struct
{
  /* Struct to hold robots x, y, and theta relative to a starting position */
  float currX, currY;
  float prevX, prevY;  /* Keep prev data for PID purposes although not used yet */
  float currTheta, prevTheta;
  float turretAngle;
  AutonState state;
  int ballCount;
  int currStack;
  int autonTargetSpeed;
  bool updated;
}robot;
//int autonTargetSpeed; //global targetSpeed set in autonous

typedef struct
{
  /* Struct to hold encoder information and address */
  int currTick;
  int prevTick;
  float gearRatio;
}encoder;

/*  Ball Positions             */
ballPosition balls[5];

/*  Initialize robot position  */
robot bigBot;

/*  Initialize encoders  */
encoder leftEnc, rightEnc;

float referenceAngle(float degrees)
{

	while(degrees > 3600)
	{
		degrees -= 3600;
	}

	while(degrees < 0)
	{
		degrees += 3600;
	}

	return degrees;
}

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
		bigBot.currTheta = referenceAngle(SensorValue[gyro]);//Get angle;

    int distance = ticksToCm(getTicks());//Get distance

		bigBot.currX +=  cosDegrees(bigBot.currTheta / 10) * distance;
		bigBot.currY +=  sinDegrees(bigBot.currTheta / 10) * distance;

		time1[T4] = 0;
    while(time1[T4] < 100){ bigBot.updated = false; } /*  Wait for 100 miliseconds to update */
		bigBot.updated = true;
    bigBot.prevTheta = bigBot.currTheta;
    leftEnc.prevTick = leftEnc.currTick;
    rightEnc.prevTick = rightEnc.currTick;
  }
}


int getTicks()
{

  int driveConfig  = (sgn(motor[leftDrive]) * 10 + sgn(motor[rightDrive]));

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

void setBallPositions()
{
	balls[3].x = 60.96 * 2;
	balls[3].y = 60.96 * 2;
	balls[3].visited = false;

	balls[0].x = 60.96 * 4;
	balls[0].y = 60.96 * 4;
	balls[0].visited = false;

	balls[2].x = 60.96 * 3;
	balls[2].y = 60.96 * 1;
	balls[2].visited = false;

	balls[1].x = 60.96 * 4;
	balls[1].y = 60.96 * 2;
	balls[1].visited = false;


	balls[4].x = 60.96 * 5;
	balls[4].y = 60.96 * 3;
	balls[4].visited = false;

}

void InitialPositioning()
{
	SensorValue[gyro] = 0;
	SensorValue[rightDriveEnc] = 0;
	SensorValue[leftDriveEnc] = 0;

	bigBot.currX =  35.56;
	bigBot.currY =  96.52;
	bigBot.currTheta = 0;
	bigBot.state = Inspection;
	bigBot.ballCount = 0;
}
