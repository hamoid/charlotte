#include <Servo.h>


//----------------------------- GLOBAL VARIABLES -----------------------------------
//----------------------------------------------------------------------------------

// Maybe the threshold should change to
// maintain a reasonable rate of events?
const int LIGHT_CHANGE_THRESHOLD = 15;
const int BEH_DIRECT  = 0;
const int BEH_TURN    = 1;
const int BEH_EV_RND  = 2;
const int BEH_WALK    = 3;
const int BEH_YOGA    = 4;

int     currInVal, currVal;
int     lightChangeDelta  = 0;
boolean goingUp           = true;
long    timeToUpdAvg      = 0;
boolean doUpdAvg          = false;

const int   LEGS = 3;
Servo       servo[LEGS];
int         pos[LEGS]       = { 20, 20, 20 };
int         avg[LEGS]       = { 0, 0, 0 };
int         inPin[LEGS]     = { 0, 1, 7 };
int         outPin[LEGS]    = { 2, 10, 12 };
int         lastInVal[LEGS] = { 80, 80, 80 };
boolean     lastGoingUp[LEGS] = { true, true, true };
boolean     triggerEvent    = false;

// TODO: we need envelopes to alter speed and range
// do we need current resting position, to tween towards that position?
float speed = 0.0008;
float range = 1.0; // unused so far
float time  = 0.0;

// TODO: generate both behaviours, and then cross fade them
int   behCurr = 1; // unused so far
int   behNext = 1; // unused so far
float behTime = 0.5; // this will increase from 0 to 1

//----------------------------- Function Prototypes ---------------------------------
//----------------------------------------------------------------------------------
/*    signature: pos (*) pos, currVal, time, triggerEvent, leg(i)
*    typedef int (*func_ptr_t)( int );
*    func_ptr_t func_ptr_array[2];
*    int f1( int );
*    func_ptr_array[0] = f1;
*/

//----------------------------- SETUP ----------------------------------------------
//----------------------------------------------------------------------------------

void setup() {
  // Configure servo pins
  for (int i = 0; i < LEGS; i++) {
    servo[i].attach(outPin[i]);
  }
  Serial.begin(9600);
}

//----------------------------- LOOP -----------------------------------------------
//----------------------------------------------------------------------------------

void loop() {

  // Update average once per second
  doUpdAvg = (millis() / 1000) > timeToUpdAvg;

  time = millis() * speed;

  for (int i = 0; i < LEGS; i++) {
    currInVal = analogRead(inPin[i]);

    if (doUpdAvg) {
      // lerp 50% towards read value
      avg[i] += currInVal;
      avg[i] /= 2;
    }

    currVal = currInVal - avg[i];
    currVal = map(currVal, -800, 800, 0, 179);

    // detect events
    triggerEvent = false;
    goingUp = pos[i] > lastInVal[i];
    if (goingUp == lastGoingUp[i]) {
      lightChangeDelta += abs(pos[i] - lastInVal[i]);
    } else {
      // when the direction changes check
      // if we traveled far
      if (lightChangeDelta > LIGHT_CHANGE_THRESHOLD) {
        triggerEvent = true;
      }
      lightChangeDelta = 0;
    }

      /*TODO replace switch-case with array of function pointer
       * */

    switch (behCurr) {
      case BEH_DIRECT:
        pos[i] += constrain(currVal, 50, 140);
        pos[i] /= 2;
        break;

      case BEH_TURN:
        // Sign of +i decides rotation direction
        // time decides the rotation speed
        pos[i] = int(95 + 55 * sin(time + i * 2.07));
        break;

      case BEH_EV_RND:
        if (triggerEvent) {
          pos[i] = random(50, 140);
        }
        break;

      case BEH_WALK:
        if (i == 0) {
          pos[i] = int(95 + 45 * impulse(7, time - int(time)));
        }
        if (i == 1) {
          pos[i] = int(95 - 45 * impulse(7, time - int(time)));
        }
        break;

      case BEH_YOGA:
        pos[i] = int(95 + 55 * (sin(time)));
        break;
    }

    servo[i].write(pos[i]);

    lastInVal[i] = pos[i];
    lastGoingUp[i] = goingUp;

    delay(15);
  }

  // schedule the next update,
  if (doUpdAvg) {
    timeToUpdAvg++;
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

