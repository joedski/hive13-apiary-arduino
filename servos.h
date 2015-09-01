
#ifndef __SERVOS_H__
#define __SERVOS_H__ yay



// Servo.h?
#include <Servo.h>
#include <math.h>

#include "global_definitions.h"
#include "animation.h"



//////// Animation Settings

// The various angle settings for both servos.
#define SERVO_POSITION_MIN 0
#define SERVO_POSITION_MAX 100

// This is the angle of the servo when the switch is set to Inactive.
#define SERVO_POSITION_RESTING 50

// 1x and 5.5x
#define SERVO_ANIMATION_RATE100_LOW 100
#define SERVO_ANIMATION_RATE100_HIGH 550



//////// Prototypes

SETUP_PROC( servos );
RUNLOOP_PROC( servos );



#endif
