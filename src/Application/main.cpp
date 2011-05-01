/**
* Some freakin copyright here just to be awsum.
*/

#include "stdio.h"
#include "windows.h"
#include "Globals.h"
#include "Application.h"
#include "btBulletDynamicsCommon.h"

using namespace GameTest;

int main(int argc, char *argv[])
{
    GApplication->init(argc, argv);

    while (true) {
        GApplication->update();
    }

    return 0;
}