
#include "servos.h"



/***

***/



// Static Initialization

#define SERVO_POSITION_MAX 100
#define SERVO_POSITION_MIN 0

// The range of ServoState.mappedLightValue
#define SERVO_SIGNAL_MIN 0
#define SERVO_SIGNAL_MAX 1000
// Determined empirically, this seems to be a useful value for the upper value of the map.
#define SERVO_SIGNAL_MAP_MAX 600

PWMServo hservo;
PWMServo iservo;

struct ServoState {
	int mappedLightValue;
	// ...
};

struct ServoState servoState = {
	.mappedLightValue = SERVO_POSITION_MIN
	// , .thing = foo
};



// Runtime

int mapLightSensorToServoValue( int lightSensorReading );

SETUP_PROC( servos ) {
	// Attach pins 9 and 10 to servos.
	hservo.attach( SERVO_H_PIN );
	iservo.attach( SERVO_I_PIN );
}

RUNLOOP_PROC( servos ) {
	// state, servoState...
	servoState.mappedLightValue = mapLightSensorToServoValue( state.lightSensorReading );
}

int mapLightSensorToServoValue( int lightSensorReading ) {
	// Originally, we used:
	//     servoLight = map( lightReading, MIN_LIGHT, MAX_LIGHT, 6, 1 );
	//     return constrain( servoLight, 1, 10 );
	int mapped = map( lightSensorReading, MIN_LIGHT, MAX_LIGHT, SERVO_SIGNAL_MAP_MAX, 0 );
	return constrain( mapped, SERVO_SIGNAL_MIN, SERVO_SIGNAL_MAX );
}
