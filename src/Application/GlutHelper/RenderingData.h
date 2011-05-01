/**
 * Rendering data structure declaration.
 * This is the structure filled by main thread and read by rendering thread.
 */

#ifndef __RENDERING_DATA_H__
#define __RENDERING_DATA_H__

class btDynamicsWorld;

namespace GameTest {

// The structure for data shared between main and rendering threads.
struct RenderingData
{
    btDynamicsWorld* m_world;
};

}

#endif