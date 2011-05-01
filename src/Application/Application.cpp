/**
 * Application implementation.
 */

#include "assert.h"
#include "stdio.h"
#include "Application.h"
#include "GlutHelper/GlutHelper.h"
#include "GlutHelper/RenderingData.h"
#include "BulletHelper/BulletHelper.h"
#include "Timers/Timer.h"
#include "globals.h"

namespace GameTest {

// Ctor.
Application::Application()
{
}

// Dtor.
Application::~Application()
{
}

// Creates the new application instance, asserts if called more than once.
Application* Application::createInstance()
{
    static Application* instance = NULL;
    assert(NULL == instance);

    instance = new Application();

    return instance;
}

// Initializes the application and all subsystems.
void Application::init(int argc, char *argv[])
{
    GTimer->init();
    GBulletHelper->init();
    GGlutHelper->init(argc, argv);

    //GBulletHelper->addBox(btVector3(0.f, 0.f, 0.f), btVector3(0.1f, 0.1f, 0.1f), 1.f);
    GBulletHelper->debugMethod();
}

// Updates the application and all subsystems.
void Application::update()
{
    float dTime = GTimer->update();
    GBulletHelper->update(dTime);
}

}
