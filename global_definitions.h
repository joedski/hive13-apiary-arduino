// Global definitions.

#ifndef __GLOBAL_DEFINITIONS_H__
#define __GLOBAL_DEFINITIONS_H__ yay



//////// Servo.

// Servos that swivel the sign.
// The H pin (has H, I, and 1) and the I pin (has V, E, and 3)
#define SERVO_H_PIN 9
#define SERVO_I_PIN 10

#define NEOPIXEL_PIN 6



//////// Light Sensor.

// What pin the light sensor runs to.
#define LIGHT_SENSOR_PIN 0

// The range of readings considered useful out of the full 0 ~ 1023 range.
// Although 500 (covered) ~ 900 (uncovered) was the actual range determined,
// adding a margin of 50 guarantees the edge values being used.
#define MIN_LIGHT 550
#define MAX_LIGHT 850



//////// Runloop Stuff.

/*********
Write the top-level runloop procedure with this.

RUNLOOP_PROC( lights ) {
	updateLights();
	boopLights();
}

RUNLOOP_PROC_ARRAY( runloop ) = {
	RUNLOOP_PROC_PTR( lights ),
	RUNLOOP_PROC_PTR( sensor ),
	RUNLOOP_PROC_PTR( action ),
	NULL
};

loop() {
	RUNLOOP_PROC_PRT( proc ) = NULL;
	int i = 0;

	while( 1 ) {
		proc = runloop[ i ];

		if( proc == NULL ) {
			break;
		}

		proc();

		++i;
	}
}

It is similar to do the setup procedures.
**********/

#define RUNLOOP_DELAY_MS 30

// Base macros so we can make sure all pointers play nice with eachother.
#define RUNLOOP_PROC_NAME(procName) procName##_loop
#define RUNLOOP_PROC_ARGS ( struct ApiaryState state )
#define RUNLOOP_PROC_RETURN_TYPE void

// Use these.
#define RUNLOOP_PROC(procName) RUNLOOP_PROC_RETURN_TYPE RUNLOOP_PROC_NAME(procName) RUNLOOP_PROC_ARGS
#define RUNLOOP_PROC_PTR(procName) RUNLOOP_PROC_RETURN_TYPE (* RUNLOOP_PROC_NAME(procName) ) RUNLOOP_PROC_ARGS
#define RUNLOOP_PROC_ARRAY(arrayName) RUNLOOP_PROC_RETURN_TYPE (* arrayName []) RUNLOOP_PROC_ARGS

// Setup Macros.  Separate so that they can have different return and argument types from the runloop macros.
#define SETUP_PROC_NAME(procName) procName##_setup
#define SETUP_PROC_ARGS ()
#define SETUP_PROC_RETURN_TYPE void

// Use these.
#define SETUP_PROC(procName) SETUP_PROC_RETURN_TYPE SETUP_PROC_NAME(procName) SETUP_PROC_ARGS
#define SETUP_PROC_PTR(procName) SETUP_PROC_RETURN_TYPE (* SETUP_PROC_NAME(procName) ) SETUP_PROC_ARGS
#define SETUP_PROC_ARRAY(arrayName) SETUP_PROC_RETURN_TYPE (* arrayName []) SETUP_PROC_ARGS




//////// Global State.

struct ApiaryState {
	// Raw reading of the pin.  The useful range was determined empirically.
	int lightSensorReading;
	// The current time in milliseconds.  Overflows in about 50 days. (as per Arduino's docs.)
	unsigned long time;
	// The actual difference in time from the last tick.
	unsigned long timeDelta;
};



#endif
