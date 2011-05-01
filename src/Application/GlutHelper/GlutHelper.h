/**
 * GlutHelper declaration.
 */

#ifndef __GLUT_HELPER_H__
#define __GLUT_HELPER_H__

#include <windows.h>
#include <Glut/GL/glut.h>
#include "Globals.h"
#include "LinearMath/btTransform.h"

class btIDebugDraw;
class btTransform;

namespace GameTest {

struct RenderingData;

/**
 * Rendering helper class.
 */
class GlutHelper {
public:

    // Creates the new instance of GlutHelper. Checks being called only once.
    static GlutHelper* createInstance();

    // Creates and initializes new window, starts event loop thread.
    // Prerequisites: Physics already created and initialized.
    void init(int argc, char *argv[]);
    
    // Locks the rendering data from writing.
    void lockRenderingData();

    // Releases the lock.
    void unlockRenderingData();

    // Gets the rendering data structure pointer.
    RenderingData* getRenderingData() { return m_renderingData; };

    // Renders the debug string on given screen coordinates.
    void renderDebugString(float x, float y, char *string);

protected:

    // Ctor.
    GlutHelper();

    // Dtor.
    ~GlutHelper();

private:

    // The thread running glut message loop.
    DWORD m_glutThreadId;

    // The rendering data access mutex.
    HANDLE m_renderingDataMutex;

    // Pointer to rendering data structure.
    RenderingData* m_renderingData;

    // Pointer to debug renderer instance.
    btIDebugDraw* m_debugRenderer;

};

}

#endif