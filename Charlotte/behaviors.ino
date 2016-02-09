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
