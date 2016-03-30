#include <Servo.h>
#include <Arduino.h>
#include "State.h"

//----------------------------- GLOBAL VARIABLES -----------------------------------
//----------------------------------------------------------------------------------

State stateGlobal;


// Maybe the threshold should change to
// maintain a reasonable rate of events?
const int   LIGHT_CHANGE_THRESHOLD = 15;
const int   BEHAVIOR_MAX_AMOUNT = 6;

const int   BEHAVIOR_DIRECT     = 0;
const int   BEHAVIOR_TURN       = 1;
const int   BEHAVIOR_EV_RND     = 2;
const int   BEHAVIOR_WALK       = 3;
const int   BEHAVIOR_YOGA       = 4;
const int   BEHAVIOR_DEAD       = 5;

int         lightChangeDelta  = 0;
boolean     isGoingUp         = true;
long        timeToUpdAvg      = 0;
boolean     doUpdateLightAvg  = false;

const int   LEGS = 3;
// TODO: put these in a State[LEGS] states, take time out of State?
Servo       servoCtrl[LEGS];
int         servoPos[LEGS]       = { 20, 20, 20 };
int         avgLightValue[LEGS]  = { 0, 0, 0 };
int         inPin[LEGS]          = { 0, 1, 7 };
int         outPin[LEGS]         = { 2, 10, 12 };
int         lastLightValue[LEGS] = { 80, 80, 80 };
int         eventCountCurr[LEGS] = { 0, 0, 0 };
int         eventCountLast[LEGS] = { 0, 0, 0 };
boolean     wasGoingUp[LEGS]     = { true, true, true };

const float BASE_SPEED = 0.04;
float speed = BASE_SPEED;

int   behaviorCurrent    = BEHAVIOR_DEAD;
int   behaviorNext       = BEHAVIOR_DEAD;
float behaviorTime       = 0.0; // this increases from 0 to 1
float interpolationSpeed = 0.0; // increment for behaviorTime. Can be positive or 0.

//----------------------------- Function Prototypes ---------------------------------
//----------------------------------------------------------------------------------

//using       func_ptr_t = int ( * )( State, int );
//func_ptr_t  behaviorPointerArray[ BEHAVIOR_MAX_AMOUNT ];
int ( *behaviorPointerArray[ BEHAVIOR_MAX_AMOUNT ] )( State, int, int );

//----------------------------- SETUP ----------------------------------------------
//----------------------------------------------------------------------------------

void setup() {
  // Configure servo pins
  for (int i = 0; i < LEGS; i++) {
    servoCtrl[i].attach(outPin[i]);
  }
  Serial.begin(9600);

  // Function pointer assignment not possible to be global (so done here)
  // Assume the declaration has to be assigned first via compiler
  behaviorPointerArray[ BEHAVIOR_DIRECT ]    =   behaviorDirect;
  behaviorPointerArray[ BEHAVIOR_TURN ]      =   behaviorTurn;
  behaviorPointerArray[ BEHAVIOR_EV_RND ]    =   behaviorRandom;
  behaviorPointerArray[ BEHAVIOR_WALK ]      =   behaviorWalk;
  behaviorPointerArray[ BEHAVIOR_YOGA ]      =   behaviorYoga;
  behaviorPointerArray[ BEHAVIOR_DEAD ]      =   behaviorDead;
}

//----------------------------- LOOP -----------------------------------------------
//----------------------------------------------------------------------------------

void loop() {

  // Update average once per second
  doUpdateLightAvg = (millis() / 1000) > timeToUpdAvg;

  stateGlobal.time += speed;

  // speed decays with time
  if (speed > BASE_SPEED) {
    speed *= 0.98;
  }

  for (int legCurrent = 0; legCurrent < LEGS; legCurrent++) {
    stateGlobal.currLightValue = analogRead(inPin[ legCurrent ]);

    if (doUpdateLightAvg) {
      // lerp 50% towards read value
      avgLightValue[ legCurrent ] += stateGlobal.currLightValue;
      avgLightValue[ legCurrent ] /= 2;
      // reset event counter once per second
      eventCountLast[ legCurrent ] = eventCountCurr[ legCurrent ];
      eventCountCurr[ legCurrent ] = 0;
    }

    stateGlobal.currLightValue -= avgLightValue[ legCurrent ];
    stateGlobal.currLightValue = map(stateGlobal.currLightValue, -800, 800, 0, 179);

    // detect events
    stateGlobal.triggerEvent = false;
    isGoingUp = stateGlobal.currLightValue > lastLightValue[ legCurrent ];
    if (isGoingUp == wasGoingUp[ legCurrent ]) {
      lightChangeDelta += abs(stateGlobal.currLightValue - lastLightValue[ legCurrent ]);
    } else {
      // when the direction changes check
      // if we traveled far
      if (lightChangeDelta > LIGHT_CHANGE_THRESHOLD) {
        // TODO: this may be a bit confusing, because it's not global: triggerEvent is only
        // temporary, and referring to one of the sensors, not all three.
        stateGlobal.triggerEvent = true;
      }
      lightChangeDelta = 0;
    }
    if (stateGlobal.triggerEvent) {
      eventCountCurr[ legCurrent ]++;
      // speed increases with events
      speed *= 1.01;
    }

    int posCurr = behaviorPointerArray[ behaviorCurrent ]( stateGlobal, legCurrent, servoPos[ legCurrent ] );
    int posNext = behaviorPointerArray[ behaviorNext ]( stateGlobal, legCurrent, servoPos[ legCurrent ] );
    servoPos[ legCurrent ] = posCurr * (1 - behaviorTime) + posNext * behaviorTime;

    servoCtrl[ legCurrent ].write(servoPos[ legCurrent ]);

    lastLightValue[ legCurrent ] = stateGlobal.currLightValue;
    wasGoingUp[ legCurrent ] = isGoingUp;

    delay(15);
  }

  // if we are interpolating behaviors
  if (interpolationSpeed > 0) {
    // if we are having events, increase interpolation speed
    if (eventCountCurr[0] + eventCountCurr[1] + eventCountCurr[2] > 2) {
      interpolationSpeed *= 1.5;
    }

    //    Serial.print(".");

    // increase time
    behaviorTime += interpolationSpeed;
    // if we complete the interpolation
    if (behaviorTime > 1) {
      // stop interpolating
      behaviorTime = 0;
      interpolationSpeed = 0;
      // and make the current behavior equal to the next behavior
      behaviorCurrent = behaviorNext;
    }
  } else {
    switch (eventCountLast[0]) {
      case 0:
        // if no events, stay doing the same or go dead with 30% probability
        if (chance(30)) {
          interpolationSpeed = 0.003 + randomf(0.02);
          behaviorNext = BEHAVIOR_DEAD;
        }
        break;
      case 1:
        interpolationSpeed = 0.003 + randomf(0.02);
        behaviorNext = BEHAVIOR_YOGA;
        break;
      case 2:
        interpolationSpeed = 0.003 + randomf(0.02);
        behaviorNext = chance(30) ? BEHAVIOR_WALK : BEHAVIOR_TURN;
        speed = BASE_SPEED * 2;
        break;
      default:
        interpolationSpeed = 0.003 + randomf(0.02);
        behaviorNext = chance(60) ? BEHAVIOR_DIRECT : BEHAVIOR_EV_RND;
    }
    //    Serial.println();
    //    Serial.print("events: ");
    //    Serial.print(eventCountLast[0]);
    //    Serial.print(" behavior: ");
    //    Serial.println(behaviorNext);
  }

  // schedule the next update,
  if (doUpdateLightAvg) {
    timeToUpdAvg++;
  }

  //debugInt3(servoPos);

}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------

