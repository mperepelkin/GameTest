/**
 * Some comments go here.
 */

#ifndef __DYNAMIC_SHAPE_H__
#define __DYNAMIC_SHAPE_H__

#include "LinearMath/btVector3.h"
#include "BulletCollision/CollisionShapes/btTriangleInfoMap.h"
#include "BulletCollision/CollisionShapes/btTriangleMeshShape.h"
#include "BulletCollision/CollisionShapes/btOptimizedBvh.h"
#include "BulletCollision/CollisionShapes/btTriangleMesh.h"


namespace GameTest {

ATTRIBUTE_ALIGNED16(class) DynamicShape : public btTriangleMeshShape
{
    btTriangleMesh* m_mesh;

public:

    DynamicShape() : btTriangleMeshShape(new btTriangleMesh())
    {
        m_mesh = static_cast<btTriangleMesh*>(m_meshInterface);
    };

    virtual ~DynamicShape() { delete m_mesh; };

    void addTriangle(const btVector3& vertex0,
        const btVector3& vertex1,
        const btVector3& vertex2);

	//debugging
	virtual const char*	getName() const { return "DYNAMICMESH"; }
};

}

#endif