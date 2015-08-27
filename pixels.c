#include "pixels.h"

// Static Initialization

#define NEOPIXEL_PIXEL_COUNT 8
// #define NEOPIXEL_PIN 6 // This is in global_definitions.h

Adafruit_NeoPixel strip = Adafruit_NeoPixel( NEOPIXEL_PIXEL_COUNT, NEOPIXEL_PIN, NEO_GRB + NEO_KHZ800 );



// Runtime

SETUP_PROC( pixels ) {
	strip.begin();
	strip.show(); // Initialize all pixels to 'off'
}

RUNLOOP_PROC( pixels ) {
	// ...
}
