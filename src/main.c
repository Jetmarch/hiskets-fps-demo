
#include "player.h"
#include "level.h"
#include "weapon.h"


int main(void)
{
    const int screenWidth = 1270;
    const int screenHeight = 920;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera fps");

    Camera worldCamera = { 0 };
    worldCamera.fovy = 60.0f;
    worldCamera.projection = CAMERA_PERSPECTIVE;
    worldCamera.position = (Vector3){
        playerBody.position.x,
        playerBody.position.y + (BOTTOM_HEIGHT + headLerp),
        playerBody.position.z,
    };

    Camera screenCamera = { 0 };
    screenCamera.position = (Vector3){ 0.0f, 0.0f, 3.0f };
    screenCamera.target   = (Vector3){ 0.0f, 0.0f, 0.0f };
    screenCamera.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
    screenCamera.fovy = 60.0f;

    DisableCursor();

    SetTargetFPS(60);

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

    while (!WindowShouldClose())
    {
		// Update state
        //----------------------------------------------------------------------------------
        
		if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			isMouseWasPressed = !isMouseWasPressed;
			shootRay = GetScreenToWorldRay(screenCenter, worldCamera);

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
		    UpdatePlayer(&worldCamera, &playerBody);
        }
        else 
        {
            UpdateCamera(&worldCamera, CAMERA_FREE);
        }

        UpdateWeapon(&weapon, worldCamera, playerBody.velocity);
        
        // UI
        //----------------------------------------------------------------------------------
        BeginTextureMode(weaponRT);
            ClearBackground(BLANK);

            BeginMode3D(screenCamera);
                DrawWeapon(weapon);
            EndMode3D();
        EndTextureMode();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GRAY);

            BeginMode3D(worldCamera);
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
            DrawText(TextFormat("- Velocity Len: (%06.3f)", Vector2Length((Vector2){ playerBody.velocity.x, playerBody.velocity.z })), 15, 60, 10, BLACK);
			DrawText(TextFormat("- FPS: (%i)", GetFPS()), 15, 75, 10, BLACK);


			DrawText(TextFormat("- IsMouseWasPressed: (%d)", isMouseWasPressed), 15, 110, 10, BLACK);

			

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    CloseWindow();

    UnloadTexture(wallTexture);
    UnloadModel(wallModel);

    DestroyWeapon(weapon);


    return 0;
}