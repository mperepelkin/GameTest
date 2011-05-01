/**
 * Contains GlutHelper ccallback routines.
 */

#ifndef __GLUT_HELPER_ROUTINES_INL__
#define __GLUT_HELPER_ROUTINES_INL__

#include <stdio.h>
#include <math.h>
#include "GlutHelper/RenderingData.h"
#include "GlutHelper/GlutHelper.h"
#include "btBulletDynamicsCommon.h"
#include "Camera.h"

static void renderDebugData();

// Renders the scene.
static void render()
{
    glClear(GL_COLOR_BUFFER_BIT);
    
    glPushMatrix();
    {
        btScalar viewMatrix[16];
        GameTest::GCamera->getViewMatrix(viewMatrix);
        glMultMatrixf(viewMatrix);
        GameTest::GGlutHelper->lockRenderingData();
        GameTest::GGlutHelper->getRenderingData()->m_world->debugDrawWorld();
        GameTest::GGlutHelper->unlockRenderingData();
    }
    glPopMatrix();

    renderDebugData();

    glFlush();
}

static void renderDebugData()
{
    char debugString[255];
    btVector3 origin = GameTest::GCamera->getViewTransform().getOrigin();
    sprintf_s(debugString, "x: %2.2f y: %2.2f z: %2.2f", origin.getX(), origin.getY(), origin.getZ());
    GameTest::GGlutHelper->renderDebugString(-1.f, 0.95f, debugString);
}

static void timedRender(int value)
{
    render();

    glutTimerFunc((unsigned int)(1000 * DESIRED_FRAME_TIME), timedRender, 0);
}

static void mouseButton(int button, int state, int x, int y)
{
    GameTest::GCamera->MouseX = x;
    GameTest::GCamera->MouseY = y;

    if (button == GLUT_LEFT_BUTTON)
    {
        if (state == GLUT_DOWN)
            GameTest::GCamera->MouseButtonState |= GameTest::Camera::MOUSE_BUTTON_LEFT;
        else
            GameTest::GCamera->MouseButtonState &= ~GameTest::Camera::MOUSE_BUTTON_LEFT;
    }

    if (button == GLUT_RIGHT_BUTTON)
    {
        if (state == GLUT_DOWN)
            GameTest::GCamera->MouseButtonState |= GameTest::Camera::MOUSE_BUTTON_RIGHT;
        else
            GameTest::GCamera->MouseButtonState &= ~GameTest::Camera::MOUSE_BUTTON_RIGHT;
    }
}

static void mouseMotion(int x, int y)
{
    if (GameTest::GCamera->MouseButtonState & GameTest::Camera::MOUSE_BUTTON_RIGHT)
    {
        GameTest::GCamera->moveLocal(
            (x - GameTest::GCamera->MouseX) * GameTest::GCamera->MovementSpeedX,
            (y - GameTest::GCamera->MouseY) * GameTest::GCamera->MovementSpeedY,
            0.f);
    }

    if (GameTest::GCamera->MouseButtonState & GameTest::Camera::MOUSE_BUTTON_LEFT)
    {
        GameTest::GCamera->rotate(
            (x - GameTest::GCamera->MouseX) * GameTest::GCamera->RotationSpeedYaw,
            (y - GameTest::GCamera->MouseY) * GameTest::GCamera->RotationSpeedPitch);
    }

    GameTest::GCamera->MouseX = x;
    GameTest::GCamera->MouseY = y;
}

static void keyboard(unsigned char key, int x, int y)
{
    const float kbdSpeed = 50.f;

    // TODO: Move to camera update for smoothness.
    switch (key) {
        case 'w' :
            GameTest::GCamera->moveLocal(0.f, 0.f, -kbdSpeed * GameTest::GCamera->MovementSpeedZ);
            break;
        case 's' :
            GameTest::GCamera->moveLocal(0.f, 0.f, kbdSpeed * GameTest::GCamera->MovementSpeedZ);
            break;
        case 'a' :
            GameTest::GCamera->moveLocal(-kbdSpeed * GameTest::GCamera->MovementSpeedX, 0.f, 0.f);
            break;
        case 'd' :
            GameTest::GCamera->moveLocal(kbdSpeed * GameTest::GCamera->MovementSpeedX, 0.f, 0.f);
            break;
        default:
            break;
    };
}


// Main entry function of glut helper thread.
int glutMain(void* parameter)
{
    (void) parameter;
    int argc = 1;
    char** argv = static_cast<char**>(parameter);

    glutInit( &argc, argv );
    glutInitDisplayMode(GLUT_DEPTH | GLUT_SINGLE | GLUT_RGBA);
    glutInitWindowPosition(100,100);
    glutInitWindowSize(800,600);
    glutCreateWindow("GameTest");
    glutDisplayFunc(render);
    glutMouseFunc(mouseButton);
    glutMotionFunc(mouseMotion);
    glutKeyboardFunc(keyboard);
    glutTimerFunc((unsigned int)(1000 * DESIRED_FRAME_TIME), timedRender, 0);
    glMatrixMode(GL_PROJECTION);
    gluPerspective(45, 1.f, 0.1, 20.0);
    glMatrixMode(GL_MODELVIEW);

    glutMainLoop();

    return 0;
}

#endif