#ifndef PLAYER_C
#define PLAYER_C
#include "player.h"

Player CreatePlayer()
{
    Player player = { 0 };
    

    Camera worldCamera = { 0 };
    worldCamera.fovy = 60.0f;
    worldCamera.projection = CAMERA_PERSPECTIVE;
    worldCamera.position = (Vector3){
        player.position.x,
        player.position.y + (BOTTOM_HEIGHT + headLerp),
        player.position.z,
    };

    Camera hudCamera = { 0 };
    hudCamera.position = (Vector3){ 0.0f, 0.0f, 3.0f };
    hudCamera.target   = (Vector3){ 0.0f, 0.0f, 0.0f };
    hudCamera.up       = (Vector3){ 0.0f, 1.0f, 0.0f };
    hudCamera.fovy = 60.0f;

    player.worldCamera = worldCamera;
    player.hudCamera = hudCamera;
    
    return player;
}

void UpdatePlayer(Camera *camera, Player *body)
{
	// Update
	//----------------------------------------------------------------------------------
	Vector2 mouseDelta = GetMouseDelta();
	lookRotation.x -= mouseDelta.x*sensitivity.x;
	lookRotation.y += mouseDelta.y*sensitivity.y;

	char sideway = (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
	char forward = (IsKeyDown(KEY_W) - IsKeyDown(KEY_S));
	bool crouching = IsKeyDown(KEY_LEFT_CONTROL);
	UpdateBody(body, lookRotation.x, sideway, forward, IsKeyPressed(KEY_SPACE), crouching);

	float delta = GetFrameTime();
	headLerp = Lerp(headLerp, (crouching ? CROUCH_HEIGHT : STAND_HEIGHT), 20.0f*delta);
	camera->position = (Vector3){
		body->position.x,
		body->position.y + (BOTTOM_HEIGHT + headLerp),
		body->position.z,
	};

	if (body->isGrounded && ((forward != 0) || (sideway != 0)))
	{
		headTimer += delta*3.0f;
		walkLerp = Lerp(walkLerp, 1.0f, 10.0f*delta);
		camera->fovy = Lerp(camera->fovy, 55.0f, 5.0f*delta);
	}
	else
	{
		walkLerp = Lerp(walkLerp, 0.0f, 10.0f*delta);
		camera->fovy = Lerp(camera->fovy, 60.0f, 5.0f*delta);
	}

	lean.x = Lerp(lean.x, sideway*0.02f, 10.0f*delta);
	lean.y = Lerp(lean.y, forward*0.015f, 10.0f*delta);

	UpdateCameraFPS(camera);
        //----------------------------------------------------------------------------------
}

// Update camera for FPS behaviour
void UpdateCameraFPS(Camera *camera)
{
    const Vector3 up = (Vector3){ 0.0f, 1.0f, 0.0f };
    const Vector3 targetOffset = (Vector3){ 0.0f, 0.0f, -1.0f };

    // Left and right
    Vector3 yaw = Vector3RotateByAxisAngle(targetOffset, up, lookRotation.x);

    // Clamp view up
    float maxAngleUp = Vector3Angle(up, yaw);
    maxAngleUp -= 0.001f; // Avoid numerical errors
    if ( -(lookRotation.y) > maxAngleUp) { lookRotation.y = -maxAngleUp; }

    // Clamp view down
    float maxAngleDown = Vector3Angle(Vector3Negate(up), yaw);
    maxAngleDown *= -1.0f; // Downwards angle is negative
    maxAngleDown += 0.001f; // Avoid numerical errors
    if ( -(lookRotation.y) < maxAngleDown) { lookRotation.y = -maxAngleDown; }

    // Up and down
    Vector3 right = Vector3Normalize(Vector3CrossProduct(yaw, up));

    // Rotate view vector around right axis
    float pitchAngle = -lookRotation.y - lean.y;
    pitchAngle = Clamp(pitchAngle, -PI/2 + 0.0001f, PI/2 - 0.0001f); // Clamp angle so it doesn't go past straight up or straight down
    Vector3 pitch = Vector3RotateByAxisAngle(yaw, right, pitchAngle);

    // Head animation
    // Rotate up direction around forward axis
    float headSin = sin(headTimer*PI);
    float headCos = cos(headTimer*PI);
    const float stepRotation = 0.01f;
    camera->up = Vector3RotateByAxisAngle(up, pitch, headSin*stepRotation + lean.x);

    // Camera BOB
    const float bobSide = 0.1f;
    const float bobUp = 0.15f;
    Vector3 bobbing = Vector3Scale(right, headSin*bobSide);
    bobbing.y = fabsf(headCos*bobUp);

    camera->position = Vector3Add(camera->position, Vector3Scale(bobbing, walkLerp));
    camera->target = Vector3Add(camera->position, pitch);
}



//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------
// Update body considering current world state
void UpdateBody(Player *body, float rot, char side, char forward, bool jumpPressed, bool crouchHold)
{
    Vector2 input = (Vector2){ (float)side, (float)-forward };

#if defined(NORMALIZE_INPUT)
    // Slow down diagonal movement
    if ((side != 0) && (forward != 0)) input = Vector2Normalize(input);
#endif

    float delta = GetFrameTime();

    if (!body->isGrounded) body->velocity.y -= GRAVITY*delta;

    if (body->isGrounded && jumpPressed)
    {
        body->velocity.y = JUMP_FORCE;
        body->isGrounded = false;

        // Sound can be played at this moment
        //SetSoundPitch(fxJump, 1.0f + (GetRandomValue(-100, 100)*0.001));
        //PlaySound(fxJump);
    }

    Vector3 front = (Vector3){ sin(rot), 0.f, cos(rot) };
    Vector3 right = (Vector3){ cos(-rot), 0.f, sin(-rot) };

    Vector3 desiredDir = (Vector3){ input.x*right.x + input.y*front.x, 0.0f, input.x*right.z + input.y*front.z, };
    body->dir = Vector3Lerp(body->dir, desiredDir, CONTROL*delta);

    float decel = (body->isGrounded ? FRICTION : AIR_DRAG);
    Vector3 hvel = (Vector3){ body->velocity.x*decel, 0.0f, body->velocity.z*decel };

    float hvelLength = Vector3Length(hvel); // Magnitude
    if (hvelLength < (MAX_SPEED*0.01f)) hvel = (Vector3){ 0 };

    // This is what creates strafing
    float speed = Vector3DotProduct(hvel, body->dir);

    // Whenever the amount of acceleration to add is clamped by the maximum acceleration constant,
    // a Player can make the speed faster by bringing the direction closer to horizontal velocity angle
    // More info here: https://youtu.be/v3zT3Z5apaM?t=165
    float maxSpeed = (crouchHold? CROUCH_SPEED : MAX_SPEED);
    float accel = Clamp(maxSpeed - speed, 0.f, MAX_ACCEL*delta);
    hvel.x += body->dir.x*accel;
    hvel.z += body->dir.z*accel;

    body->velocity.x = hvel.x;
    body->velocity.z = hvel.z;

    body->position.x += body->velocity.x*delta;
    body->position.y += body->velocity.y*delta;
    body->position.z += body->velocity.z*delta;

    // Fancy collision system against the floor
    if (body->position.y <= 0.0f)
    {
        body->position.y = 0.0f;
        body->velocity.y = 0.0f;
        body->isGrounded = true; // Enable jumping
    }
}

#endif //PLAYER_C