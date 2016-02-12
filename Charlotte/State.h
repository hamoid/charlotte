//
// Created by Peter A. Kolski on 12.02.16.
//

#ifndef CHARLOTTE_STATE_H
#define CHARLOTTE_STATE_H

// passing the position into the 'State' would make a mess
// init of arrays dynamically (as constuctor) isn't nice
struct State
{
    float time          = 0.0;
    int   currentValue  = 0;
    bool  triggerEvent  = false;
};

#endif //CHARLOTTE_STATE_H
