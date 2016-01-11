///////////FOR EVERYTHING
#define NULL2 0

///////////STATE DEFINITIONS
#define NUMBER_OF_STATES 13
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
#define NUMBER_OF_EVENTS 17
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
#define SEARCH_BALL
#define TURN_INTAKE_ON
#define TURN_INTAKE_OFF_COUNT_BALL_PLUS
#define FIND_ORIENTATION
#define FIND_MORE_BALLS
#define TURN_TO_GOAL
#define POSITIONING
#define CHARGE
#define TURN_INTAKE_OFF
#define TURN_INTAKE_OFF_COUNT_BALL_MINUS

int* setStateTransitionMachine(){
	int matrix[NUMBER_OF_STATES][NUMBER_OF_EVENTS];
	int i, j;
	for (i=0;i<NUMBER_OF_STATES;i++){
		for (j=0;j<NUMBER_OF_EVENTS;j++){
			matrix[i][j]=NULL2;
		}
	}

	matrix[SEARCH]					[BALL_DETECTED]			=	MOVE;
	matrix[SEARCH]					[NO_BALLS]					=	TURN;
	matrix[TURN]						[ANGULE_REACH]			=	SEARCH;
	matrix[MOVE]						[BALL_REACHED]			=	CATCH_BALL;
	matrix[MOVE]						[BALL_NOT_REACHED]	=	SEARCH;
	matrix[CATCH_BALL]			[INTAKE_SENSOR_TRIP]=	BALL_COUNT_CHECK;
	matrix[BALL_COUNT_CHECK][BALL_COUNT_FULL]		=	MOVE_TO_LINE;
	matrix[BALL_COUNT_CHECK][BALL_COUNT_EMPTY]	=	SEARCH;
	matrix[MOVE_TO_LINE]		[HIT_LINE]					=	LINE_FOLLOW;
	matrix[LINE_FOLLOW]			[HIT_BAR]						=	POSITION;
	matrix[POSITION]				[IN_POSITION]				=	SHOOTING;
	matrix[SHOOTING]				[BALL_COUNT_FULL]		=	LOADING;
	matrix[SHOOTING]				[BALL_COUNT_EMPTY]	=	SEARCH;
	matrix[LOADING]					[SHOOTER_LOADED]		=	CHARGING;
	matrix[CHARGING]				[SPEED_REACH]				=	FIRING;
	matrix[FIRING]					[SHOOTER_UNLOADED]	=	SHOOTING;

	return matrix;
}

int* setEventMatrix(){

	int matrix[NUMBER_OF_STATES][NUMBER_OF_EVENTS];
	int i, j;
	for (i=0;i<NUMBER_OF_STATES;i++){
		for (j=0;j<NUMBER_OF_EVENTS;j++){
			matrix[i][j]=NULL2;
		}
	}

	matrix[SEARCH]					[BALL_DETECTED]				=	GET_DIRECTION;
	matrix[SEARCH]					[NO_BALLS]						=	SET_TURN_ANGULE;
	matrix[TURN]						[ANGULE_REACH]				=	SEARCH_BALL;
	matrix[MOVE]						[BALL_REACHED]				=	TURN_INTAKE_ON;
	matrix[MOVE]						[BALL_NOT_REACHED]		=	SEARCH_BALL;
	matrix[CATCH_BALL]			[INTAKE_SENSOR_TRIP]	=	TURN_INTAKE_OFF_COUNT_BALL_PLUS;
	matrix[BALL_COUNT_CHECK][BALL_COUNT_FULL]			=	FIND_ORIENTATION;
	matrix[BALL_COUNT_CHECK][BALL_COUNT_EMPTY]		=	SEARCH_BALL;
	matrix[MOVE_TO_LINE]		[HIT_LINE]						=	TURN_TO_GOAL;
	matrix[LINE_FOLLOW]			[HIT_BAR]							=	POSITIONING;
	matrix[POSITION]				[IN_POSITION]					=	CHARGE;
	matrix[SHOOTING]				[BALL_COUNT_FULL]			=	TURN_INTAKE_ON;
	matrix[SHOOTING]				[BALL_COUNT_EMPTY]		=	SEARCH_BALL;
	matrix[LOADING]					[SHOOTER_LOADED]			=	TURN_INTAKE_OFF;
	matrix[CHARGING]				[SPEED_REACH]					=	TURN_INTAKE_ON;
	matrix[FIRING]					[SHOOTER_UNLOADED]		=	TURN_INTAKE_OFF_COUNT_BALL_MINUS;

	return matrix;
}




