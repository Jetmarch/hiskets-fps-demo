#include "raylib.h"
#include "raymath.h"

typedef struct {
    Model model;
    Vector3 baseOffset;
    Vector3 currentOffset;
    Vector3 recoilDirection;
    Vector3 defaultRotationAxis;
    Vector3 defaultSize;
    Vector3 swayDirection;
    Vector3 positionOffset;
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
} FPSWeapon;


FPSWeapon CreateFPSWeapon(Model model, Vector3 offset);
void UpdateWeapon(FPSWeapon *weapon, Camera camera, Vector3 playerVelocity);
void DrawWeapon(FPSWeapon weapon);


#define DEFAULT_SWAY_TIME_X_MOD 1.0f
#define DEFAULT_SWAY_TIME_Y_MOD 2.0f
#define DEFAULT_SWAY_X_AMOUNT 0.1f
#define DEFAULT_SWAY_Y_AMOUNT 0.05f

#define MAX_WEAPON_SWAY_AMOUNT 1.0f
#define MIN_WEAPON_SWAY_AMOUNT 0.0f

#define VELOCITY_TRESHOLD_FOR_WEAPON_SWAY 0.7f