/**
 * Simple debug renderer implementation.
 */

#include "GlutHelper/DebugRenderer.h"
#include <Glut/GL/glut.h>

namespace GameTest {

// Ctor.
DebugRenderer::DebugRenderer()
{
}

// Dtor.
DebugRenderer::~DebugRenderer()
{
}

void DebugRenderer::drawLine(const btVector3& from, const btVector3& to, const btVector3& color)
{
    glBegin(GL_LINES);
		glColor3f(color.getX(), color.getY(), color.getZ());
		glVertex3d(from.getX(), from.getY(), from.getZ());
		glVertex3d(to.getX(), to.getY(), to.getZ());
	glEnd();
}

void DebugRenderer::drawSphere(btScalar radius, const btTransform& transform, const btVector3& color)
{
    glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );
    glColor3b(127, 127, 127);
    glPushMatrix();

    btVector3 position = transform.getOrigin();
    glTranslatef(position.getX(), position.getY(), position.getZ());

    btQuaternion quat = transform.getRotation();
    glRotatef(180.f * quat.getAngle() / SIMD_PI, quat.getAxis().getX(), quat.getAxis().getY(), quat.getAxis().getZ());

    int lats = 4;
    int longs = 8;

    for (int i = 0; i <= lats; i++) {
        btScalar lat0 = SIMD_PI * (-btScalar(0.5) + (btScalar) (i - 1) / lats);
        btScalar z0   = radius*sin(lat0);
        btScalar zr0  = radius*cos(lat0);

        btScalar lat1 = SIMD_PI * (-btScalar(0.5) + (btScalar) i / lats);
        btScalar z1   = radius*sin(lat1);
        btScalar zr1  = radius*cos(lat1);

        glBegin(GL_QUAD_STRIP);
        for (int j = 0; j <= longs; j++) {
            btScalar lng = 2 * SIMD_PI * (btScalar) (j - 1) / longs;
            btScalar x   = cos(lng);
            btScalar y   = sin(lng);

            glNormal3f(x * zr0, y * zr0, z0);
            glVertex3f(x * zr0, y * zr0, z0);
            glNormal3f(x * zr1, y * zr1, z1);
            glVertex3f(x * zr1, y * zr1, z1);
        }
        glEnd();
    }

    glPopMatrix();
    glPolygonMode( GL_FRONT_AND_BACK, GL_FILL );
}

void DebugRenderer::drawSphere(const btVector3& position, btScalar radius, const btVector3& color)
{
    btTransform transform = btTransform(btQuaternion::getIdentity(), position);
    drawSphere(radius, transform, color);
}

}