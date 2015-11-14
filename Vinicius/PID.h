#ifndef PID_H
#define PID_H

	#define PROPORTIONAL_GAIN 1.25
	#define SAMPLE_PERIOD 200
	#define INTEGRAL_CONST 990.0
	#define SATURATION_UP 120
	#define SATURATION_DOWN 0
	#define MOTOR_DEADZONE 20

	extern void initMotor (MOTOR_PI* motorA, tMotor title1, tMotor title2, tSensors sensor);

	extern void ReadSpeed(MOTOR_PI* motorA, MOTOR_PI* motorB, int time);

	extern void ControlFunction(MOTOR_PI* motorA, MOTOR_PI* motorB);

#endif
