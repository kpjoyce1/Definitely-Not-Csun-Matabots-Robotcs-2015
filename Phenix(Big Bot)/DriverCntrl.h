/////////////////////////////////////////////////////////////////////////////////////////////////////
//
// Driver Control File
//
//
/////////////////////////////////////////////////////////////////////////////////////////////////////


#define DEADZONE 10

int shootSpeed = 0;


void drive();
void cntrl_intake1();
void cntrl_intake2();
void shooter();


void drive()
{

	int leftPower = abs(vexRT[Ch2]) > DEADZONE ? vexRT[Ch2] : 0;

	motor[leftDrive]=leftPower;
	motor[leftCenter]=leftPower;

	int rightPower = abs(vexRT[Ch3]) > DEADZONE ? vexRT[Ch2] : 0;

	motor[rightDrive]=rightPower;
	motor[rightCenter]=rightPower;
}

void cntrl_intake1()
{
	 motor[intake1stStage] = vexRT[Btn5U] ? 127 : vexRT[Btn5D] ? -127 : 0;
}

void cntrl_intake2()
{
	 motor[intake2ndStage] = vexRT[Btn6U] ? 127 : vexRT[Btn6D] ? -127 : 0;
}

void shooter()
{
	shootSpeed = vexRT[Btn8R] ? 100 : vexRT[Btn8D] ? 0 : shootSpeed;

	motor[shooterTop] = shootSpeed;
	motor[shooterMid] = shootSpeed;
	motor[shooterBot] = shootSpeed;

}
