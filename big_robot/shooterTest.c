#pragma config(Motor,  port1,           rightTop,      tmotorVex393_HBridge, openLoop)
#pragma config(Motor,  port2,           rightBot,      tmotorVex393_MC29, openLoop, reversed)
#pragma config(Motor,  port9,           leftBot,       tmotorVex393_MC29, openLoop)
#pragma config(Motor,  port10,          leftTop,       tmotorVex393_HBridge, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

#define POWER 118

void controlShooter();

task main()
{
	while(true)
	{
		controlShooter();
	}
}

void controlShooter()
{
	int power = vexRT[Btn6U] ? POWER : vexRT[Btn6D] ? -1 * POWER : 0;

	motor[rightTop] = power;
	motor[rightBot] = power;
	motor[leftTop] = power;
	motor[leftBot] = power;

}
