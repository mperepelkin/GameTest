/**
 * Fixed timer declaration.
 */

#ifndef __FIXED_TIMER_H__
#define __FIXED_TIMER_H__

#include "ctime"
#include "Timers/Timer.h"

namespace GameTest {

// Ticks 0.0167s irrelevant of actual time passed.
class FixedTimer : public Timer
{
public:

    // Creates the fixed timer instance. Should be called once.
    static Timer* createInstance();

    // Initializes the timer, resets the internal counters.
    virtual void init();

    // Sleeps until desired time comes and updates internal counters.
    // Returns time passed since last update.
    virtual float update();

    // Returns in-game time passed since timer initialization.
    virtual float getTime();

    // Returns absolute time passed since timer initialization.
    virtual float getAbsoluteTime();

    // Returns last update delta time.
    virtual float getLastDTime();

protected:

    // Ctor.
    FixedTimer();

    // Dtor.
    virtual ~FixedTimer();

private:

    // Initialization time.
    clock_t m_initTime;

    // Time of the last update call.
    clock_t m_lastUpdateTime;

    // In-game passed time.
    clock_t m_elapsedGameTime;

    // Last update's time delta.
    float m_lastDTime;
};





}



#endif
