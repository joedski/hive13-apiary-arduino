
#include "pixels.h"



//////// Static Initialization

#define NEOPIXEL_PIXEL_COUNT 8
Adafruit_NeoPixel strip = Adafruit_NeoPixel( NEOPIXEL_PIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800 );

// NOTE: Vars are being stored this way so that names don't collide.

//// Color Wheel Animation

// How much to shift the color of each pixel in the animation as they're iterated over.
#define COLOR_WHEEL_SHIFT_AMOUNT 16

struct ColorWheelAnimationModel {
	struct AnimationTimingModel timing;
	byte colorPosition;
	byte saturation;
};

struct ColorWheelAnimationModel colorWheelAnimation = {
	.timing = {
		.progress = 0
		, .normalDuration = 4000
		, .rate100 = 100
	}
	, .colorPosition = 0
	, .saturation = 0
};

//// Chaser Animation

struct ChaserAnimationModel {
	struct AnimationTimingModel timing;
	byte pixelOffset;
};

struct ChaserAnimationModel chaserAnimation = {
	.timing = {
		.progress = 0
		, .normalDuration = 1000
		, .rate100 = 100
	}
	, .pixelOffset = 0
};



//////// Runtime

// Static because I use the same names in servos.cpp.
static void updateAnimationRate( struct ApiaryState state );
static void incrementAnimations( struct ApiaryState state );
static void calculateAnimations( struct ApiaryState state );
static void calculateColorWheel( struct ApiaryState state );
static void calculateChaser();
static void writeAnimations();

SETUP_PROC( pixels ) {
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
}

RUNLOOP_PROC( pixels ) {
	// General thoughts on organizing the animation code:
	// I broke it up into the following general sections:
	// 
	// - Updating the animation rate based on the global state.
	// - Updating the animation's progress based on the current animation rate and the change in time from the last tick.
	// - Calculating the animation's current variables from both the animation's progress and from the global state.
	// - Actually calculating and writing out each bit of the current animation.
	// 
	// I've tended to defer actually "expanding" the animation's variables into a full "frame"
	// until the point of writing just to save a bit on memory, and also because that's how it was before.
	// Whether or not memory is actually a concern here or not is another question entirely.

	updateAnimationRate( state );
	incrementAnimations( state );
	calculateAnimations( state );
	writeAnimations();
}



//////// Implementation

//// Update Animation Rate

static void updateAnimationRate( struct ApiaryState state ) {
	int rate100 = map(
		state.lightSensorReading,
		LIGHT_MIN, LIGHT_MAX,
		PIXEL_ANIMATION_RATE100_HIGH, PIXEL_ANIMATION_RATE100_LOW
		);

	rate100 = constrain( rate100, PIXEL_ANIMATION_RATE100_LOW, PIXEL_ANIMATION_RATE100_HIGH );

	colorWheelAnimation.timing.rate100 = rate100;
	chaserAnimation.timing.rate100 = rate100;
}

//// Increment Animation Progress

static void incrementAnimations( struct ApiaryState state ) {
	colorWheelAnimation.timing = animation_incrementProgress( colorWheelAnimation.timing, state.timeDelta );
	chaserAnimation.timing = animation_incrementProgress( chaserAnimation.timing, state.timeDelta );
}

//// Calculate Animations

static void calculateAnimations( struct ApiaryState state ) {
	calculateColorWheel( state );
	calculateChaser();
}

static void calculateColorWheel( struct ApiaryState state ) {
	colorWheelAnimation.colorPosition = map( colorWheelAnimation.timing.progress, 0, ANIMATION_PRORGESS_MAX, 0, 255 );
	colorWheelAnimation.saturation = constrain(
		map(
			state.lightSensorReading,
			LIGHT_MIN, LIGHT_MAX,
			255, 0
			),
		0, 255
		);
}

static void calculateChaser() {
	// chaserAnimation.pixelOffset = chaserAnimation.timing.progress * CHASER_PIXEL_PERIOD / ANIMATION_PRORGESS_MAX;
	chaserAnimation.pixelOffset = map(
		chaserAnimation.timing.progress,
		0, ANIMATION_PRORGESS_MAX,
		0, CHASER_PIXEL_PERIOD
		);

#if defined(ENABLE_SERIAL_DEBUG) && defined(ENABLE_SERIAL_DEBUG_PIXELS)
	Serial.println( "Chaser:" );
	Serial.print( "    timing.normalDuration = " );
	Serial.println( chaserAnimation.timing.normalDuration );
	Serial.print( "    timing.progress = " );
	Serial.println( chaserAnimation.timing.progress );
	Serial.print( "    timing.rate100 ="  );
	Serial.println( chaserAnimation.timing.rate100 );
	Serial.print( "    pixelOffset = " );
	Serial.println( chaserAnimation.pixelOffset );
#endif
}

//// Write Animations

static void writeAnimations() {
	// First, this assumes that all the pixels start at 0x000000, which is off.
	// It then works by...
	// - setting the currently on pixels to the desired color,
	// - writing those pixel values to the hardware lights,
	// - and finally blanking out those pixels to reset all values to 0.
	//     Note that the blanking out is NOT written to the hardware lights,
	//     so the viewer never sees them go out except when they're not extinguished again.
	//     Technically we can just overwrite everything back to 0 before showing it,
	//     and in some cases that may save some cpu clock time.

	int numPixels = strip.numPixels();
	byte colorHue;
	struct ColorRGB color;

	for( int i = chaserAnimation.pixelOffset; i < numPixels; i = i + CHASER_PIXEL_PERIOD ) {
		// Just a bit of fun.
		colorHue = (colorWheelAnimation.colorPosition + i) * COLOR_WHEEL_SHIFT_AMOUNT % 255;
		color = colorHSV( colorHue, colorWheelAnimation.saturation, 255 );
		strip.setPixelColor( i, strip.Color( color.r, color.g, color.b ) );
	}

	strip.show();

	// NOTE: In the original code, there was a 10 ms wait here, after the strip.show().  Is that really necessary?  I guess I'll find out.

	for( int i = chaserAnimation.pixelOffset; i < numPixels; i = i + CHASER_PIXEL_PERIOD ) {
		strip.setPixelColor( i, 0 );
	}
}



//////// Utilities

// ...
