
#ifndef __COLOR_H__
#define __COLOR_H__ yay



#include <Arduino.h>



//////// Types

struct ColorRGB {
	byte r;
	byte g;
	byte b;
};

// Not sure what to do with this right now...
// struct ColorRGBW {
// 	byte r;
// 	byte g;
// 	byte b;
// 	byte w;
// };

//////// Functions

struct ColorRGB wheel( byte wheelPos );
struct ColorRGB colorHSV( byte hue, byte saturation, byte value );



#endif