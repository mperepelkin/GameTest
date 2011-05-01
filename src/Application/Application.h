/**
 * Application declaration.
 */

#ifndef __APPLICATION_H__
#define __APPLICATION_H__

#include "windows.h"

namespace GameTest {

// The main application ticking class.
class Application
{
public:

    // Creates the new application instance, asserts if called more than once.
    static Application* createInstance();

    // Initializes the application and all subsystems.
    void init(int argc, char *argv[]);

    // Updates the application and all subsystems.
    void update();

protected:

    // Ctor.
    Application();

    // Dtor.
    ~Application();
};

}

#endif