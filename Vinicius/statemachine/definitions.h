///////////FOR EVERYTHING
#define NULL 0

///////////STATE DEFINITIONS

//STATE NULL IS AN ERROR STATE
#define TURN 							1
#define SEARCH 						2
#define MOVE 							3
#define CATCH_BALL 				4
#define BALL_COUNT_CHECK 	5
#define MOVE_TO_LINE 			6
#define LINE_FOLLOW 			7
#define POSITION 					8
#define SHOOTING 					9
#define LOADING 					10
#define CHARGING 					11
#define FIRING 						12

////////EVENTS DEFINITIONS

//NULL IS NO EVENT
#define NO_BALLS 						1
#define ANGULE_REACH 				2
#define BALL_DETECTED 			3
#define BALL_NOT_REACHED 		4
#define PID_DISTANCE 				5
#define BALL_REACHED 				6
#define INTAKE_SENSOR_TRIP	7
#define BALL_COUNT_FULL 		8
#define BALL_COUNT_EMPTY 		9
#define HIT_LINE 						10
#define HIT_BAR 						11
#define MOVE_TO_POSITION 		12
#define IN_POSITION 				13
#define SHOOTER_LOADED 			14
#define SPEED_REACH					15
#define SHOOTER_UNLOADED 		16

///////ACTIONS DEFINITIONS
//NULL IS NO ACTION
#define SET_TURN_ANGULE
#define GET_DIRECTION
#define SEARCH
#define TURN_INTAKE_ON
#define TURN_INTAKE_OFF_COUNT_BALL_PLUS
#define FIND_ORIENTATION
#define FIND_MORE_BALLS
#define TURN TO GOAL
#define POSITION
#define CHARGE
#define TURN_INTAKE_OFF
#define TUNR_INTAKE_OFF_COUNT_BALL_MINUS
