//----------------------------- FUNCTION DECLARATION -------------------------------
//----------------------------------------------------------------------------------
//
//int behaviorDirected( State state )
//{
//    pos[ leg ] += constrain(currVal, 50, 140);
//    pos[ leg ] /= 2;
//}
//
////----------------------------------------------------------------------------------
//
//int behaviorTurn( State state )
//{
//    // Sign of +leg  decides rotation direction
//    // time decides the rotation speed
//    pos[ leg ] = int(95 + 55 * sin(time + leg  * 2.07));
//}
//
////----------------------------------------------------------------------------------
//
//int behaviorRandom( State state )
//{
//    if (triggerEvent)
//    {
//        pos[ leg ] = random( 50, 140 );
//    }
//}
//
////----------------------------------------------------------------------------------
//
//int behaviorWalk( State state )
//{
//    if ( leg == 0) {
//        pos[ leg ] = int(95 + 45 * impulse(7, time - int(time)));
//    }
//    if ( leg == 1) {
//        pos[ leg ] = int(95 - 45 * impulse(7, time - int(time)));
//    }
//}
//
////----------------------------------------------------------------------------------
//
//int behaviorYoga( State state )
//{
//    pos[ leg ] = int(95 + 55 * (sin(time)));
//}
