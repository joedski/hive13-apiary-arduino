
#include <Servo.h>

#include "global_definitions.h"
#include "animation.h"

#include "servos.h"



//////// Static Initialization

#define SERVO_POSITION_MAX 100
#define SERVO_POSITION_MIN 0

// The range of ServoState.mappedLightValue
#define SERVO_SIGNAL_MIN 0
#define SERVO_SIGNAL_MAX 1000
// Determined empirically, this seems to be a useful value for the upper value of the map.
#define SERVO_SIGNAL_MAP_MAX 600

PWMServo hservo;
PWMServo iservo;

struct ServoAnimationModel {
	struct AnimationTimingModel timing;
	int position;
};

struct ServoAnimationModel servoAnimation = {
	.timing = {
		.progress = 0
		, .normalDuration = 8000
		, .rate100 = 100
	}
	, .position = SERVO_POSITION_MIN
};



//////// Runtime

int mapLightSensorToServoValue( int lightSensorReading );

SETUP_PROC( servos ) {
	// Attach pins 9 and 10 to servos.
	hservo.attach( SERVO_H_PIN );
	iservo.attach( SERVO_I_PIN );
}

RUNLOOP_PROC( servos ) {
	// state, servoState...
	// servoState.mappedLightValue = mapLightSensorToServoValue( state.lightSensorReading );
	updateAnimationRate( state );
	incrementAnimations( state );
	calculateAnimations()
	writeAnimations()
}



//////// Implementation

//// Animation Rate

void updateAnimationRate( struct ApiaryState state ) {
	int rate100 = map(
		lightSensorReading,
		MIN_LIGHT, MAX_LIGHT,
		SERVO_ANIMATION_RATE100_LOW, SERVO_ANIMATION_RATE100_HIGH
		);

	// Originally, we had in the map a low value of 1 and a high value of 6,
	// but in the constrain, a low of 1 and a high of 10.
	// Also, in the map, the 6 was on the low-light value, and the 1 was on the high-light value,
	// so lower light sensed meant a higher number.
	// 
	// Going to try just going straight through here.  But, if it's too low,
	// we can multiply the range's high end by 1.66 to get the original range back.
	rate100 = constrain( rate100, SERVO_ANIMATION_RATE100_LOW, SERVO_ANIMATION_RATE100_HIGH );

	servoAnimation.timing.rate100 = rate100;
}

//// Increment Progress

void incrementAnimations( struct ApiaryState state ) {
	servoAnimation.timing = animation_incrementProgress( servoAnimation.timing, state );
}

//// Calculate Animations

void calculateAnimations() {
	float progressFloat = ((float) servoAnimation.timing.progress) / ((float) ANIMATION_PRORGESS_MAX);
	float angle = progressFloat * 2.0 * PI;
	float positionFloat = (cos( angle ) + 1.0 / 2.0) * ((float) SERVO_POSITION_MAX);

	servoAnimation.position = (int) lround( positionFloat );
}

//// Write Animations

void writeAnimations() {
	hservo.write( servoAnimation.position );
	iservo.write( servoAnimation.position );
}
