#pragma config(Motor,  port2,           rightFront,    tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port3,           leftFront,     tmotorVex393, openLoop)
#pragma config(Motor,  port4,           liftRight,     tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port5,           inTakeMotor,   tmotorVex393, openLoop)
#pragma config(Motor,  port6,           wrist,         tmotorVex393, openLoop)
#pragma config(Motor,  port7,           liftLeft,      tmotorVex393, openLoop)
#pragma config(Motor,  port8,           rightBack,     tmotorVex393, openLoop, reversed)
#pragma config(Motor,  port9,           leftBack,      tmotorVex393, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int deadZone = 20;
int speed = 120;

void drive();
void lift();
void rotation();

task main(){
	while(true){
		drive();
		lift();
		rotation();
	}
}

void drive(){
	int Y1 = vexRT[Ch3];
	int X1 = vexRT[Ch4];
	int X2 = vexRT[Ch1];
	if(abs(Y1) < deadZone)
		Y1 = 0;
	if(abs(X1) < deadZone)
		X1 = 0;
	if(abs(X2) < deadZone)
		X2 = 0;

	motor[leftFront] = Y1 + X2 + X1;
	motor[leftBack] = Y1 + X2 - X1;
	motor[rightFront] = Y1 - X2 - X1;
	motor[rightBack] = Y1 - X2 + X1;
}

void lift(){
	int liftUp = vexRT[Btn5U];
	int liftDown = vexRT[Btn5D];
	int liftSpeed;
	if(liftUp == 1)
		liftSpeed = speed;
	else if(liftDown == 1)
		liftSpeed = -speed;
	else
		liftSpeed = 0;

	motor[liftLeft] = liftSpeed;
	motor[liftRight] = liftSpeed;
}

void rotation(){
	int rotaOut = vexRT[Btn6D];
	int rotaIn = vexRT[Btn6U];
	int rotaSpeed;
	if(rotaOut == 1)
		rotaSpeed = -speed;
	else if(rotaIn == 1)
		rotaSpeed = speed;
	else
		rotaSpeed = 0;
	motor[wrist] = rotaSpeed;
}
