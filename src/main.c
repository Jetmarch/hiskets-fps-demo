
#include "player.h"
#include "level.h"


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

	Vector3 towerSize = (Vector3){ 16.0f, 32.0f, 16.0f };
    Vector3 towerPos = (Vector3){ 0.0f, 0.0f, 0.0f };

	bool isMouseWasPressed = false;
	Ray shootRay;
	RayCollision shootCollision;
    //--------------------------------------------------------------------------------------

    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
		// Update state
        //----------------------------------------------------------------------------------
		UpdatePlayer(&camera, &playerBody);

		if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
		{
			isMouseWasPressed = !isMouseWasPressed;
			TraceLog(LOG_INFO, "Mouse was pressed, yeah");
			Vector2 screenCenter = (Vector2) {
				screenWidth / 2,
				screenHeight/ 2
			};
			shootRay = GetScreenToWorldRay(screenCenter, camera);

			shootCollision = GetRayCollisionBox(shootRay, (BoundingBox){ (Vector3){ -towerSize.x/2, -towerSize.y/2, -towerSize.z/2 }, (Vector3){ towerSize.x/2, towerSize.y/2, towerSize.z/2 } });

			if(shootCollision.hit)
			{
				TraceLog(LOG_INFO, "Hitted that box!");
			}
		}

		

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            BeginMode3D(camera);
                DrawLevel();

				DrawCube(towerPos, towerSize.x, towerSize.y, towerSize.z, GRAY);
				DrawCubeWires(towerPos, towerSize.x, towerSize.y, towerSize.z, MAROON);

				if(shootCollision.hit)
				{
					DrawSphere(shootCollision.point, 0.1f, BLUE);
					DrawLine3D(shootCollision.point, Vector3Add(shootCollision.point, Vector3Scale(shootCollision.normal, 0.5f)), GREEN);
				}
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

    return 0;
}