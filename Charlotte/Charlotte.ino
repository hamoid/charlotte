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
int   behaviorCurrent = 1; // unused so far
int   behNext = 1; // unused so far
float behTime = 0.5; // this will increase from 0 to 1

//----------------------------- Function Prototypes ---------------------------------
//----------------------------------------------------------------------------------
/* TODO Functional approach:
 * signature: pos (*) pos, currVal, time, triggerEvent, leg(i)
*/

//typedef void ( *func_ptr_t )( int ); // Function pointers who use global variables
//func_ptr_t func_ptr_array[ 5 ];
void ( *func_ptr_array[ 5 ] )( int );

void behaviorDirected( int leg );
void behaviorTurn( int leg );
void behaviorRandom( int leg );
void behaviorWalk( int leg );
void behaviorYoga( int leg );

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
    func_ptr_array[ BEH_DIRECT ]    =   behaviorDirected;
    func_ptr_array[ BEH_TURN ]      =   behaviorTurn;
    func_ptr_array[ BEH_EV_RND ]    =   behaviorRandom;
    func_ptr_array[ BEH_WALK ]      =   behaviorWalk;
    func_ptr_array[ BEH_YOGA ]      =   behaviorYoga;

}

//----------------------------- LOOP -----------------------------------------------
//----------------------------------------------------------------------------------

void loop() {

  // Update average once per second
  doUpdAvg = (millis() / 1000) > timeToUpdAvg;

  time = millis() * speed;

  for (int legCurrent = 0; legCurrent < LEGS; legCurrent++) {
    currInVal = analogRead(inPin[ legCurrent ]);

    if (doUpdAvg) {
      // lerp 50% towards read value
      avg[ legCurrent ] += currInVal;
      avg[ legCurrent ] /= 2;
    }

    currVal = currInVal - avg[ legCurrent ];
    currVal = map(currVal, -800, 800, 0, 179);

    // detect events
    triggerEvent = false;
    goingUp = pos[ legCurrent ] > lastInVal[ legCurrent ];
    if (goingUp == lastGoingUp[ legCurrent ]) {
      lightChangeDelta += abs(pos[ legCurrent ] - lastInVal[ legCurrent ]);
    } else {
      // when the direction changes check
      // if we traveled far
      if (lightChangeDelta > LIGHT_CHANGE_THRESHOLD) {
        triggerEvent = true;
      }
      lightChangeDelta = 0;
    }
      
      // Perform movement behavior
      func_ptr_array[ behaviorCurrent ]( legCurrent );

    servo[ legCurrent ].write(pos[ legCurrent ]);

    lastInVal[ legCurrent ] = pos[ legCurrent ];
    lastGoingUp[ legCurrent ] = goingUp;

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

//----------------------------- FUNCTION DECLARATION -------------------------------
//----------------------------------------------------------------------------------

void behaviorDirected( int leg )
{
    pos[ leg ] += constrain(currVal, 50, 140);
    pos[ leg ] /= 2;
}

//----------------------------------------------------------------------------------

void behaviorTurn( int leg )
{
    // Sign of +leg  decides rotation direction
    // time decides the rotation speed
    pos[ leg ] = int(95 + 55 * sin(time + leg  * 2.07));
}

//----------------------------------------------------------------------------------

void behaviorRandom( int leg )
{
    if (triggerEvent)
    {
        pos[ leg ] = random( 50, 140 );
    }
}

//----------------------------------------------------------------------------------

void behaviorWalk( int leg )
{
    if ( leg == 0) {
        pos[ leg ] = int(95 + 45 * impulse(7, time - int(time)));
    }
    if ( leg == 1) {
        pos[ leg ] = int(95 - 45 * impulse(7, time - int(time)));
    }
}

//----------------------------------------------------------------------------------

void behaviorYoga( int leg )
{
    pos[ leg ] = int(95 + 55 * (sin(time)));
}

//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------
//----------------------------------------------------------------------------------