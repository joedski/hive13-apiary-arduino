
#include "pixels.h"
#include "animation.h"

// Static Initialization

#define NEOPIXEL_PIXEL_COUNT 8
// #define NEOPIXEL_PIN 6 // This is in global_definitions.h

Adafruit_NeoPixel strip = Adafruit_NeoPixel( NEOPIXEL_PIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800 );

struct AnimationProgressModel colorWheelProgress = {
	.progress = 0
	, .normalDuration = 4000
	, .rate100 = 100
};

struct AnimationProgressModel chaserProgress = {
	.progress 0
	, .normalDuration = 1200
	, .rate100 = 100
};



// Runtime

SETUP_PROC( pixels ) {
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
}

RUNLOOP_PROC( pixels ) {
	// ...
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
