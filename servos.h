
#ifndef __SERVOS_H__
#define __SERVOS_H__ yay



// Servo.h?
#include <PWMServo.h>
#include <math.h>

#include "global_definitions.h"
#include "animation.h"



// 1x and 5.5x
#define SERVO_ANIMATION_RATE100_LOW 100
#define SERVO_ANIMATION_RATE100_HIGH 550

SETUP_PROC( servos );
RUNLOOP_PROC( servos );



#endif
