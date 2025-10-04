#ifndef WEAPON_C
#define WEAPON_C

#include "weapon.h"

FPSWeapon CreateFPSWeapon(const char* modelFilename, const char* textureFilename)
{
    Model model = LoadModel(modelFilename);
    Texture2D texture = LoadTexture(textureFilename);

    model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

    FPSWeapon weapon;
    weapon.model = model;
    weapon.baseOffset = POSITION_OFFSET;
    weapon.currentOffset = POSITION_OFFSET;
    weapon.recoilDirection = (Vector3) {0.5f, 0.1f, 0.0f};
    weapon.defaultRotationAxis = (Vector3) {0.0f, 1.0f, 0.0f};
    weapon.defaultSize = (Vector3) {0.027f, 0.027f, 0.027f};
    weapon.defaultRotationAngle = 180.0f;
    weapon.swayDirection = (Vector3) {0.0f, 0.0f, 0.0f};
    weapon.positionOffset = (Vector3) {0.0f, -0.5f, -0.1f};
    weapon.aimingPositionOffset = (Vector3) {-0.169f, -0.34f, 0.61f};
    weapon.swayTime = 0.0f;
    weapon.swaySpeed = 7.0f;
    weapon.currentSwayAmount - 0.0f;
    weapon.recoilTimer = 0.0f;
    weapon.recoilStrength = 1.0f;
    weapon.recoilPositionRecover = 45.0f;
    weapon.recoilAngleRecover = 7.5f;
    weapon.recoilAngle = 0.0f;
    weapon.isShooting = false;
    weapon.isAiming = false;
    return weapon;
}


static void UpdateWeaponSway(FPSWeapon* weapon, Vector3 playerVelocity)
{
    bool isMoving = Vector3Length(playerVelocity) > VELOCITY_TRESHOLD_FOR_WEAPON_SWAY;

    if(isMoving)
    {
        weapon->currentSwayAmount = Clamp(weapon->currentSwayAmount + GetFrameTime() * weapon->swaySpeed, MIN_WEAPON_SWAY_AMOUNT, MAX_WEAPON_SWAY_AMOUNT);
    }
    else 
    {
        weapon->currentSwayAmount = Clamp(weapon->currentSwayAmount - GetFrameTime() * weapon->swaySpeed, MIN_WEAPON_SWAY_AMOUNT, MAX_WEAPON_SWAY_AMOUNT);
    }

    weapon->swayTime += GetFrameTime() * weapon->swaySpeed;

    float swayX = sinf(weapon->swayTime * DEFAULT_SWAY_TIME_X_MOD) * DEFAULT_SWAY_X_AMOUNT * weapon->currentSwayAmount;
    float swayY = cosf(weapon->swayTime * DEFAULT_SWAY_TIME_Y_MOD) * DEFAULT_SWAY_Y_AMOUNT * weapon->currentSwayAmount;

    weapon->swayDirection = (Vector3) {swayX, swayY, 0.0f};
}

static void UpdateWeaponRecoil(FPSWeapon* weapon)
{
    if(weapon->isShooting)
    {
        weapon->recoilAngle += weapon->recoilStrength;
    }

    if(weapon->recoilAngle > 0.0f)
    {
        weapon->recoilAngle -= GetFrameTime() * weapon->recoilAngleRecover * weapon->recoilAngle;
        if(weapon->recoilAngle < 0.0f)
        {
            weapon->recoilAngle = 0.0f;
        }
    }
} 

void UpdateWeapon(FPSWeapon *weapon, Camera camera, Vector3 playerVelocity)
{
    if (weapon->isShooting)
    {
        weapon->recoilTimer += GetFrameTime();
        
        float recoilProgress = weapon->recoilTimer / 0.1f;
        if (recoilProgress < 1.0f)
        {
            weapon->currentOffset.z = weapon->baseOffset.z + weapon->recoilStrength * sinf(recoilProgress * PI);
        }
        else
        {
            weapon->currentOffset.z = Lerp(weapon->currentOffset.z, weapon->baseOffset.z, GetFrameTime() * weapon->recoilPositionRecover) ;
            weapon->isShooting = false;
            weapon->recoilTimer = 0.0f;
        }
    }

    UpdateWeaponSway(weapon, playerVelocity);
    UpdateWeaponRecoil(weapon);
}

void DrawWeapon(FPSWeapon weapon)
{
    Vector3 currentWeaponPosition = {0};
                
    Quaternion qY = QuaternionFromAxisAngle((Vector3){0, 1, 0}, DEG2RAD * weapon.defaultRotationAngle);
    
    Quaternion qX = QuaternionFromAxisAngle(weapon.recoilDirection, DEG2RAD * weapon.recoilAngle);


    Quaternion qFinal = QuaternionMultiply(qX, qY);

    Vector3 axis;
    float angle;
    QuaternionToAxisAngle(qFinal, &axis, &angle);

    if(weapon.isAiming)
    {
        currentWeaponPosition = Vector3Add(Vector3Add(weapon.swayDirection, weapon.aimingPositionOffset), weapon.currentOffset);
    }
    else
    {
        currentWeaponPosition = Vector3Add(Vector3Add(weapon.swayDirection, weapon.positionOffset), weapon.currentOffset);
    }

    DrawModelEx(weapon.model, currentWeaponPosition, axis, RAD2DEG * angle, weapon.defaultSize, WHITE);
}

void DestroyWeapon(FPSWeapon weapon)
{
    UnloadModel(weapon.model);
    UnloadTexture(weapon.texture);
}

#endif //WEAPON_C