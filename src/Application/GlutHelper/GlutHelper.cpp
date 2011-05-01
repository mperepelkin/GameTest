/**
 * GlutHelper implementation.
 */

#include "assert.h"
#include "Globals.h"
#include "BulletHelper/BulletHelper.h"
#include "GlutHelper/GlutHelper.h"
#include "GlutHelper/GlutHelperRoutines.inl"
#include "GlutHelper/DebugRenderer.h"

namespace GameTest {

// Ctor.
GlutHelper::GlutHelper() :
    m_glutThreadId( NULL ),
    m_renderingDataMutex( NULL ),
    m_debugRenderer( NULL )
{
}

// Dtor.
GlutHelper::~GlutHelper()
{
    delete m_renderingData;
    CloseHandle(m_renderingDataMutex);
    //TerminateThread(&m_glutThreadId, 0);
}

// Creates and initializes new window, starts event loop thread.
// Prerequisites: Physics already created and initialized.
void GlutHelper::init(int argc, char *argv[])
{
    assert(NULL == m_glutThreadId);

    m_renderingDataMutex = CreateMutex(NULL, false, (LPCSTR)("RenderingDataMutex"));
    assert(m_renderingDataMutex);

    btDynamicsWorld* world = GBulletHelper->getWorld();

    m_debugRenderer = new DebugRenderer();
    world->setDebugDrawer(m_debugRenderer);

    m_renderingData = new RenderingData();
    m_renderingData->m_world = world;

    CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)&glutMain, (void*)(argv), 0, &m_glutThreadId);
}

// Creates the new instance of GlutHelper. Checks being called only once.
GlutHelper* GlutHelper::createInstance()
{
    static GlutHelper* instance = NULL;
    assert(NULL == instance);

    instance = new GlutHelper();

    return instance;
}

// Locks the rendering data from writing.
void GlutHelper::lockRenderingData()
{
    DWORD result = WaitForSingleObject(m_renderingDataMutex, (DWORD)(1000 * DESIRED_FRAME_TIME));
    assert(WAIT_OBJECT_0 == result);
}

// Releases the lock.
void GlutHelper::unlockRenderingData()
{
    ReleaseMutex(m_renderingDataMutex);
}

void GlutHelper::renderDebugString(float x, float y, char *string)
{
    glPushMatrix();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glRasterPos2f(x, y);

    for (char * c = string; *c != '\0'; c++)
        glutBitmapCharacter(GLUT_BITMAP_8_BY_13, *c);

    glPopMatrix();
}

}