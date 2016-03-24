//----------------------------- FUNCTION DECLARATION -------------------------------
//----------------------------------------------------------------------------------

int behaviorDirect( State state, int leg, int servoPosCurrent )
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
  switch ( leg ) {
    case 0:
      return int(95 + 45 * impulse(7, state.time - int(state.time) ) );
    case 1:
      return int(95 - 45 * impulse(7, state.time - int(state.time) ) );
    case 2:
      // third leg is more or less straight, but controllable
      return int(constrain( state.currLightValue, 75, 115 ));
  }
  return servoPosCurrent;
}

//----------------------------------------------------------------------------------

int behaviorYoga( State state, int leg, int servoPosCurrent )
{
  return int(95 + 55 * ( sin( state.time ) ) );
}

//----------------------------------------------------------------------------------

int behaviorDead( State state, int leg, int servoPosCurrent )
{
  return int(95 + 55 * ( sin( leg + state.time * 0.001 ) ) );
}
