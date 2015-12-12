/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Driver Control File
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////

#define DEADZONE 20

int wheelSpeed = 0;
int DriveMode = -1;
int shootMode = 0;
bool isSwitchingModes = false;
bool isSwitchingSpeeds = false;

void drive()
{

	if(vexRT[Btn7L] && !isSwitchingModes)//change modes
	{

			//DriveMode = -DriveMode;
			isSwitchingModes = true;
	}
	else if(vexRT[Btn7L] == 0)
	{
			isSwitchingModes = false;
	}

	if(DriveMode == 1) //arcade drive
	{
		int y = abs(vexRT[Ch3]) < DEADZONE ? 0 : vexRT[Ch3];
		int x = abs(vexRT[Ch1]) < DEADZONE ? 0 : vexRT[Ch1];

		motor[leftBack] = y + x;
		motor[leftCenter] = y + x;
		motor[leftFront] = y + x;

		motor[rightBack] = y - x;
		motor[rightCenter] = y - x;
		motor[rightFront] = y - x;

	}
	else if(DriveMode == -1) //tank drive
	{
		int rightPower = abs(vexRT[Ch2]) < DEADZONE ? 0 : vexRT[Ch2];
		int leftPower  = abs(vexRT[Ch3]) < DEADZONE ? 0 : vexRT[Ch3];

		motor[leftBack] = leftPower;
		motor[leftCenter] = leftPower;
		motor[leftFront] = leftPower;

		motor[rightBack] = rightPower;
		motor[rightCenter] = rightPower;
		motor[rightFront] = rightPower;
	}
}

void ctrl_intake()
{
	int power_in = vexRT[Btn5U] ? 127 : vexRT[Btn5D] ? -127 : 0;

	motor[intake] = power_in;

}

void ctrl_ramp()
{
	int power_in = vexRT[Btn7U] ? 127 : vexRT[Btn7D] ? -127 : 0;

	motor[ramp] = power_in;

}
