
#include "player.h"
#include "level.h"

typedef struct {
    Model model;
    Vector3 baseOffset;
    Vector3 currentOffset;
    float recoilTimer;
    bool isShooting;
} FPSWeapon;

FPSWeapon CreateFPSWeapon(Model model, Vector3 offset)
{
    FPSWeapon weapon;
    weapon.model = model;
    weapon.baseOffset = offset;
    weapon.currentOffset = offset;
    weapon.recoilTimer = 0.0f;
    weapon.isShooting = false;
    return weapon;
}

void UpdateWeapon(FPSWeapon *weapon, Camera camera)
{
    if (weapon->isShooting)
    {
        weapon->recoilTimer += GetFrameTime();
        
        float recoilProgress = weapon->recoilTimer / 0.1f;
        if (recoilProgress < 1.0f)
        {
            weapon->currentOffset.z = weapon->baseOffset.z - 0.1f * sinf(recoilProgress * PI);
        }
        else
        {
            weapon->currentOffset.z = weapon->baseOffset.z;
            weapon->isShooting = false;
            weapon->recoilTimer = 0.0f;
        }
    }
}

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

void DrawFPSWeapon(FPSWeapon weapon, Camera camera)
{
    Vector3 weaponPos = Vector3Add(camera.position, weapon.currentOffset);

    // Vector3 cameraAngles = GetCameraEulerAngles(camera);
    // weapon.model.transform = MatrixRotateXYZ((Vector3){ DEG2RAD*cameraAngles.x, DEG2RAD*cameraAngles.y, DEG2RAD*cameraAngles.z });


    DrawModelEx(weapon.model, weaponPos, camera.target, 0.0f, (Vector3) {0.01f, 0.01f, 0.01f}, GRAY);
}

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

    screenCamera.position = (Vector3){ 0.0f, 0.0f, 3.0f }; // чуть дальше
    screenCamera.target   = (Vector3){ 0.0f, 0.0f, 0.0f };
    screenCamera.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
    screenCamera.fovy = 60.0f;

    DisableCursor();

    SetTargetFPS(60);

    Vector3 wallSize = (Vector3){ 16.0f, 16.0f,  16.0f };
    Mesh wallMesh = GenMeshCube(wallSize.x, wallSize.y, wallSize.z);
    Model wallModel = LoadModelFromMesh(wallMesh);
    Texture2D wallTexture = LoadTexture("resources/textures/damaged_plaster_diff_1k.jpg");
    wallModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = wallTexture;

    Vector3 wallPos  = (Vector3){ 0.0f, 8.0f, -32.0f };


    Model weaponModel = LoadModel("resources/models/mp5.obj");
    Texture2D weaponTexture = LoadTexture("resources/textures/weapons/mp5.png");
    weaponModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = weaponTexture;
    Vector3 weaponPositionOffset = (Vector3){0.2f, -0.5f, -0.5f};
    Quaternion weaponRotation;
    Vector3 weaponForwardVector = (Vector3){0.0f, 0.0f, 1.0f};

    FPSWeapon weapon = CreateFPSWeapon(weaponModel, weaponPositionOffset);

	bool isMouseWasPressed = false;
	Ray shootRay;
	RayCollision shootCollision;

    Vector2 screenCenter = (Vector2) {
				screenWidth / 2,
				screenHeight/ 2
	};
    
    RenderTexture2D weaponRT = LoadRenderTexture(GetScreenWidth(), GetScreenHeight());

    bool isFlyCam = false;

    float weaponSwayTime = 0.0f;
    float weaponSwaySpeed = 7.0f;
    float maxWeaponSwayAmount = 1.0f;
    float currentWeaponSwayAmount = 0.0f;
    float weaponSwayVelocityTreshhold = 0.7f;

    Vector3 weaponRotationAxis = {0.0f, 1.0f, 0.0f};
    float weaponRotationAngle = 180.0f;
    Vector3 weaponSize = {0.02f, 0.02f, 0.02f};

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

        UpdateWeapon(&weapon, worldCamera);

        bool isMoving = Vector3Length(playerBody.velocity) > weaponSwayVelocityTreshhold;

        if(isMoving)
        {
            currentWeaponSwayAmount = Clamp(currentWeaponSwayAmount + GetFrameTime() * weaponSwaySpeed, 0.0f, maxWeaponSwayAmount);
        }
        else 
        {
            currentWeaponSwayAmount = Clamp(currentWeaponSwayAmount - GetFrameTime() * weaponSwaySpeed, 0.0f, maxWeaponSwayAmount);
        }

        weaponSwayTime += GetFrameTime() * weaponSwaySpeed;

        float swayX = sinf(weaponSwayTime) * 0.1f * currentWeaponSwayAmount;
        float swayY = cosf(weaponSwayTime * 2.0f) * 0.05f * currentWeaponSwayAmount;

        Vector3 weaponSway = (Vector3) {swayX, swayY, 0.0f};
        

        // UI
        //----------------------------------------------------------------------------------
        BeginTextureMode(weaponRT);
            ClearBackground(BLANK);

            BeginMode3D(screenCamera);
                Vector3 weaponPosition = Vector3Add(weaponSway, weaponPositionOffset);


                DrawModelEx(weapon.model, weaponPosition, weaponRotationAxis, weaponRotationAngle, weaponSize, WHITE);
            EndMode3D();
        EndTextureMode();

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(GRAY);

            BeginMode3D(worldCamera);
                DrawLevel();

				// DrawCube(towerPos, towerSize.x, towerSize.y, towerSize.z, GRAY);
				DrawCubeWires(wallPos, wallSize.x, wallSize.y, wallSize.z, MAROON);

				if(shootCollision.hit)
				{
					DrawSphere(shootCollision.point, 0.1f, BLUE);
					DrawLine3D(shootCollision.point, Vector3Add(shootCollision.point, Vector3Scale(shootCollision.normal, 0.5f)), GREEN);
				}
                

                // DrawModel(weaponModel, weaponPosition, 0.01f, WHITE);
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
    UnloadModel(weaponModel);
    UnloadModel(wallModel);


    return 0;
}