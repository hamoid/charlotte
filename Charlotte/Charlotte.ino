#include <Servo.h>

int currInVal, currVal;
int len = 0;
// Maybe the threshold should change to maintain a reasonable rate of events?
const int lightChangeThreshold = 15;
boolean goingUp = true;
const int LEGS = 3;
Servo servo[LEGS];

int pos[LEGS] = { 20, 20, 20 };
int avg[LEGS] = { 0, 0, 0 };
int inPin[LEGS] = { 0, 1, 7 };
int outPin[LEGS] = { 2,10, 12 };
int lastInVal[LEGS] = { 80, 80, 80 };
boolean lastGoingUp[LEGS] = { true, true, true };

// To be used:
// we need envelopes to alter speed and range
// do we need current resting position, to tween towards that position?
float speed = 1.0; 
float range = 1.0;
int modeCurr = 1;
int modeNext = 1;
float modeInterp = 0.5;

// TODO
// Keep list of recent events
// 1. Walk - needs atan2
// 2. Direct action (original) algorithm
// 3. Calm slow moves
// 4. Broken / death sequence
// 5. Waiting yoga (bored)

// Delayed fading out reactions (echo)
// Time warp (slow down time)
// Predictable, so one can learn how it works
// Speed and motion range can vary


// DONE
// Detect events. To do that, notice direction changes.

long seconds=0;

void setup() {
  for(int i=0; i<LEGS; i++) {
    servo[i].attach(outPin[i]);
  }
  Serial.begin(9600);
}

void loop() {

  // update avg once per second
  if (millis() / 1000 > seconds){
    for(int i=0; i<LEGS; i++) {
      avg[i] += analogRead(inPin[i]);
      avg[i] /= 2;
    }
    seconds++;
    //speed = random(300) / 100000.0;
  }  

  for(int i=0; i<LEGS; i++) {
    currInVal = analogRead(inPin[i]) - avg[i];
    currInVal = map(currInVal, -800, 800, 0, 179);
    pos[i] += constrain(currInVal, 50, 140);
    pos[i] /= 2;    
    currInVal = pos[i];
    
    // yoga
    // currInVal = int(95 + 55 * (sin(millis() * speed)));
    
    // turn around. Sign of +i decides direction
    // the 0.005 decides the rotation speed
    // currInVal = int(95 + 55 * sin(millis() * 0.005 + i * 2.07));
    
    //servo[i].write(currVal);
    
    goingUp = currInVal > lastInVal[i];
    
    if(goingUp == lastGoingUp[i]) {
      len += abs(currInVal - lastInVal[i]);
    } else {
      // when the direction changes check
      // if we traveled far
      if(len > lightChangeThreshold) {
        // trigger event
        servo[i].write(random(50, 140));
      }
      len = 0;
    }
    
    lastInVal[i] = currInVal;
    lastGoingUp[i] = goingUp;
    
    delay(15);
  }

  /*
  Serial.print(pos[0]);
  Serial.print(", ");
  Serial.print(pos[1]);
  Serial.print(", ");
  Serial.print(pos[2]);
  Serial.println();
  */

}


float normalized_atan2(float y, float x) {
    static const uint32_t sign_mask = 0x80000000;
    static const float b = 0.596227f;

    // Extract the sign bits
    uint32_t ux_s  = sign_mask & (uint32_t &)x;
    uint32_t uy_s  = sign_mask & (uint32_t &)y;

    // Determine the quadrant offset
    float q = (float)( ( ~ux_s & uy_s ) >> 29 | ux_s >> 30 ); 

    // Calculate the arctangent in the first quadrant
    float bxy_a = ::fabs( b * x * y );
    float num = bxy_a + y * y;
    float atan_1q =  num / ( x * x + bxy_a + num );

    // Translate it to the proper quadrant
    uint32_t uatan_2q = (ux_s ^ uy_s) | (uint32_t &)atan_1q;
    return q + (float &)uatan_2q;
} 
