/**
 * Dynamic shape implementation.
 */

#include "BulletHelper/DynamicShape.h"

namespace GameTest {

void DynamicShape::addTriangle(const btVector3& vertex0,
                          const btVector3& vertex1,
                          const btVector3& vertex2)
{
    m_mesh->addTriangle(vertex0, vertex1, vertex2, true);
}

}
