/**
 * Fixed timer implementation.
 */

#include "assert.h"
#include "windows.h"
#include "Timers/FixedTimer.h"

namespace GameTest {

// Ctor.
FixedTimer::FixedTimer() : Timer()
{
}

// Dtor.
FixedTimer::~FixedTimer()
{
}

// Creates the fixed timer instance.
Timer* FixedTimer::createInstance()
{
    static Timer* instance = NULL;
    assert(NULL == instance);

    instance = new FixedTimer();

    return instance;
}

// Initializes the timer.
void FixedTimer::init()
{
    m_initTime = m_lastUpdateTime = clock();

    // Fuck the division by zero.
    m_elapsedGameTime = (clock_t)(1000.f * DESIRED_FRAME_TIME);
    m_lastDTime       = DESIRED_FRAME_TIME;
}

// Updates the timer.
float FixedTimer::update()
{
    float result        = DESIRED_FRAME_TIME;
    clock_t currentTime = clock();

    // Sleep the resting time.
    if ( currentTime - m_lastUpdateTime < 1000.f * DESIRED_FRAME_TIME) {
        Sleep((clock_t)(m_lastUpdateTime + 1000.f * DESIRED_FRAME_TIME - currentTime));
        currentTime = clock();
    }

    //result = 0.001f * (currentTime - m_lastUpdateTime);
    result = DESIRED_FRAME_TIME;

    //m_elapsedGameTime += currentTime - m_lastUpdateTime;
    m_elapsedGameTime += (int)(1000 * DESIRED_FRAME_TIME);
    m_lastUpdateTime  = currentTime;
    m_lastDTime       = result;

    return result;
}

// Returns in-game time passed since timer initialization.
float FixedTimer::getTime()
{
    return (0.001f * m_elapsedGameTime);
}

// Returns absolute time passed since timer initialization.
float FixedTimer::getAbsoluteTime()
{
    return (0.001f * (clock() - m_initTime));
}

// Get the last update's delta time.
float FixedTimer::getLastDTime()
{
    return (m_lastDTime);
}


}