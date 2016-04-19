/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// X, Y, Theta System File
// Degrees normalized to 0 - 360
// Publishes
// -Robot Orientation in 2-D (x, y, theta)
// -Has function to get reference angle
// -Uses timer T4
/////////////////////////////////////////////////////////////////////////////////////////////////////

//Movement Types
//const int	Forward = 11;
//const int Reverse = -11;
const int  TurnLeft = 9;
const int  TurnRight = -9;
//const int Idle = 0;

task UpdatePosition();
int Average(float value1, float value2);
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
	float X, Y;
	float prevX, prevY;  /* Keep prev data for PID purposes although not used yet */
	float XSpeed, YSpeed;
	float Theta, Drift;
	AutonState state;
	int ballCount;
	int autonTargetSpeed;
	bool updated;
}Robot;
//int autonTargetSpeed; //global targetSpeed set in autonous

typedef struct
{
	/* Struct to hold encoder information and address */
	int currTick;
	int prevTick;
	float gearRatio;
}Encoder;

/*  Ball Positions             */
//ballPosition balls[10];

/*  Initialize robot position  */
Robot bigBot;

/*  Initialize encoders  */
Encoder leftEnc, rightEnc;

task UpdatePosition()
{
	/*
	Use movement type to define how to get distance
	Use gyro to define the angle
	Create vector and add to original values
	*/

	while(true){

		leftEnc.prevTick = leftEnc.currTick;
		rightEnc.prevTick = rightEnc.currTick;

		leftEnc.currTick = SensorValue[leftDriveEnc];
		rightEnc.currTick = SensorValue[rightDriveEnc];

		bigBot.Theta = (SensorValue[gyro] / 10) % 360;

		float distance = ticksToCm(getTicks());//Get distance
		bigBot.XSpeed = cosDegrees(bigBot.Theta) * distance;
		bigBot.YSpeed = sinDegrees(bigBot.Theta) * distance;
		bigBot.X +=  bigBot.XSpeed;
		bigBot.Y +=  bigBot.YSpeed;

		while(time1[T4] < 10) {
			bigBot.Theta = (SensorValue[gyro] / 10) % 360 + bigBot.Drift;
		}
		time1[T4] = 0;
	}
}

int getTicks()
{
	return Average((leftEnc.currTick - leftEnc.prevTick), (rightEnc.currTick - rightEnc.prevTick));
}

int Average(float value1, float value2)
{
	return (value1 + value2) / 2.;
}

float ticksToCm(int ticks)
{
	return 2. * PI * (2.0) * 1.1 * (float)(ticks) / 360.; //2 * pi * r * ratio of angle ticks : 360
}

//void setBallPositions()
//{
//	balls[0].x = 60.96 * 4;
//	balls[0].y = 60.96 * 4;//fudge
//	balls[0].visited = false;

//	balls[1].x = 60.96 * 5;
//	balls[1].y = 60.96 * 3;
//	balls[1].visited = false;

//	balls[2].x = 60.96 * 3;
//	balls[2].y = 60.96 * 1;
//	balls[2].visited = false;

//	balls[3].x = 60.96 * 4;
//	balls[3].y = 60.96 * 2;
//	balls[3].visited = false;

//	balls[4].x = 60.96 * 2;
//	balls[4].y = 60.96 * 2;
//	balls[4].visited = false;

//}

void InitialPositioning()
{
	SensorValue[gyro] = 0;
	SensorValue[rightDriveEnc] = 0;
	SensorValue[leftDriveEnc] = 0;

	bigBot.X =  0;//35.56;
	bigBot.Y =  0;//96.52;
	bigBot.Theta = 0;
	bigBot.Drift = 0;
	bigBot.state = Inspection;
	bigBot.ballCount = 0;
}
