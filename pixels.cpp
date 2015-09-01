
#include <Adafruit_NeoPixel.h>

#include "global_definitions.h"
#include "animation.h"

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
	unsigned int colorPosition;
};

struct ColorWheelAnimationModel colorWheelAnimation = {
	.timing = {
		.progress = 0
		, .normalDuration = 4000
		, .rate100 = 100
	}
	, .colorPosition = 0
};

//// Chaser Animation

// What pixels are lit.  3 means every third pixel is lit, 4 means every fourth pixel is lit, etc.
#define CHASER_PIXEL_PERIOD 3

struct ChaserAnimationModel {
	struct AnimationTimingModel timing;
	char pixelOffset;
};

struct ChaserAnimationModel chaserAnimation = {
	.timing = {
		.progress = 0
		, .normalDuration = 4000
		, .rate100 = 100
	}
	, .pixelOffset = 0
};



//////// Runtime

void updateAnimationRate( struct ApiaryState state );
void incrementAnimations( struct ApiaryState state );
void calculateAnimations();
void calculateColorWheel();
void calculateChaser();
void writeAnimations();
uint32_t wheel( Adafruit_NeoPixel strip, byte wheelPos );

SETUP_PROC( pixels ) {
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
}

RUNLOOP_PROC( pixels ) {
	updateAnimationRate( state );
	incrementAnimations( state );
	calculateAnimations();
	writeAnimations();
}



//////// Implementation

//// Update Animation Rate

void updateAnimationRate( struct ApiaryState state ) {
	int rate100 = map(
		state.lightSensorReading,
		LIGHT_MIN, LIGHT_MAX,
		PIXEL_ANIMATION_RATE100_LOW, PIXEL_ANIMATION_RATE100_HIGH
		);

	rate100 = constrain( rate100, PIXEL_ANIMATION_RATE100_LOW, PIXEL_ANIMATION_RATE100_HIGH );

	colorWheelAnimation.timing.rate100 = rate100;
	chaserAnimation.timing.rate100 = rate100;
}

//// Increment Animation Progress

void incrementAnimations( struct ApiaryState state ) {
	colorWheelAnimation.timing = animation_incrementProgress( colorWheelAnimation.timing, state.timeDelta );
	chaserAnimation.timing = animation_incrementProgress( chaserAnimation.timing, state.timeDelta );
}

//// Calculate Animations

void calculateAnimations() {
	calculateColorWheel();
	calculateChaser();
}

void calculateColorWheel() {
	colorWheelAnimation.colorPosition = map( colorWheelAnimation.timing.progress, 0, ANIMATION_PRORGESS_MAX, 0, 255 );
}

void calculateChaser() {
	chaserAnimation.pixelOffset = chaserAnimation.timing.progress * CHASER_PIXEL_PERIOD / ANIMATION_PRORGESS_MAX;
}

//// Write Animations

void writeAnimations() {
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
	int color;

	for( int i = chaserAnimation.pixelOffset; i < numPixels; i = i + CHASER_PIXEL_PERIOD ) {
		// Just a bit of fun.
		color = wheel( strip, (colorWheelAnimation.colorPosition + i) * COLOR_WHEEL_SHIFT_AMOUNT % 255 );
		strip.setPixelColor( i, color );
	}

	strip.show();

	// NOTE: In the original code, there was a 10 ms wait here, after the strip.show().  Is that really necessary?  I guess I'll find out.

	for( int i = chaserAnimation.pixelOffset; i < numPixels; i = i + CHASER_PIXEL_PERIOD ) {
		strip.setPixelColor( i, 0 );
	}
}



//////// Utilities

// Sets a full saturation color from an input of 0 ~ 255,
// mapping through the colors R ~ Y ~ G ~ C ~ B ~ M ~ R
// (Not sure why strip.Color is a method, but hey.)
uint32_t wheel( Adafruit_NeoPixel strip, byte wheelPos ) {
	if( wheelPos < 85 ) {
		// 0 ~ 84 :: G ~ Y ~ R
		// return strip.Color( wheelPos * 3, 255 - wheelPos * 3, 0 );
		// 0 ~ 84 :: R ~ Y ~ G
		return strip.Color( 255 - wheelPos * 3, wheelPos * 3, 0 );
	} else if( wheelPos < 170 ) {
		// // 85 ~ 169 :: R ~ M ~ B
		// wheelPos -= 85;
		// return strip.Color( 255 - wheelPos * 3, 0, wheelPos * 3 );
		// 85 ~ 169 :: G ~ C ~ B
		wheelPos -= 85;
		return strip.Color( 0, 255 - wheelPos * 3, wheelPos * 3 );
	} else {
		// 170 ~ 255 :: B ~ C ~ G
		// wheelPos -= 170;
		// return strip.Color( 0, wheelPos * 3, 255 - wheelPos * 3 );
		// 170 ~ 255 :: B ~ M ~ R
		wheelPos -= 170;
		return strip.Color( wheelPos * 3, 0, 255 - wheelPos * 3 );
	}
}
