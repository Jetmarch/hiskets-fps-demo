
#ifndef DEBUG_CONSOLE_H

#include "weapon.h"
#include "raylib-nuklear.h"


#define MIN_WEAPON_OFFSET_COORD -5.0f
#define MAX_WEAPON_OFFSET_COORD  5.0f
#define WEAPON_OFFSET_STEP       0.01f

void DrawWeaponDebugWindow(struct nk_context* ctx, FPSWeapon* weapon);

#endif //DEBUG_CONSOLE_H