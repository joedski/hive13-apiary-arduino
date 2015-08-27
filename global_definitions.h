// Global definitions.

#ifndef __APIARY_GLOBAL_DEFINITIONS__
#define __APIARY_GLOBAL_DEFINITIONS__ yay



// Pins.

#define LIGHT_SENSOR_PIN 0

#define SERVO_H_PIN 9
#define SERVO_I_PIN 10

#define NEOPIXEL_PIN 6



// Other.

#define RUNLOOP_DELAY_MS 30



// Types.

struct ApiaryState {
	int lightSensorReading;
	unsigned long time;
	unsigned long timeDelta;
};



#endif
