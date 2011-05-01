/**
 * Simple debug renderer declaration.
 */

#ifndef __DEBUG_RENDERER_H__
#define __DEBUG_RENDERER_H__

#include "LinearMath/btIDebugDraw.h"

namespace GameTest {

// The simpliest debug renderer.
class DebugRenderer : public btIDebugDraw {
public:
    
    // Dtor.
    virtual ~DebugRenderer();

    // Basic method drawing a line.
    virtual void drawLine(const btVector3& from, const btVector3& to, const btVector3& color);

    // Draws a sphere.
    virtual void drawSphere(const btVector3& position, btScalar radius, const btVector3& color);
    virtual void drawSphere(btScalar radius, const btTransform& transform, const btVector3& color);

protected:

    // Ctor. Should be instanced only by friend GlutHelper.
    DebugRenderer();

    friend class GlutHelper;

    // Unimplemented methods.
    virtual void drawContactPoint(const btVector3 &,const btVector3 &,btScalar,int,const btVector3 &) {};
    virtual void reportErrorWarning(const char *) {};
    virtual void draw3dText(const btVector3 &,const char *) {};
    virtual void setDebugMode(int) {};
    virtual int getDebugMode(void) const { return DBG_DrawWireframe; };

};


}

#endif