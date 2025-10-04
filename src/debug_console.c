#ifndef DEBUG_CONSOLE_C
#define DEBUG_CONSOLE_C

#include "debug_console.h"

void DrawWeaponDebugWindow(struct nk_context* ctx, FPSWeapon* weapon)
{
    if (nk_begin(ctx, "Base position", nk_rect(100, 100, 500, 500), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE)) 
    {
        nk_layout_row_static(ctx, 50, 150, 1);

        nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
        {
            nk_layout_row_push(ctx, 100);
            nk_label(ctx, "Weapon X:", NK_TEXT_LEFT);
            nk_layout_row_push(ctx, 200);
            nk_slider_float(ctx, MIN_WEAPON_OFFSET_COORD, &weapon->positionOffset.x, MAX_WEAPON_OFFSET_COORD, WEAPON_OFFSET_STEP);
            nk_layout_row_push(ctx, 200);
            nk_value_float(ctx, "Weapon X", weapon->positionOffset.x);
        }
        nk_layout_row_end(ctx);

        nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
        {
            nk_layout_row_push(ctx, 100);
            nk_label(ctx, "Weapon Y:", NK_TEXT_LEFT);
            nk_layout_row_push(ctx, 200);
            nk_slider_float(ctx, MIN_WEAPON_OFFSET_COORD, &weapon->positionOffset.y, MAX_WEAPON_OFFSET_COORD, WEAPON_OFFSET_STEP);
            nk_layout_row_push(ctx, 200);
            nk_value_float(ctx, "Weapon Y", weapon->positionOffset.y);
        }

        nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
        {
            nk_layout_row_push(ctx, 100);
            nk_label(ctx, "Weapon Z:", NK_TEXT_LEFT);
            nk_layout_row_push(ctx, 200);
            nk_slider_float(ctx, MIN_WEAPON_OFFSET_COORD, &weapon->positionOffset.z, MAX_WEAPON_OFFSET_COORD, WEAPON_OFFSET_STEP);
            nk_layout_row_push(ctx, 200);
            nk_value_float(ctx, "Weapon Z", weapon->positionOffset.z);
        }
        nk_layout_row_end(ctx);
    }
    nk_end(ctx);

    if (nk_begin(ctx, "Aim position", nk_rect(400, 100, 500, 500), NK_WINDOW_BORDER|NK_WINDOW_MOVABLE)) 
    {
        nk_layout_row_static(ctx, 50, 150, 1);

        nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
        {
            nk_layout_row_push(ctx, 100);
            nk_label(ctx, "Weapon X:", NK_TEXT_LEFT);
            nk_layout_row_push(ctx, 200);
            nk_slider_float(ctx, MIN_WEAPON_OFFSET_COORD, &weapon->aimingPositionOffset.x, MAX_WEAPON_OFFSET_COORD, WEAPON_OFFSET_STEP);
            nk_layout_row_push(ctx, 200);
            nk_value_float(ctx, "Weapon X", weapon->aimingPositionOffset.x);
        }
        nk_layout_row_end(ctx);

        nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
        {
            nk_layout_row_push(ctx, 100);
            nk_label(ctx, "Weapon Y:", NK_TEXT_LEFT);
            nk_layout_row_push(ctx, 200);
            nk_slider_float(ctx, MIN_WEAPON_OFFSET_COORD, &weapon->aimingPositionOffset.y, MAX_WEAPON_OFFSET_COORD, WEAPON_OFFSET_STEP);
            nk_layout_row_push(ctx, 200);
            nk_value_float(ctx, "Weapon Y", weapon->aimingPositionOffset.y);
        }

        nk_layout_row_begin(ctx, NK_STATIC, 30, 2);
        {
            nk_layout_row_push(ctx, 100);
            nk_label(ctx, "Weapon Z:", NK_TEXT_LEFT);
            nk_layout_row_push(ctx, 200);
            nk_slider_float(ctx, MIN_WEAPON_OFFSET_COORD, &weapon->aimingPositionOffset.z, MAX_WEAPON_OFFSET_COORD, WEAPON_OFFSET_STEP);
            nk_layout_row_push(ctx, 200);
            nk_value_float(ctx, "Weapon Z", weapon->aimingPositionOffset.z);
        }
        nk_layout_row_end(ctx);
    }
    nk_end(ctx);
}

#endif //DEBUG_CONSOLE_C