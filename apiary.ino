#include <Adafruit_NeoPixel.h>
#include <Servo.h>

#include "macros.h"
#include "global_definitions.h"

#include "pixels.h"



// Static Initializations

struct ApiaryState apiaryState = {
	// .otherThing = 42, .thingamabob = { .foo = 1, .bar = 2 }, ...
	// This style of struct initialization is called the "designated initializer style".

	// Analog reading of light sensor.  Varies from 0 to 255.
	.lightSensorReading = 0

	// Time and time since last loop in MS.
	, .time = 0
	, .timeDelta = 0
};

// Sensor

RUNLOOP_PROC( sensors ) {
	static unsigned long lastTime = 0;

	state.lightSensorReading = analogRead( LIGHT_SENSOR_PIN );

	// TODO: Overflow detection?  (That assumes the box is left running for more than 50 days...)
	lastTime = state.time;
	state.time = millis();
	state.timeDelta = state.time - state.lastTime;
}

// Main

SETUP_PROC_ARRAY( setupArray ) = {
	SETUP_PROC_NAME( pixels ),
	SETUP_PROC_NAME( servos ),
	NULL
};

RUNLOOP_PROC_ARRAY( runloop ) = {
	RUNLOOP_PROC_NAME( sensors ),
	RUNLOOP_PROC_NAME( pixels ),
	RUNLOOP_PROC_NAME( servos ),
	NULL
};



// Setup

setup() {
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



// Loop

loop() {
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

	delay( RUNLOOP_DELAY_MS );
}