#include "servos.h"



// Static Initialization

PWMServo hservo;
PWMServo iservo;



// Runtime

SETUP_PROC( servos ) {
	// Attach pins 9 and 10 to servos.
	hservo.attach( SERVO_H_PIN );
	iservo.attach( SERVO_I_PIN );
}

RUNLOOP_PROC( servos ) {
	// ...
}
