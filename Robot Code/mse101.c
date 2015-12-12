#pragma config(Motor,  port1,           RDrive1,       tmotorVex269, openLoop)
#pragma config(Motor,  port2,           RDrive2,       tmotorVex393, openLoop)
#pragma config(Motor,  port3,           LDrive1,       tmotorVex393, openLoop)
#pragma config(Motor,  port10,          LDrive2,       tmotorVex269, openLoop, reversed)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void drive();
void lift();
void claw();

task main()
{
	while(true){
			drive();
			//lift();
			//claw();
		}

}

void drive(){
	int lSpeed, rSpeed;
	int driveCh2 = vexRT[Ch2];
	int driveCh3 = vexRT[Ch3];

	//Right Motor Controls
  if (driveCh2 <= 10 && driveCh2 >= -10)
    rSpeed = 0;
  else if(driveCh2 > 10)
    rSpeed = (127/117) * driveCh2 - (1270/117);
  else if(driveCh2 < -10)
    rSpeed = (127/117) * driveCh2 + (1270/117);

  //Left Motor Controls
  if (driveCh3 <= 10 && driveCh3 >= -10)
    lSpeed = 0;
  else if(driveCh3 > 10)
    lSpeed = (127/117) * driveCh3 - (1270/117);
  else if(driveCh3 < -10)
    lSpeed = (127/117) * driveCh3 + (1270/117);

    motor[LDrive1] = lSpeed;
    motor[LDrive2] = lSpeed;
    motor[RDrive1] = rSpeed;
    motor[RDrive2] = rSpeed;
}
//lift Control
void lift(){
	if(vexRT[Btn5U] == 1) {
		motor[RDrive1] = 127;
		}
	else if(vexRT[Btn5D] == 1){
		motor[RDrive1] = -127;
	}
	else {
		motor[RDrive1] = 0;
	}
}
//Claw Control
void claw(){
	if (vexRT[Btn6U] == 1){
		motor[LDrive1] = 127;
	}
	else if(vexRT[Btn6D] == 1){
		motor[LDrive1] = -127;
	}
	else{
		motor[LDrive1] = 0;
	}
}
