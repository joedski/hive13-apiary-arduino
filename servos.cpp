
#include "servos.h"



//////// Static Initialization

PWMServo hservo;
PWMServo iservo;

struct ServoAnimationModel {
	struct AnimationTimingModel timing;
	int position;
};

struct ServoAnimationModel servoAnimation = {
	.timing = {
		.progress = 0
		, .normalDuration = 4000
		, .rate100 = 100
	}
	, .position = SERVO_POSITION_MIN
};



//////// Runtime

// Static because I use the same names in pixels.cpp.
// int mapLightSensorToServoValue( int lightSensorReading );
static void updateAnimationRate( struct ApiaryState state );
static void incrementAnimations( struct ApiaryState state );
static void calculateAnimations();
static void writeAnimations();

SETUP_PROC( servos ) {
	hservo.attach( SERVO_H_PIN );
	iservo.attach( SERVO_I_PIN );
}

RUNLOOP_PROC( servos ) {
	// state, servoState...
	// servoState.mappedLightValue = mapLightSensorToServoValue( state.lightSensorReading );
	updateAnimationRate( state );
	incrementAnimations( state );
	calculateAnimations();
	writeAnimations();
}



//////// Implementation

//// Animation Rate

static void updateAnimationRate( struct ApiaryState state ) {
	int rate100 = map(
		state.lightSensorReading,
		LIGHT_MIN, LIGHT_MAX,
		SERVO_ANIMATION_RATE100_HIGH, SERVO_ANIMATION_RATE100_LOW
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

static void incrementAnimations( struct ApiaryState state ) {
#if defined(ENABLE_SERIAL_DEBUG) && defined(ENABLE_SERIAL_DEBUG_SERVOS)
	unsigned long beforeProgress = servoAnimation.timing.progress;
#endif

	servoAnimation.timing = animation_incrementProgress( servoAnimation.timing, state.timeDelta );

#if defined(ENABLE_SERIAL_DEBUG) && defined(ENABLE_SERIAL_DEBUG_SERVOS)
	Serial.println( "Servos: incrementAnimations" );
	Serial.print( "    rate100: " );
	Serial.println( servoAnimation.timing.rate100 );
	// Serial.print( "    progress before: " );
	// Serial.println( beforeProgress );
	// Serial.print( "    progress after: " );
	// Serial.println( servoAnimation.timing.progress );
	Serial.print( "    progress increment: " );
	Serial.println( servoAnimation.timing.progress - beforeProgress );
#endif
}

//// Calculate Animations

static void calculateAnimations() {
	float progressFloat = float( servoAnimation.timing.progress ) / float( ANIMATION_PRORGESS_MAX );
	float angle = progressFloat * 2.0 * PI;
	float positionFloat = ((cos( angle ) + 1.0) / 2.0) * float( SERVO_POSITION_MAX ) + float( SERVO_POSITION_MIN );

	servoAnimation.position = (int) lround( positionFloat );
}

//// Write Animations

static void writeAnimations() {
	hservo.write( servoAnimation.position );
	iservo.write( servoAnimation.position );
}
