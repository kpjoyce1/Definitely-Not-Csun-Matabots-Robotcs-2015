#include "PID.h"

const float B0 = PROPORTIONAL_GAIN*(1+SAMPLE_PERIOD/(2*INTEGRAL_CONST));
const float B1 = PROPORTIONAL_GAIN*(SAMPLE_PERIOD/(2*INTEGRAL_CONST)-1);

typedef struct {
  tMotor name1, name2; //this have to be rewied, whatever to be that keywork that identify the motor
  tSensors sensorname;
  int speedSet; //speed desired to the motor
  float lastError; //last error calculated for that motor
  float controller_output; //controller output to the motor
  float speedRead; //speed calculated from the encoder values
  int lastEncoderRead; //last encoder read. Necessary to calulate the speed
} MOTOR_PI;

void ReadSpeed(MOTOR_PI* motorA, MOTOR_PI* motorB, int time){
  int encoder;
  encoder = SensorValue[(*motorA).sensorname];
  (*motorA).speedRead = abs(5.88235*(float)(encoder - (*motorA).lastEncoderRead)/((float)time) ); //that is not a unit. You should multiply for a constant. I will leave this way by now.
  (*motorA).lastEncoderRead = encoder;
  encoder = SensorValue[(*motorB).sensorname];
  (*motorB).speedRead = abs(5.88235*(float)(encoder - (*motorB).lastEncoderRead)/((float)time) );
  (*motorB).lastEncoderRead = encoder;
}

void ControlFunction(MOTOR_PI* motorA, MOTOR_PI* motorB){
	PI_Control(motorA);
  PI_Control(motorB);
}

void PI_Control(MOTOR_PI* motorA){
  int erroPast1;
	int output;
  //make the calculation for the controller inputs
  erroPast1 = (*motorA).lastError;
  (*motorA).lastError = (*motorA).speedSet-(*motorA).speedRead;

  //apply control`s law
  (*motorA).controller_output = (*motorA).controller_output+B0*((*motorA).lastError)+B1*erroPast1;

  //apply saturation of the controller
  if ( (*motorA).controller_output > SATURATION_UP ) output = SATURATION_UP;
  else if ( (*motorA).controller_output < SATURATION_DOWN ) output = SATURATION_DOWN;
  else if ( (*motorA).controller_output < MOTOR_DEADZONE && (*motorA).controller_output > -MOTOR_DEADZONE ) output = 0;
	else output = (*motorA).controller_output;

  //set power in the motor
  motor[(*motorA).name1] = output;
  motor[(*motorA).name2] = output;

  //the controler output can`t be less the saturation dowm
  if ( (*motorA).controller_output < SATURATION_DOWN ) (*motorA).controller_output = SATURATION_DOWN;
}
