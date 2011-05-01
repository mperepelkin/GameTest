/**
 * Header file containing all global variables declaration. Fuck singletons.
 */

#ifndef __GLOBALS_H__
#define __GLOBALS_H__

#define DESIRED_FPS 60.f
#define DESIRED_FRAME_TIME 0.0167f
#define PI_F 3.141592653f
#define ASPECT_RATIO 1.3333333f

namespace GameTest {

extern class GlutHelper*   GGlutHelper;
extern class Application*  GApplication;
extern class Timer*        GTimer;
extern class BulletHelper* GBulletHelper;

// TODO: To be moved to some camera manager. For now just stick to one camera instance.
extern class Camera*       GCamera;

}

#endif