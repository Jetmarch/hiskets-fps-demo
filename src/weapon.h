
#ifndef WEAPON_H
#define WEAPON_H

#include "raylib.h"
#include "raymath.h"

struct FPSWeapon {
    Model model;
    Texture2D texture;
    Vector3 baseOffset;
    Vector3 currentOffset;
    Vector3 recoilDirection;
    Vector3 defaultRotationAxis;
    Vector3 defaultSize;
    Vector3 swayDirection;
    Vector3 positionOffset;
    Vector3 aimingPositionOffset;
    float defaultRotationAngle;
    float swayTime;
    float swaySpeed;
    float currentSwayAmount;
    float recoilTimer;
    float recoilStrength;
    float recoilPositionRecover;
    float recoilAngleRecover;
    float recoilAngle;
    bool isShooting;
    bool isAiming;
};

typedef struct FPSWeapon FPSWeapon;

FPSWeapon CreateFPSWeapon(const char* modelFilename, const char* textureFilename);
void UpdateWeapon(FPSWeapon *weapon, Camera camera, Vector3 playerVelocity);
void DrawWeapon(FPSWeapon weapon);
void DestroyWeapon(FPSWeapon weapon);


#define DEFAULT_SWAY_TIME_X_MOD 1.0f
#define DEFAULT_SWAY_TIME_Y_MOD 2.0f
#define DEFAULT_SWAY_X_AMOUNT 0.1f
#define DEFAULT_SWAY_Y_AMOUNT 0.05f

#define MAX_WEAPON_SWAY_AMOUNT 1.0f
#define MIN_WEAPON_SWAY_AMOUNT 0.0f

#define VELOCITY_TRESHOLD_FOR_WEAPON_SWAY 0.7f
#define POSITION_OFFSET (Vector3){0.2f, -0.5f, -0.5f}

#endif //WEAPON_H