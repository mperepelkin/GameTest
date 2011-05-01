/**
 * Default camera interface.
 */

#ifndef __CAMERA_H__
#define __CAMERA_H__

#include "LinearMath/btTransform.h"

namespace GameTest {

class Camera {
public:

    Camera();
    
    void rotate(btScalar yaw, btScalar pitch);

    void move(btScalar dx, btScalar dy, btScalar dz);

    void moveLocal(btScalar dx, btScalar dy, btScalar dz);

    void getViewMatrix(btScalar* matrix) const;

    const btTransform& getViewTransform() const { return m_transform; };

    enum MouseButton {
        MOUSE_BUTTON_NONE  = 0x0000,
        MOUSE_BUTTON_LEFT  = 0x0001,
        MOUSE_BUTTON_RIGHT = 0x0002
    };

    int MouseButtonState;
    int MouseX;
    int MouseY;

    btScalar MovementSpeedX;
    btScalar MovementSpeedY;
    btScalar MovementSpeedZ;
    btScalar RotationSpeedYaw;
    btScalar RotationSpeedPitch;
    
private:

    btTransform m_transform;
    btScalar m_yaw;
    btScalar m_pitch;
    btScalar m_fov;
};

}

#endif