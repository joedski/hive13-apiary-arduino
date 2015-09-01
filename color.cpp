
#include "color.h"



//////// Functions

// Sets a full saturation color from an input of 0 ~ 255,
// mapping through the colors R ~ Y ~ G ~ C ~ B ~ M ~ R
// Adapted from an Adafruit NeoPixel example function of the same name.
struct ColorRGB wheel( byte wheelPos ) {
	if( wheelPos < 85 ) {
		// 0 ~ 84 :: R ~ Y ~ G
		return {
			.r = 255 - wheelPos * 3,
			.g = wheelPos * 3,
			.b = 0
		};
	} else if( wheelPos < 170 ) {
		// 85 ~ 169 :: G ~ C ~ B
		wheelPos -= 85;
		return {
			.r = 0,
			.g = 255 - wheelPos * 3,
			.b = wheelPos * 3
		};
	} else {
		// 170 ~ 255 :: B ~ M ~ R
		wheelPos -= 170;
		return {
			.r = wheelPos * 3,
			.g = 0,
			.b = 255 - wheelPos * 3
		};
	}
}

// struct ColorRGBW wheelRGBW( byte wheelPos ) {
// 	struct ColorRGB rgbColor = wheel( wheelPos );

// 	return {
// 		.r = rgbColor.r,
// 		.g = rgbColor.g,
// 		.b = rgbColor.b,
// 		.w = 0
// 	};
// }

static byte hsvMix( byte component, byte saturation, byte value ) {
	uint16_t componentInverse = 255 - component;
	uint16_t saturatedComponent = component + ((componentInverse * (uint16_t)saturation) >> 8);
	return (byte)((saturatedComponent * (uint16_t)value) >> 8);
}

// at t=0, returns a.  t=1 returns b.  t=0.5 returns a mix of the two.
static byte mix( byte a, byte b, byte t ) {
	return ((a * t) >> 8) + ((b * (255 - t)) >> 8);
}

// Hue is the wheelPosition in wheel.
// Saturation alters all components, reducing their difference as saturation becomes closer to 0.  At 255, the initial values are left as is.
// Value alters all components multiplicatively.
struct ColorRGB colorHSV( byte hue, byte saturation, byte value ) {
	struct ColorRGB colorHue = wheel( hue );

	return {
		.r = hsvMix( colorHue.r, saturation, value ),
		.g = hsvMix( colorHue.g, saturation, value ),
		.b = hsvMix( colorHue.b, saturation, value )
	};
}
