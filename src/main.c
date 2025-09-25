
#include "player.h"
#include "level.h"

#include "rlgl.h"



int main(void)
{
    const int screenWidth = 1270;
    const int screenHeight = 920;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera fps");

    // Initialize camera variables
    // NOTE: UpdateCameraFPS() takes care of the rest
    Camera camera = { 0 };
    camera.fovy = 60.0f;
    camera.projection = CAMERA_PERSPECTIVE;
    camera.position = (Vector3){
        playerBody.position.x,
        playerBody.position.y + (BOTTOM_HEIGHT + headLerp),
        playerBody.position.z,
    };

    UpdateCameraFPS(&camera);

    DisableCursor();

    SetTargetFPS(60);

    Vector3 wallSize = (Vector3){ 16.0f, 16.0f,  16.0f };
    Mesh wallMesh = GenMeshCube(wallSize.x, wallSize.y, wallSize.z);
    Model wallModel = LoadModelFromMesh(wallMesh);
    Texture2D wallTexture = LoadTexture("resources/textures/damaged_plaster_diff_1k.jpg");
    wallModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = wallTexture;

    Vector3 wallPos  = (Vector3){  0.0f,  8.0f, -32.0f };


    Model weaponModel = LoadModel("resources/models/mp5.obj");
    Texture2D weaponTexture = LoadTexture("resources/textures/weapons/mp5.png");
    weaponModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = weaponTexture;

	bool isMouseWasPressed = false;
	Ray shootRay;
	RayCollision shootCollision;


    bool isFlyCam = false;
    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		// Update state
        //----------------------------------------------------------------------------------
        
        

		if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			isMouseWasPressed = !isMouseWasPressed;
			TraceLog(LOG_INFO, "Mouse was pressed, yeah");
			Vector2 screenCenter = (Vector2) {
				screenWidth / 2,
				screenHeight/ 2
			};
			shootRay = GetScreenToWorldRay(screenCenter, camera);

			shootCollision = GetRayCollisionBox(shootRay, (BoundingBox){
                (Vector3){ -wallSize.x/2 + wallPos.x, -wallSize.y/2 + wallPos.y, -wallSize.z/2 + wallPos.z }, 
                (Vector3){  wallSize.x/2 + wallPos.x,  wallSize.y/2 + wallPos.y,  wallSize.z/2 + wallPos.z } 
            });

			if(shootCollision.hit)
			{
				TraceLog(LOG_INFO, "Hitted that box!");
			}
		}

        if(IsKeyPressed(KEY_T))
        {
            isFlyCam = !isFlyCam;
        }

        if(!isFlyCam)
        {
		    UpdatePlayer(&camera, &playerBody);
        }
        else 
        {
            UpdateCamera(&camera, CAMERA_FREE);
        }

		

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GRAY);

            BeginMode3D(camera);
                DrawLevel();

				// DrawCube(towerPos, towerSize.x, towerSize.y, towerSize.z, GRAY);
				DrawCubeWires(wallPos, wallSize.x, wallSize.y, wallSize.z, MAROON);

				if(shootCollision.hit)
				{
					DrawSphere(shootCollision.point, 0.1f, BLUE);
					DrawLine3D(shootCollision.point, Vector3Add(shootCollision.point, Vector3Scale(shootCollision.normal, 0.5f)), GREEN);
				}

                DrawModel(weaponModel, (Vector3){0.0f, 5.0f, 0.0f}, 1.0f, WHITE);
                DrawModel(wallModel, wallPos, 1.0f, WHITE);


            EndMode3D();

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
    UnloadModel(weaponModel);
    UnloadModel(wallModel);


    return 0;
}