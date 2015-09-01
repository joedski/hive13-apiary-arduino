
#ifndef __PIXELS_H__
#define __PIXELS_H__ yay



#include <Adafruit_NeoPixel.h>

#include "global_definitions.h"
#include "animation.h"
#include "color.h"



//////// Pixel Animation Settings

// What pixels are lit.  3 means every third pixel is lit, 4 means every fourth pixel is lit, etc.
#define CHASER_PIXEL_PERIOD 3

// 1x and 5.5x
#define PIXEL_ANIMATION_RATE100_LOW 100
#define PIXEL_ANIMATION_RATE100_HIGH 550



///////// Prototypes

SETUP_PROC( pixels );
RUNLOOP_PROC( pixels );



#endif
