#pragma config(Sensor, dgtl1,  rightBackEncode, sensorQuadEncoder)
#pragma config(Sensor, dgtl3,  leftBackEncode, sensorQuadEncoder)
#pragma config(Motor,  port2,           rightFront,    tmotorVex393, openLoop)
#pragma config(Motor,  port3,           leftFront,     tmotorVex393, openLoop)
#pragma config(Motor,  port8,           rightBack,     tmotorVex393, openLoop)
#pragma config(Motor,  port9,           leftBack,      tmotorVex393, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

int speed = 120;
int flag = 0;
void leftTurn();
void rightTurn();
void resetEnodeValues();

task main()
{
	leftTurn();
	flag = 0;
	resetEnodeValues();
	wait10Msec(200);
	rightTurn();
	resetEnodeValues();
	wait10Msec(200);

}

void leftTurn(){
	while(flag != 1){
		if(SensorValue[leftBackEncode] >= -500 && SensorValue[rightBackEncode] <= 630){
			motor[rightFront] = speed;
			motor[leftFront] = -speed;
			motor[rightBack] = speed;
			motor[leftBack] = -speed;
			flag = 0;
		}
		else{
			motor[rightFront] = 0;
			motor[leftFront] = 0;
			motor[rightBack] = 0;
			motor[leftBack] = 0;
			flag = 1;
		}
	}
}

void rightTurn(){
	while(flag != 1){
		if(SensorValue[leftBackEncode] <= 630 && SensorValue[rightBackEncode] >= -500){
			motor[rightFront] = -speed;
			motor[leftFront] = speed;
			motor[rightBack] = -speed;
			motor[leftBack] = speed;
			flag = 0;
		}
		else{
			motor[rightFront] = 0;
			motor[leftFront] = 0;
			motor[rightBack] = 0;
			motor[leftBack] = 0;
			flag = 1;
		}
	}
}

void resetEnodeValues(){
	SensorValue[leftBackEncode] = 0;
	SensorValue[rightBackEncode] = 0;
}