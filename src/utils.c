#ifndef UTILS_C
#define UTILS_C

#include "utils.h"

Vector3 GetCameraEulerAngles(Camera camera)
{
    Vector3 forward = Vector3Normalize(Vector3Subtract(camera.target, camera.position));
    Vector3 right = Vector3Normalize(Vector3CrossProduct(forward, camera.up));
    Vector3 up = Vector3CrossProduct(right, forward);
    
    Vector3 angles;
    
    //yaw
    angles.y = atan2f(forward.x, forward.z);
    //pitch
    angles.x = asinf(-forward.y);
    //roll
    angles.z = 0.0f;
    
    return angles;
}

#endif //UTILS_C