#include <Servo.h>

//----------------------------- GLOBAL VARIABLES -----------------------------------
//----------------------------------------------------------------------------------

struct State
{
    float time          = 0.0;
    int   currentValue  = 0;
    bool  triggerEvent  = false;
};


State stateGlobal;


// Maybe the threshold should change to
// maintain a reasonable rate of events?
const int   LIGHT_CHANGE_THRESHOLD = 15;
const int   BEHAVIOR_MAX_AMOUNT = 5;
const int   BEHAVIOR_DIRECT     = 0;
const int   BEHAVIOR_TURN       = 1;
const int   BEHAVIOR_EV_RND     = 2;
const int   BEHAVIOR_WALK       = 3;
const int   BEHAVIOR_YOGA       = 4;

int         currInVal;
int         lightChangeDelta  = 0;
boolean     goingUp           = true;
long        timeToUpdAvg      = 0;
boolean     doUpdAvg          = false;

const int   LEGS = 3;
Servo       servo[LEGS];
int         pos[LEGS]       = { 20, 20, 20 };
int         avg[LEGS]       = { 0, 0, 0 };
int         inPin[LEGS]     = { 0, 1, 7 };
int         outPin[LEGS]    = { 2, 10, 12 };
int         lastInVal[LEGS] = { 80, 80, 80 };
boolean     lastGoingUp[LEGS] = { true, true, true };

// TODO: we need envelopes to alter speed and range
// do we need current resting position, to tween towards that position?
float speed = 0.0008;
float range = 1.0; // unused so far


// TODO: generate both behaviours, and then cross fade them
int   behaviorCurrent   = 1; // unused so far
int   behaviorNext      = 1; // unused so far
float behaviorTime      = 0.5; // this will increase from 0 to 1

//----------------------------- Function Prototypes ---------------------------------
//----------------------------------------------------------------------------------

using       func_ptr_t = int ( * )( State, int );
func_ptr_t  behaviorPointerArray[ BEHAVIOR_MAX_AMOUNT ];

//----------------------------- SETUP ----------------------------------------------
//----------------------------------------------------------------------------------

void setup() {
  // Configure servo pins
  for (int i = 0; i < LEGS; i++) {
    servo[i].attach(outPin[i]);
  }
  Serial.begin(9600);

    // Function pointer assignment not possible to be global (so done here)
    // Assume the declaration has to be assigned first via compiler
    behaviorPointerArray[ BEHAVIOR_DIRECT ]    =   behaviorDirected;
    behaviorPointerArray[ BEHAVIOR_TURN ]      =   behaviorTurn;
    behaviorPointerArray[ BEHAVIOR_EV_RND ]    =   behaviorRandom;
    behaviorPointerArray[ BEHAVIOR_WALK ]      =   behaviorWalk;
    behaviorPointerArray[ BEHAVIOR_YOGA ]      =   behaviorYoga;

}

//----------------------------- LOOP -----------------------------------------------
//----------------------------------------------------------------------------------

void loop() {

  // Update average once per second
  doUpdAvg = (millis() / 1000) > timeToUpdAvg;

    stateGlobal.time = millis() * speed;

  for (int legCurrent = 0; legCurrent < LEGS; legCurrent++) {
    stateGlobal.currentValue = analogRead(inPin[ legCurrent ]);

    if (doUpdAvg) {
      // lerp 50% towards read value
      avg[ legCurrent ] += currInVal;
      avg[ legCurrent ] /= 2;
    }

      stateGlobal.currentValue -= avg[ legCurrent ];
      stateGlobal.currentValue = map(stateGlobal.currentValue, -800, 800, 0, 179);

    // detect events
    stateGlobal.triggerEvent = false;
    goingUp = pos[ legCurrent ] > lastInVal[ legCurrent ];
    if (goingUp == lastGoingUp[ legCurrent ]) {
      lightChangeDelta += abs(pos[ legCurrent ] - lastInVal[ legCurrent ]);
    } else {
      // when the direction changes check
      // if we traveled far
      if (lightChangeDelta > LIGHT_CHANGE_THRESHOLD) {
        stateGlobal.triggerEvent = true;
      }
      lightChangeDelta = 0;
    }
      
      // Perform movement behavior
      pos[ legCurrent ] = behaviorPointerArray[ behaviorCurrent ]( stateGlobal, legCurrent );

    servo[ legCurrent ].write(pos[ legCurrent ]);

    lastInVal[ legCurrent ] = pos[ legCurrent ];
    lastGoingUp[ legCurrent ] = goingUp;

    // Test. Change behavior every 4 seconds.
    // The changes should happen according
    // to user interaction (in the future)
    behaviorCurrent = (millis() / 4000) % 5;

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

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

