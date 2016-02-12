//----------------------------- FUNCTION DECLARATION -------------------------------
//----------------------------------------------------------------------------------

int behaviorDirected( State state, int leg, int servoPosCurrent )
{
    //TODO check 'int' precision -> what's the pupose of that function?
    return ( servoPosCurrent + constrain( state.currLightValue, 50, 140 ) ) / 2;
}

//----------------------------------------------------------------------------------

int behaviorTurn( State state, int leg, int servoPosCurrent )
{
  // Sign of +leg  decides rotation direction
  // time decides the rotation speed
    return int(95 + 55 * sin( state.time + leg  * 2.07));
}

//----------------------------------------------------------------------------------

int behaviorRandom( State state, int leg, int servoPosCurrent )
{
  return state.triggerEvent ? random( 50, 140 ) : servoPosCurrent;
}

//----------------------------------------------------------------------------------

int behaviorWalk( State state, int leg, int servoPosCurrent )
{
  if ( leg == 0) {
      return int(95 + 45 * impulse(7, state.time - int(state.time) ) );
  }
  if ( leg == 1) {
      return int(95 - 45 * impulse(7, state.time - int(state.time) ) );
  }
  return servoPosCurrent;
}

//----------------------------------------------------------------------------------

int behaviorYoga( State state, int leg, int servoPosCurrent )
{
  return int(95 + 55 * ( sin( state.time ) ) );
}
