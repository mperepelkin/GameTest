/**
 * Simple camera implementation.
 */

#include "Camera.h"
#include "Globals.h"
#include <string.h>

namespace GameTest {

Camera::Camera() :
    MouseButtonState(0),    
    m_yaw(PI_F),
    m_pitch(0.f),
    m_fov(90.f),
    MouseX(0),
    MouseY(0),
    MovementSpeedX(0.001f),
    MovementSpeedY(-0.001f),
    MovementSpeedZ(0.001f),
    RotationSpeedYaw(0.001f),
    RotationSpeedPitch(-0.001f)
{
    m_transform = btTransform(btQuaternion(m_yaw, m_pitch, 0.f), btVector3(0.0f, 0.f, -1.0f));
}

void Camera::rotate(btScalar yaw, btScalar pitch)
{
    m_yaw -= yaw;
    m_pitch += pitch;

    m_transform.setRotation(btQuaternion(m_yaw, m_pitch, 0.f));
}

void Camera::move(btScalar dx, btScalar dy, btScalar dz)
{
    m_transform.setOrigin(m_transform.getOrigin() + btVector3(dx, dy, dz));
}

void Camera::moveLocal(btScalar dx, btScalar dy, btScalar dz)
{
    m_transform.setOrigin(m_transform.getOrigin() + quatRotate(m_transform.getRotation(), btVector3(dx, dy, dz)));
}

void Camera::getViewMatrix(btScalar *matrix) const
{
    m_transform.inverse().getOpenGLMatrix(matrix);
}

}
