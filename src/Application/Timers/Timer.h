/**
 * Interface for game timer classes.
 */

#ifndef __TIMER_H__
#define __TIMER_H__

#include "Globals.h"

namespace GameTest {

// Abstract interface for game timer.
class Timer
{
public:
    // Creates the particular timer implementation instance. Should be called once.
    static Timer* createInstance();

    // Initializes the timer instance.
    virtual void init() = 0;

    // Should update the internal counters.
    // Could sleep until desired time comes.
    // Returns time passed since last update.
    virtual float update() = 0;

    // Returns in-game time passed since timer initialization.
    virtual float getTime() = 0;

    // Returns absolute time passed since timer initialization.
    virtual float getAbsoluteTime() = 0;

    // Returns last update delta time.
    virtual float getLastDTime() = 0;

protected:

    // Ctor.
    Timer() {};

    // Dtor.
    virtual ~Timer() {};
};

}

#endif