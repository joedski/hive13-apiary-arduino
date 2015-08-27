// Sweep
// by BARRAGAN <http://barraganstudio.com>
// This example code is in the public domain.
// Now includes code from AdaFruit for LEDs
// Now includes mods by Joseph Sikorski

#include <Adafruit_NeoPixel.h>
#include <Servo.h>

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, PIN, NEO_GRB + NEO_KHZ800);
 
Servo hservo;  // create servo object to control a servo
Servo iservo;                // a maximum of eight servo objects can be created
 
int pos = 0;    // variable to store the servo position
int val = 1;
int val2 = 1;
const int LIGHT=0;
const int MIN_LIGHT=450;
const int MAX_LIGHT=850;

void setup()
{
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  hservo.attach(9);
  iservo.attach(10);  // attaches the servo on pin 9 or 10 to the servo object
}
 
// It's bad practice to ever comment out all the things
// Instead, just comment out the calls to those other things, leaving the function's
// curly braces alone.
void loop()
{
  int lightReading;
  
  lightReading = getSensorReading();
  updateVars( lightReading );
  writeOutputs();
}

void updateVars( int lightReading )
{
  updateStripVars( lightReading );
  updateServoVars( lightReading );
}

void writeOutputs()
{
  writeStripOutputs();
  writeServoOutputs();
}

////////////////////////
// Implementation
////////////////////////

// Useful range: about 500 (covered) ~ 900 (uncovered)
// use 550 ~ 850 if you want to guarantee the edges.
int getSensorReading()
{
  return analogRead( LIGHT );
}

////// NeoPixel Strip //////

int stripLoop = 0;
int stripLoopIncrement = 4;
const int STRIP_LOOP_LIMIT = 256;
int stripLight = 0;
int stripWait = 10;

int stripSkip = 0;
int stripSkipIncrement = 10;
int stripSkipTarget = 100;

int stripOffset = 0;
int stripOffsetIncrement = 1;
const int STRIP_OFFSET_LIMIT = 3;

void updateStripVars( int lightReading )
{
  stripLight = map( lightReading, MIN_LIGHT, MAX_LIGHT, 10, 100 );
  stripWait = constrain( stripLight, 10, 100 );
  stripSkipTarget = constrain( stripLight, 10, 100 );
  
  if( stripSkip < stripSkipTarget ) {
    stripSkip += stripSkipIncrement;
    return;
  }
  
  stripSkip = 0;
  
  stripLoop += stripLoopIncrement;
  
  while( stripLoop >= STRIP_LOOP_LIMIT )
  {
    stripLoop -= STRIP_LOOP_LIMIT;
  }
  
  stripOffset += stripOffsetIncrement;
  
  if( stripOffset >= STRIP_OFFSET_LIMIT )
  {
    stripOffset = 0;
  }
}

void writeStripOutputs() {
  theaterChaseRainbow();
}

////// Servos //////

int servoLight = 0;
int servoMovementAmount = 0;
int servoMovementForward = true; // false means move backwards.
const int SERVO_POSITION_MAX = 100;
const int SERVO_POSITION_MIN = 0;
int servoPosition = SERVO_POSITION_MIN;

void updateServoVars( int lightReading )
{
  servoLight = map( lightReading, MIN_LIGHT, MAX_LIGHT, 6, 1 );
  servoMovementAmount = constrain( servoLight, 1, 10 );
  
  if( ! servoMovementForward )
  {
    servoMovementAmount = -servoMovementAmount;
  }
  
  servoPosition += servoMovementAmount;
  
  if( servoPosition < SERVO_POSITION_MIN )
  {
    servoPosition = SERVO_POSITION_MIN;
    servoMovementForward = true;
  }
  else if( servoPosition > SERVO_POSITION_MAX )
  {
    servoPosition = SERVO_POSITION_MAX;
    servoMovementForward = false;
  }
}

void writeServoOutputs()
{
  RotateServos();
}



//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow() {
  int wait, j, q;
  
  wait = 10;
  j = stripLoop;
  q = stripOffset;
  for (int i=0; i < strip.numPixels(); i=i+3) {
    strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
  }
  strip.show();
  delay(wait);
   
  for (int i=0; i < strip.numPixels(); i=i+3) {
    strip.setPixelColor(i+q, 0);        //turn every third pixel off
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
    return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
    WheelPos -= 170;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}

// This is a separate function (or subroutine) to make the servos rotate.
void RotateServos()
{
    hservo.write(servoPosition);              // tell servo to go to position in variable 'pos'
    delay(15);  
    iservo.write(servoPosition);              // tell servo to go to position in variable 'pos'
    delay(15);                      // waits 15ms for the servo to reach the position
}
