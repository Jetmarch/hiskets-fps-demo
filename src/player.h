#include "raylib.h"
#include "raymath.h"

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Movement constants
#define GRAVITY         32.0f
#define MAX_SPEED       20.0f
#define CROUCH_SPEED     5.0f
#define JUMP_FORCE      12.0f
#define MAX_ACCEL      150.0f
// Grounded drag
#define FRICTION         0.86f
// Increasing air drag, increases strafing speed
#define AIR_DRAG         0.98f
// Responsiveness for turning movement direction to looked direction
#define CONTROL         15.0f
#define CROUCH_HEIGHT    0.0f
#define STAND_HEIGHT     1.0f
#define BOTTOM_HEIGHT    0.5f

#define NORMALIZE_INPUT  0

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// Body structure
typedef struct {
    Vector3 position;
    Vector3 velocity;
    Vector3 dir;
    bool isGrounded;
} Body;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static Vector2 sensitivity = { 0.001f, 0.001f };

static Body playerBody = { 0 };
static Vector2 lookRotation = { 0 };
static float headTimer = 0.0f;
static float walkLerp = 0.0f;
static float headLerp = STAND_HEIGHT;
static Vector2 lean = { 0 };

//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
void UpdatePlayer(Camera *camera, Body *body);
void UpdateCameraFPS(Camera *camera);
void UpdateBody(Body *body, float rot, char side, char forward, bool jumpPressed, bool crouchHold);