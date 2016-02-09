//----------------------------- FUNCTION DECLARATION -------------------------------
//----------------------------------------------------------------------------------


int behaviorDirected( State state, int leg )
{
    //TODO do I have to pass the position?
//  pos[ leg ] += constrain( state.currentValue, 50, 140);
//  pos[ leg ] /= 2;
}

//----------------------------------------------------------------------------------

int behaviorTurn( State state, int leg  )
{
  // Sign of +leg  decides rotation direction
  // time decides the rotation speed
    return int(95 + 55 * sin( state.time + leg  * 2.07));
}

//----------------------------------------------------------------------------------

int behaviorRandom( State state, int leg )
{
  if ( state.triggerEvent )
  {
      return random( 50, 140 );
  }
}

//----------------------------------------------------------------------------------

int behaviorWalk( State state, int leg )
{
  if ( leg == 0) {
      return int(95 + 45 * impulse(7, state.time - int(state.time) ) );
  }
  if ( leg == 1) {
      return int(95 - 45 * impulse(7, state.time - int(state.time) ) );
  }
}

//----------------------------------------------------------------------------------

int behaviorYoga( State state, int leg )
{
  return int(95 + 55 * ( sin( state.time ) ) );
}
