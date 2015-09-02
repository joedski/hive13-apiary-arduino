
#include <Adafruit_NeoPixel.h>
// Servo.h?
#include <PWMServo.h>

#include "global_definitions.h"
#include "animation.h"
#include "pixels.h"
#include "servos.h"



//////// Static Initializations

struct ApiaryState apiaryState = {
	// This style of struct initialization is called the "designated initializer style".

	// Analog reading of light sensor.  Varies from 0 to 255.
	.lightSensorReading = 0

	// Time and time since last loop in MS.
	, .time = 0
	, .timeDelta = 0
};



//////// Debug LED

#define DEBUG_LED_PIN 13

SETUP_PROC( debug ) {
	pinMode( DEBUG_LED_PIN, OUTPUT );

#ifdef ENABLE_SERIAL_DEBUG
	// Serial debug.
	Serial.begin( 9600 );
#endif
}



//////// Sensor

RUNLOOP_PROC( sensors ) {
	apiaryState.lightSensorReading = analogRead( LIGHT_SENSOR_PIN );
}

RUNLOOP_PROC( time ) {
	unsigned long lastTime = 0;

	// Note: The first time this is called, timeDelta is the difference between millis() and 0.
	// TODO: Overflow detection?  (That assumes the box is left running for more than 50 days...)
	lastTime = apiaryState.time;
	apiaryState.time = millis();
	apiaryState.timeDelta = apiaryState.time - lastTime;
}



//////// Main Procedures

SETUP_PROC_ARRAY( setupArray ) = {
	SETUP_PROC_NAME( debug ),
	SETUP_PROC_NAME( pixels ),
	SETUP_PROC_NAME( servos ),
	NULL
};

RUNLOOP_PROC_ARRAY( runloop ) = {
	RUNLOOP_PROC_NAME( time ),
	RUNLOOP_PROC_NAME( sensors ),
	RUNLOOP_PROC_NAME( pixels ),
	RUNLOOP_PROC_NAME( servos ),
	NULL
};



//////// Setup

void setup() {
	SETUP_PROC_PTR( proc ) = NULL;
	int i = 0;

	while( 1 ) {
		proc = setupArray[ i ];

		if( proc == NULL ) {
			break;
		}

		proc();

		++i;
	}
}



//////// Loop

void loop() {
	RUNLOOP_PROC_PTR( proc ) = NULL;
	int i = 0;

	while( 1 ) {
		proc = runloop[ i ];

		if( proc == NULL ) {
			break;
		}

		proc( apiaryState );

		++i;
	}

	// Try to keep to RUNLOOP_DELAY_MS as our pulse.
	// these are longs rather than unsigned longs.
	long runloopDuration = millis() - apiaryState.time;
	long remainingDelay = RUNLOOP_DELAY_MS - runloopDuration;
	int animationTimeOverran = remainingDelay < 0;

	if( animationTimeOverran ) {
		digitalWrite( DEBUG_LED_PIN, HIGH );
	}
	else {
		digitalWrite( DEBUG_LED_PIN, LOW );
	}

#if defined(ENABLE_SERIAL_DEBUG) && defined(ENABLE_SERIAL_DEBUG_RUNLOOP)
	Serial.print( "time (ms): " );
	Serial.println( apiaryState.time );
	Serial.print( "time (ms, from millis()): " );
	Serial.println( millis() );
	Serial.print( "timeDelta (ms): " );
	Serial.println( apiaryState.timeDelta );
	Serial.print( "runloopDuration (ms): " );
	Serial.println( runloopDuration );
	Serial.print( "animationTimeOverran: " );
	Serial.println( animationTimeOverran ? "yes" : "no" );
#endif

	delay( animationTimeOverran ? 0 : remainingDelay );
}
