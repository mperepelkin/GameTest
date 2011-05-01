/**
 * All initialization of global variables go here.
 * Lets see where it comes.
 */

#include "Globals.h"
#include "Application.h"
#include "GlutHelper/GlutHelper.h"
#include "Timers/FixedTimer.h"
#include "BulletHelper/BulletHelper.h"
#include "Camera.h"

using namespace GameTest;

Application*  GameTest::GApplication  = Application::createInstance();
GlutHelper*   GameTest::GGlutHelper   = GlutHelper::createInstance();
BulletHelper* GameTest::GBulletHelper = BulletHelper::createInstance();
Timer*        GameTest::GTimer        = FixedTimer::createInstance();
Camera*       GameTest::GCamera       = new Camera();
