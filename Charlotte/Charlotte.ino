#include <Servo.h>

int currInVal;
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
boolean triggerEvent = false;

// To be used:
// we need envelopes to alter speed and range
// do we need current resting position, to tween towards that position?
float speed = 0.0008; 
float range = 1.0; // unused so far
float time = 0.0;

// To do: generate both behaviours, and then cross fade them
int behCurr = 1; // unused so far
int behNext = 1;
float behTime = 0.5; // this will increase from 0 to 1

long timeToUpdAvg=0;

void setup() {
  // Configure servo pins
  for(int i=0; i<LEGS; i++) {
    servo[i].attach(outPin[i]);
  }
  Serial.begin(9600);
}
void loop() {

  // Update average once per second
  if (millis() / 1000 > timeToUpdAvg){
    for(int i=0; i<LEGS; i++) {
      // lerp 50% towards read value
      avg[i] += analogRead(inPin[i]);
      avg[i] /= 2;
    }
    // schedule the next update
    timeToUpdAvg++;
    
    // test speed randomization
    //speed = random(300) / 100000.0;
  }
  
  time = millis() * speed;

  for(int i=0; i<LEGS; i++) {
    currInVal = analogRead(inPin[i]) - avg[i];
    currInVal = map(currInVal, -800, 800, 0, 179);
    pos[i] += constrain(currInVal, 50, 140);
    pos[i] /= 2;    
    // smoothed input constrained to 50 ~ 140
    currInVal = pos[i];
    
    // 1. yoga
    currInVal = int(95 + 55 * (sin(time)));
    
    // 2. turn around. Sign of +i decides direction
    // the 0.005 decides the rotation speed
    // currInVal = int(95 + 55 * sin(time + i * 2.07));
        
    // 3. events
    triggerEvent = false;
    goingUp = currInVal > lastInVal[i];
    if(goingUp == lastGoingUp[i]) {
      len += abs(currInVal - lastInVal[i]);
    } else {
      // when the direction changes check
      // if we traveled far
      if(len > lightChangeThreshold) {
        triggerEvent = true;
      }
      len = 0;
    }
    if(triggerEvent) {
      currInVal = random(50, 140);
    }
    
    // 4. walk
    if(i == 0) {
      //currInVal = int(95 + 45 * impulse(7, time - int(time)));
    }
    if(i == 1) {
      //currInVal = int(95 - 45 * impulse(7, time - int(time)));
    }
    
    servo[i].write(currInVal);

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

