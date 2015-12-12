#pragma config(Motor,  port1,           left,          tmotorVex393, openLoop)
#pragma config(Motor,  port2,           lift,          tmotorVex393, openLoop)
#pragma config(Motor,  port3,           hand,          tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port4,           hand2,         tmotorServoContinuousRotation, openLoop)
#pragma config(Motor,  port10,          right,         tmotorVex393, openLoop)
//*!!Code automatically generated by 'ROBOTC' configuration wizard               !!*//

void drive();
void arm();

task main()
{
	while(true){
		drive();
		arm();
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

    motor[left] = lSpeed;
    motor[right] = rSpeed;
}

void arm(){
	if(vexRT[Btn5U] == 1) {
		motor[lift] = 127;
		}
	else if(vexRT[Btn5D] == 1){
		motor[lift] = -127;
	}
	else {
		motor[lift] = 0;
	}
	if(vexRT[Btn7U] == 1) {
		motor[hand] = 127;
	}
	else if(vexRT[Btn7D] == 1) {
		motor[hand] = -127;
 	}
 	else{
 		motor[hand] = 0;
 	}
 	if(vexRT[Btn8U] == 1){
 		motor[hand2] = 127;
 	}
 	else if(vexRT[Btn8D]){
 		motor[hand2] = -127;
 	}
 	else{
 		motor[hand2] = 0;
 	}
}
