
#define RAYLIB_NUKLEAR_IMPLEMENTATION
#include "player.h"
#include "level.h"
#include "weapon.h"
#include "raylib-nuklear.h"

#define MIN_WEAPON_OFFSET_COORD -5.0f
#define MAX_WEAPON_OFFSET_COORD  5.0f
#define WEAPON_OFFSET_STEP       0.01f


void DrawWeaponDebugWindow(struct nk_context* ctx, FPSWeapon* weapon);

int main(void)
{
    const int screenWidth = 1270;
    const int screenHeight = 920;

    InitWindow(screenWidth, screenHeight, "HISKET [PROTOTYPE]");

    struct nk_context *ctx = InitNuklear(20);

    

    SetTargetFPS(60);

    Player player = CreatePlayer();

    //Draw walls
    Vector3 wallSize = (Vector3){ 16.0f, 16.0f,  16.0f };
    Mesh wallMesh = GenMeshCube(wallSize.x, wallSize.y, wallSize.z);
    Model wallModel = LoadModelFromMesh(wallMesh);
    Texture2D wallTexture = LoadTexture("resources/textures/damaged_plaster_diff_1k.jpg");
    wallModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = wallTexture;
    Vector3 wallPos  = (Vector3){ 0.0f, 8.0f, -32.0f };

    FPSWeapon weapon = CreateFPSWeapon(
        "resources/models/mp5.obj", 
        "resources/textures/weapons/mp5.png"
    );

    //Input
	bool isMouseWasPressed = false;
	Ray shootRay;
	RayCollision shootCollision;

    Vector2 screenCenter = (Vector2) {
				screenWidth / 2,
				screenHeight/ 2
	};
    
    //UI texture
    RenderTexture2D weaponRT = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    //Debug
    bool isFlyCam = false;

    bool isCursorDisabled = true;
    DisableCursor();


    while (!WindowShouldClose())
    {
		// Update state
        //----------------------------------------------------------------------------------

        if(IsKeyReleased(KEY_K))
        {
            isCursorDisabled = !isCursorDisabled;

            if(isCursorDisabled)
            {
                DisableCursor();
            }
            else 
            {
                EnableCursor();
            }
        }

        if(IsMouseButtonPressed(MOUSE_RIGHT_BUTTON))
        {
            weapon.isAiming = true;
        }

        if(IsMouseButtonReleased(MOUSE_RIGHT_BUTTON))
        {
            weapon.isAiming = false;
        }

        

        if(isCursorDisabled) 
        {
        
            if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
            {
                isMouseWasPressed = !isMouseWasPressed;
                shootRay = GetScreenToWorldRay(screenCenter, player.worldCamera);

                shootCollision = GetRayCollisionBox(shootRay, (BoundingBox){
                    (Vector3){ -wallSize.x/2 + wallPos.x, -wallSize.y/2 + wallPos.y, -wallSize.z/2 + wallPos.z }, 
                    (Vector3){  wallSize.x/2 + wallPos.x,  wallSize.y/2 + wallPos.y,  wallSize.z/2 + wallPos.z } 
                });

                if(shootCollision.hit)
                {
                    TraceLog(LOG_INFO, "Hitted that box!");
                }

                weapon.isShooting = true;
            }

            


            if(IsKeyPressed(KEY_T))
            {
                isFlyCam = !isFlyCam;
            }

            if(!isFlyCam)
            {
                UpdatePlayer(&player.worldCamera, &player);
            }
            else 
            {
                UpdateCamera(&player.worldCamera, CAMERA_FREE);
            }

            UpdateWeapon(&weapon, player.worldCamera, player.velocity);

        }
        else
        {
            DrawWeaponDebugWindow(ctx, &weapon);
        }

        UpdateNuklear(ctx);

        
        // UI
        //----------------------------------------------------------------------------------
        BeginTextureMode(weaponRT);
            ClearBackground(BLANK);

            BeginMode3D(player.hudCamera);
                DrawWeapon(weapon);
            EndMode3D();
        EndTextureMode();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GRAY);

            BeginMode3D(player.worldCamera);
                DrawLevel();

				DrawCubeWires(wallPos, wallSize.x, wallSize.y, wallSize.z, MAROON);

				if(shootCollision.hit)
				{
					DrawSphere(shootCollision.point, 0.1f, BLUE);
					DrawLine3D(shootCollision.point, Vector3Add(shootCollision.point, Vector3Scale(shootCollision.normal, 0.5f)), GREEN);
				}
                
                DrawModel(wallModel, wallPos, 1.0f, WHITE);
            EndMode3D();

            int weaponX = GetScreenWidth() / 2 + 100;
            int weaponY = GetScreenHeight() / 2 - 200;

            DrawTextureRec(weaponRT.texture,
                (Rectangle){0, 0, (float)weaponRT.texture.width, -(float)weaponRT.texture.height},
                (Vector2){0, 0},
                WHITE
            );

            DrawRectangle(5, 5, 330, 90, Fade(SKYBLUE, 0.5f));
            DrawRectangleLines(5, 5, 330, 90, BLUE);

            DrawText("Camera controls:", 15, 15, 10, BLACK);
            DrawText("- Move keys: W, A, S, D, Space, Left-Ctrl", 15, 30, 10, BLACK);
            DrawText("- Look around: arrow keys or mouse", 15, 45, 10, BLACK);
            DrawText(TextFormat("- Velocity Len: (%06.3f)", Vector2Length((Vector2){ player.velocity.x, player.velocity.z })), 15, 60, 10, BLACK);
			DrawText(TextFormat("- FPS: (%i)", GetFPS()), 15, 75, 10, BLACK);


			DrawText(TextFormat("- IsMouseWasPressed: (%d)", isMouseWasPressed), 15, 110, 10, BLACK);
			DrawText(TextFormat("- IsAiming: (%d)", weapon.isAiming), 15, 135, 10, BLACK);



            DrawNuklear(ctx);
			

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();

    UnloadTexture(wallTexture);
    UnloadModel(wallModel);

    DestroyWeapon(weapon);

    UnloadNuklear(ctx);


    return 0;
}


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