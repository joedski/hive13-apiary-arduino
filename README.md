Arduino Code for the HIVE13 Apiary Box
======================================

Arduino code for the Hive13 Apiary potpourri box.

This is a ground-up rewrite of the 2014 code because I wanted an easier animation framework.  This time, it shall be put on Github for posterity and so people can publicly mock the code.

Most of it at its base is actually just a refactor of the old code, broken apart into more generalistic things, and grouped logically rather than as a giant mess of spaghetti.  Most importantly, the servo and neopixel animations were split across separate files.

This is also an "I learned C preprocessor macros yaaay" thing so there's too many of those in `global_definitions.h`.



Configurable Things
-------------------

There's a bunch of things that have been made into logically named constants placed in headers.  Outside of things of global scope, such as pin assignments, most settings are stored in their specific section's header files.

- `global_definitions.h` contains the settings that must be respected across the entire sketch.
	- Pin assignments.  What pins go to the servos, pixels, sensor, etc.  These are put here so that all pin assignments can be seen in one place.
	- Light sensor range.  These were determined empirically, so probably shouldn't be changed.  Though, maybe they should be, depending on the environment.  I don't know.
- `servos.h` contains the settings specific to the servos.
	- The various Servo Positions. (Angles, in degrees roughly.)
	- The servo animations' rate range.
- `pixels.h` contains the settings specific to the pixel thingy.
	- The period (or pitch?) of the chaser animation, which is a fancy way of saying how many pixels are lit and how many aren't.
	- The pixel animations' rate range.  Controls both the rainbow color-wheeling and the chasing.



Libraries
---------

This uses the following libraries:

- [PWMServo][pwmservo] This restricts servos to pins 9 and 10, so should probably be replaced at some point, and is old anyway.
- [Adafruit NeoPixel][neopixel]

[pwmservo]: http://arduiniana.org/libraries/pwmservo/
[neopixel]: https://github.com/adafruit/Adafruit_NeoPixel
