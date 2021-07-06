/*******************************************************************************************
*
*   raylib.camera - Camera system with multiple modes support
*
*   NOTE: Memory footprint of this library is aproximately 52 bytes (global variables)
*
*   CONFIGURATION:
*
*   #define CAMERA_IMPLEMENTATION
*       Generates the implementation of the library into the included file.
*       If not defined, the library is in header only mode and can be included in other headers
*       or source files without problems. But only ONE file should hold the implementation.
**********************************************************************************************/

/***********************************************************************************
*
*   CAMERA IMPLEMENTATION
*
************************************************************************************/

#if defined(CAMERA_IMPLEMENTATION)

#include <math.h>               // Required for: sinf(), cosf(), sqrtf()

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
#ifndef PI
    #define PI 3.14159265358979323846
#endif
#ifndef DEG2RAD
    #define DEG2RAD (PI/180.0f)
#endif
#ifndef RAD2DEG
    #define RAD2DEG (180.0f/PI)
#endif

// Camera mouse movement sensitivity
#define CAMERA_MOUSE_MOVE_SENSITIVITY                   0.003f
#define CAMERA_MOUSE_SCROLL_SENSITIVITY                 1.5f

// FREE_CAMERA
#define CAMERA_FREE_MOUSE_SENSITIVITY                   0.01f
#define CAMERA_FREE_DISTANCE_MIN_CLAMP                  0.3f
#define CAMERA_FREE_DISTANCE_MAX_CLAMP                  120.0f
#define CAMERA_FREE_MIN_CLAMP                           85.0f
#define CAMERA_FREE_MAX_CLAMP                          -85.0f
#define CAMERA_FREE_SMOOTH_ZOOM_SENSITIVITY             0.05f
#define CAMERA_FREE_PANNING_DIVIDER                     5.1f

// ORBITAL_CAMERA
#define CAMERA_ORBITAL_SPEED                            0.01f       // Radians per frame

// FIRST_PERSON
//#define CAMERA_FIRST_PERSON_MOUSE_SENSITIVITY           0.003f
#define CAMERA_FIRST_PERSON_FOCUS_DISTANCE              25.0f
#define CAMERA_FIRST_PERSON_MIN_CLAMP                   89.0f
#define CAMERA_FIRST_PERSON_MAX_CLAMP                  -89.0f

#define CAMERA_FIRST_PERSON_STEP_TRIGONOMETRIC_DIVIDER  8.0f
#define CAMERA_FIRST_PERSON_STEP_DIVIDER                30.0f
#define CAMERA_FIRST_PERSON_WAVING_DIVIDER              200.0f

// THIRD_PERSON
//#define CAMERA_THIRD_PERSON_MOUSE_SENSITIVITY           0.003f
#define CAMERA_THIRD_PERSON_DISTANCE_CLAMP              1.2f
#define CAMERA_THIRD_PERSON_MIN_CLAMP                   5.0f
#define CAMERA_THIRD_PERSON_MAX_CLAMP                  -85.0f
#define CAMERA_THIRD_PERSON_OFFSET                      (Vector3){ 0.4f, 0.0f, 0.0f }

// PLAYER (used by camera)
#define PLAYER_MOVEMENT_SENSITIVITY                     20.0f

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------
// Camera move modes (first person and third person cameras)
typedef enum {
    MOVE_FRONT = 0,
    MOVE_BACK,
    MOVE_RIGHT,
    MOVE_LEFT,
    MOVE_UP,
    MOVE_DOWN
} CameraMove;

// Camera global state context data [56 bytes]
typedef struct {
    unsigned int mode;              // Current camera mode
    float targetDistance;           // Camera distance from position to target
    float playerEyesPosition;       // Player eyes position from ground (in meters)
    Vector2 angle;                  // Camera angle in plane XZ

    // Camera movement control keys
    int moveControl[6];             // Move controls (CAMERA_FIRST_PERSON)
    int smoothZoomControl;          // Smooth zoom control key
    int altControl;                 // Alternative control key
    int panControl;                 // Pan view control key
} CameraData;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static CameraData CAMERA = {        // Global CAMERA state context
    .mode = 0,
    .targetDistance = 0,
    .playerEyesPosition = 1.85f,
    .angle = { 0 },
    .moveControl = { 'W', 'S', 'D', 'A', 'E', 'Q' },
    .smoothZoomControl = 341,       // raylib: KEY_LEFT_CONTROL
    .altControl = 342,              // raylib: KEY_LEFT_ALT
    .panControl = 2                 // raylib: MOUSE_BUTTON_MIDDLE
};

//----------------------------------------------------------------------------------
// Module Functions Definition
//----------------------------------------------------------------------------------

// Select camera mode (multiple camera modes available)
void SetCameraMode(Camera camera, int mode)
{
    Vector3 v1 = camera.position;
    Vector3 v2 = camera.target;

    float dx = v2.x - v1.x;
    float dy = v2.y - v1.y;
    float dz = v2.z - v1.z;

    CAMERA.targetDistance = sqrtf(dx*dx + dy*dy + dz*dz);   // Distance to target

    // Camera angle calculation
    CAMERA.angle.x = atan2f(dx, dz);                        // Camera angle in plane XZ (0 aligned with Z, move positive CCW)
    CAMERA.angle.y = atan2f(dy, sqrtf(dx*dx + dz*dz));      // Camera angle in plane XY (0 aligned with X, move positive CW)

    CAMERA.playerEyesPosition = camera.position.y;          // Init player eyes position to camera Y position

    // Lock cursor for first person and third person cameras
    if ((mode == CAMERA_FIRST_PERSON) || (mode == CAMERA_THIRD_PERSON)) DisableCursor();
    else EnableCursor();

    CAMERA.mode = mode;
}

// Update camera depending on selected mode
// NOTE: Camera controls depend on some raylib functions:
//       System: EnableCursor(), DisableCursor()
//       Mouse: IsMouseButtonDown(), GetMousePosition(), GetMouseWheelMove()
//       Keys:  IsKeyDown()
// TODO: Port to quaternion-based camera (?)
void UpdateCamera(Camera *camera)
{
    static int swingCounter = 0;    // Used for 1st person swinging movement
    static Vector2 previousMousePosition = { 0.0f, 0.0f };

    // TODO: Compute CAMERA.targetDistance and CAMERA.angle here (?)

    // Mouse movement detection
    Vector2 mousePositionDelta = { 0.0f, 0.0f };
    Vector2 mousePosition = GetMousePosition();
    float mouseWheelMove = GetMouseWheelMove();

    // Keys input detection
    // TODO: Input detection is raylib-dependant, it could be moved outside the module
    bool keyPan = IsMouseButtonDown(CAMERA.panControl);
    bool keyAlt = IsKeyDown(CAMERA.altControl);
    bool szoomKey = IsKeyDown(CAMERA.smoothZoomControl);
    bool direction[6] = { IsKeyDown(CAMERA.moveControl[MOVE_FRONT]),
                          IsKeyDown(CAMERA.moveControl[MOVE_BACK]),
                          IsKeyDown(CAMERA.moveControl[MOVE_RIGHT]),
                          IsKeyDown(CAMERA.moveControl[MOVE_LEFT]),
                          IsKeyDown(CAMERA.moveControl[MOVE_UP]),
                          IsKeyDown(CAMERA.moveControl[MOVE_DOWN]) };

    if (CAMERA.mode != CAMERA_CUSTOM)
    {
        mousePositionDelta.x = mousePosition.x - previousMousePosition.x;
        mousePositionDelta.y = mousePosition.y - previousMousePosition.y;

        previousMousePosition = mousePosition;
    }

    // Support for multiple automatic camera modes
    // NOTE: In case of CAMERA_CUSTOM nothing happens here, user must update it manually
    switch (CAMERA.mode)
    {
        case CAMERA_FREE:           // Camera free controls, using standard 3d-content-creation scheme
        {
            // Camera zoom
            if ((CAMERA.targetDistance < CAMERA_FREE_DISTANCE_MAX_CLAMP) && (mouseWheelMove < 0))
            {
                CAMERA.targetDistance -= (mouseWheelMove*CAMERA_MOUSE_SCROLL_SENSITIVITY);
                if (CAMERA.targetDistance > CAMERA_FREE_DISTANCE_MAX_CLAMP) CAMERA.targetDistance = CAMERA_FREE_DISTANCE_MAX_CLAMP;
            }

            // Camera looking down
            else if ((camera->position.y > camera->target.y) && (CAMERA.targetDistance == CAMERA_FREE_DISTANCE_MAX_CLAMP) && (mouseWheelMove < 0))
            {
                camera->target.x += mouseWheelMove*(camera->target.x - camera->position.x)*CAMERA_MOUSE_SCROLL_SENSITIVITY/CAMERA.targetDistance;
                camera->target.y += mouseWheelMove*(camera->target.y - camera->position.y)*CAMERA_MOUSE_SCROLL_SENSITIVITY/CAMERA.targetDistance;
                camera->target.z += mouseWheelMove*(camera->target.z - camera->position.z)*CAMERA_MOUSE_SCROLL_SENSITIVITY/CAMERA.targetDistance;
            }
            else if ((camera->position.y > camera->target.y) && (camera->target.y >= 0))
            {
                camera->target.x += mouseWheelMove*(camera->target.x - camera->position.x)*CAMERA_MOUSE_SCROLL_SENSITIVITY/CAMERA.targetDistance;
                camera->target.y += mouseWheelMove*(camera->target.y - camera->position.y)*CAMERA_MOUSE_SCROLL_SENSITIVITY/CAMERA.targetDistance;
                camera->target.z += mouseWheelMove*(camera->target.z - camera->position.z)*CAMERA_MOUSE_SCROLL_SENSITIVITY/CAMERA.targetDistance;

                // if (camera->target.y < 0) camera->target.y = -0.001;
            }
            else if ((camera->position.y > camera->target.y) && (camera->target.y < 0) && (mouseWheelMove > 0))
            {
                CAMERA.targetDistance -= (mouseWheelMove*CAMERA_MOUSE_SCROLL_SENSITIVITY);
                if (CAMERA.targetDistance < CAMERA_FREE_DISTANCE_MIN_CLAMP) CAMERA.targetDistance = CAMERA_FREE_DISTANCE_MIN_CLAMP;
            }
            // Camera looking up
            else if ((camera->position.y < camera->target.y) && (CAMERA.targetDistance == CAMERA_FREE_DISTANCE_MAX_CLAMP) && (mouseWheelMove < 0))
            {
                camera->target.x += mouseWheelMove*(camera->target.x - camera->position.x)*CAMERA_MOUSE_SCROLL_SENSITIVITY/CAMERA.targetDistance;
                camera->target.y += mouseWheelMove*(camera->target.y - camera->position.y)*CAMERA_MOUSE_SCROLL_SENSITIVITY/CAMERA.targetDistance;
                camera->target.z += mouseWheelMove*(camera->target.z - camera->position.z)*CAMERA_MOUSE_SCROLL_SENSITIVITY/CAMERA.targetDistance;
            }
            else if ((camera->position.y < camera->target.y) && (camera->target.y <= 0))
            {
                camera->target.x += mouseWheelMove*(camera->target.x - camera->position.x)*CAMERA_MOUSE_SCROLL_SENSITIVITY/CAMERA.targetDistance;
                camera->target.y += mouseWheelMove*(camera->target.y - camera->position.y)*CAMERA_MOUSE_SCROLL_SENSITIVITY/CAMERA.targetDistance;
                camera->target.z += mouseWheelMove*(camera->target.z - camera->position.z)*CAMERA_MOUSE_SCROLL_SENSITIVITY/CAMERA.targetDistance;

                // if (camera->target.y > 0) camera->target.y = 0.001;
            }
            else if ((camera->position.y < camera->target.y) && (camera->target.y > 0) && (mouseWheelMove > 0))
            {
                CAMERA.targetDistance -= (mouseWheelMove*CAMERA_MOUSE_SCROLL_SENSITIVITY);
                if (CAMERA.targetDistance < CAMERA_FREE_DISTANCE_MIN_CLAMP) CAMERA.targetDistance = CAMERA_FREE_DISTANCE_MIN_CLAMP;
            }

            // Input keys checks
            if (keyPan)
            {
                if (keyAlt)     // Alternative key behaviour
                {
                    if (szoomKey)
                    {
                        // Camera smooth zoom
                        CAMERA.targetDistance += (mousePositionDelta.y*CAMERA_FREE_SMOOTH_ZOOM_SENSITIVITY);
                    }
                    else
                    {
                        // Camera rotation
                        CAMERA.angle.x += mousePositionDelta.x*-CAMERA_FREE_MOUSE_SENSITIVITY;
                        CAMERA.angle.y += mousePositionDelta.y*-CAMERA_FREE_MOUSE_SENSITIVITY;

                        // Angle clamp
                        if (CAMERA.angle.y > CAMERA_FREE_MIN_CLAMP*DEG2RAD) CAMERA.angle.y = CAMERA_FREE_MIN_CLAMP*DEG2RAD;
                        else if (CAMERA.angle.y < CAMERA_FREE_MAX_CLAMP*DEG2RAD) CAMERA.angle.y = CAMERA_FREE_MAX_CLAMP*DEG2RAD;
                    }
                }
                else
                {
                    // Camera panning
                    camera->target.x += ((mousePositionDelta.x*CAMERA_FREE_MOUSE_SENSITIVITY)*cosf(CAMERA.angle.x) + (mousePositionDelta.y*CAMERA_FREE_MOUSE_SENSITIVITY)*sinf(CAMERA.angle.x)*sinf(CAMERA.angle.y))*(CAMERA.targetDistance/CAMERA_FREE_PANNING_DIVIDER);
                    camera->target.y += ((mousePositionDelta.y*CAMERA_FREE_MOUSE_SENSITIVITY)*cosf(CAMERA.angle.y))*(CAMERA.targetDistance/CAMERA_FREE_PANNING_DIVIDER);
                    camera->target.z += ((mousePositionDelta.x*-CAMERA_FREE_MOUSE_SENSITIVITY)*sinf(CAMERA.angle.x) + (mousePositionDelta.y*CAMERA_FREE_MOUSE_SENSITIVITY)*cosf(CAMERA.angle.x)*sinf(CAMERA.angle.y))*(CAMERA.targetDistance/CAMERA_FREE_PANNING_DIVIDER);
                }
            }

            // Update camera position with changes
            camera->position.x = -sinf(CAMERA.angle.x)*CAMERA.targetDistance*cosf(CAMERA.angle.y) + camera->target.x;
            camera->position.y = -sinf(CAMERA.angle.y)*CAMERA.targetDistance + camera->target.y;
            camera->position.z = -cosf(CAMERA.angle.x)*CAMERA.targetDistance*cosf(CAMERA.angle.y) + camera->target.z;

        } break;
        case CAMERA_ORBITAL:        // Camera just orbits around target, only zoom allowed
        {
            CAMERA.angle.x += CAMERA_ORBITAL_SPEED;      // Camera orbit angle
            CAMERA.targetDistance -= (mouseWheelMove*CAMERA_MOUSE_SCROLL_SENSITIVITY);   // Camera zoom

            // Camera distance clamp
            if (CAMERA.targetDistance < CAMERA_THIRD_PERSON_DISTANCE_CLAMP) CAMERA.targetDistance = CAMERA_THIRD_PERSON_DISTANCE_CLAMP;

            // Update camera position with changes
            camera->position.x = sinf(CAMERA.angle.x)*CAMERA.targetDistance*cosf(CAMERA.angle.y) + camera->target.x;
            camera->position.y = ((CAMERA.angle.y <= 0.0f)? 1 : -1)*sinf(CAMERA.angle.y)*CAMERA.targetDistance*sinf(CAMERA.angle.y) + camera->target.y;
            camera->position.z = cosf(CAMERA.angle.x)*CAMERA.targetDistance*cosf(CAMERA.angle.y) + camera->target.z;

        } break;
        case CAMERA_FIRST_PERSON:   // Camera moves as in a first-person game, controls are configurable
        {
            camera->position.x += (sinf(CAMERA.angle.x)*direction[MOVE_BACK] -
                                   sinf(CAMERA.angle.x)*direction[MOVE_FRONT] -
                                   cosf(CAMERA.angle.x)*direction[MOVE_LEFT] +
                                   cosf(CAMERA.angle.x)*direction[MOVE_RIGHT])/PLAYER_MOVEMENT_SENSITIVITY;

            camera->position.y += (sinf(CAMERA.angle.y)*direction[MOVE_FRONT] -
                                   sinf(CAMERA.angle.y)*direction[MOVE_BACK] +
                                   1.0f*direction[MOVE_UP] - 1.0f*direction[MOVE_DOWN])/PLAYER_MOVEMENT_SENSITIVITY;

            camera->position.z += (cosf(CAMERA.angle.x)*direction[MOVE_BACK] -
                                   cosf(CAMERA.angle.x)*direction[MOVE_FRONT] +
                                   sinf(CAMERA.angle.x)*direction[MOVE_LEFT] -
                                   sinf(CAMERA.angle.x)*direction[MOVE_RIGHT])/PLAYER_MOVEMENT_SENSITIVITY;

            // Camera orientation calculation
            CAMERA.angle.x += (mousePositionDelta.x*-CAMERA_MOUSE_MOVE_SENSITIVITY);
            CAMERA.angle.y += (mousePositionDelta.y*-CAMERA_MOUSE_MOVE_SENSITIVITY);

            // Angle clamp
            if (CAMERA.angle.y > CAMERA_FIRST_PERSON_MIN_CLAMP*DEG2RAD) CAMERA.angle.y = CAMERA_FIRST_PERSON_MIN_CLAMP*DEG2RAD;
            else if (CAMERA.angle.y < CAMERA_FIRST_PERSON_MAX_CLAMP*DEG2RAD) CAMERA.angle.y = CAMERA_FIRST_PERSON_MAX_CLAMP*DEG2RAD;

            // Recalculate camera target considering translation and rotation
            Matrix translation = MatrixTranslate(0, 0, (CAMERA.targetDistance/CAMERA_FREE_PANNING_DIVIDER));
            Matrix rotation = MatrixRotateXYZ((Vector3){ PI*2 - CAMERA.angle.y, PI*2 - CAMERA.angle.x, 0 });
            Matrix transform = MatrixMultiply(translation, rotation);

            camera->target.x = camera->position.x - transform.m12;
            camera->target.y = camera->position.y - transform.m13;
            camera->target.z = camera->position.z - transform.m14;

            // If movement detected (some key pressed), increase swinging
            for (int i = 0; i < 6; i++) if (direction[i]) { swingCounter++; break; }

            // Camera position update
            // NOTE: On CAMERA_FIRST_PERSON player Y-movement is limited to player 'eyes position'
            camera->position.y = CAMERA.playerEyesPosition - sinf(swingCounter/CAMERA_FIRST_PERSON_STEP_TRIGONOMETRIC_DIVIDER)/CAMERA_FIRST_PERSON_STEP_DIVIDER;

            camera->up.x = sinf(swingCounter/(CAMERA_FIRST_PERSON_STEP_TRIGONOMETRIC_DIVIDER*2))/CAMERA_FIRST_PERSON_WAVING_DIVIDER;
            camera->up.z = -sinf(swingCounter/(CAMERA_FIRST_PERSON_STEP_TRIGONOMETRIC_DIVIDER*2))/CAMERA_FIRST_PERSON_WAVING_DIVIDER;

        } break;
        case CAMERA_THIRD_PERSON:   // Camera moves as in a third-person game, following target at a distance, controls are configurable
        {
            camera->position.x += (sinf(CAMERA.angle.x)*direction[MOVE_BACK] -
                                   sinf(CAMERA.angle.x)*direction[MOVE_FRONT] -
                                   cosf(CAMERA.angle.x)*direction[MOVE_LEFT] +
                                   cosf(CAMERA.angle.x)*direction[MOVE_RIGHT])/PLAYER_MOVEMENT_SENSITIVITY;

            camera->position.y += (sinf(CAMERA.angle.y)*direction[MOVE_FRONT] -
                                   sinf(CAMERA.angle.y)*direction[MOVE_BACK] +
                                   1.0f*direction[MOVE_UP] - 1.0f*direction[MOVE_DOWN])/PLAYER_MOVEMENT_SENSITIVITY;

            camera->position.z += (cosf(CAMERA.angle.x)*direction[MOVE_BACK] -
                                   cosf(CAMERA.angle.x)*direction[MOVE_FRONT] +
                                   sinf(CAMERA.angle.x)*direction[MOVE_LEFT] -
                                   sinf(CAMERA.angle.x)*direction[MOVE_RIGHT])/PLAYER_MOVEMENT_SENSITIVITY;

            // Camera orientation calculation
            CAMERA.angle.x += (mousePositionDelta.x*-CAMERA_MOUSE_MOVE_SENSITIVITY);
            CAMERA.angle.y += (mousePositionDelta.y*-CAMERA_MOUSE_MOVE_SENSITIVITY);

            // Angle clamp
            if (CAMERA.angle.y > CAMERA_THIRD_PERSON_MIN_CLAMP*DEG2RAD) CAMERA.angle.y = CAMERA_THIRD_PERSON_MIN_CLAMP*DEG2RAD;
            else if (CAMERA.angle.y < CAMERA_THIRD_PERSON_MAX_CLAMP*DEG2RAD) CAMERA.angle.y = CAMERA_THIRD_PERSON_MAX_CLAMP*DEG2RAD;

            // Camera zoom
            CAMERA.targetDistance -= (mouseWheelMove*CAMERA_MOUSE_SCROLL_SENSITIVITY);

            // Camera distance clamp
            if (CAMERA.targetDistance < CAMERA_THIRD_PERSON_DISTANCE_CLAMP) CAMERA.targetDistance = CAMERA_THIRD_PERSON_DISTANCE_CLAMP;

            // TODO: It seems camera->position is not correctly updated or some rounding issue makes the camera move straight to camera->target...
            camera->position.x = sinf(CAMERA.angle.x)*CAMERA.targetDistance*cosf(CAMERA.angle.y) + camera->target.x;

            if (CAMERA.angle.y <= 0.0f) camera->position.y = sinf(CAMERA.angle.y)*CAMERA.targetDistance*sinf(CAMERA.angle.y) + camera->target.y;
            else camera->position.y = -sinf(CAMERA.angle.y)*CAMERA.targetDistance*sinf(CAMERA.angle.y) + camera->target.y;

            camera->position.z = cosf(CAMERA.angle.x)*CAMERA.targetDistance*cosf(CAMERA.angle.y) + camera->target.z;

        } break;
        case CAMERA_CUSTOM: break;
        default: break;
    }
}

// Set camera pan key to combine with mouse movement (free camera)
void SetCameraPanControl(int keyPan) { CAMERA.panControl = keyPan; }

// Set camera alt key to combine with mouse movement (free camera)
void SetCameraAltControl(int keyAlt) { CAMERA.altControl = keyAlt; }

// Set camera smooth zoom key to combine with mouse (free camera)
void SetCameraSmoothZoomControl(int szoomKey) { CAMERA.smoothZoomControl = szoomKey; }

// Set camera move controls (1st person and 3rd person cameras)
void SetCameraMoveControls(int keyFront, int keyBack, int keyRight, int keyLeft, int keyUp, int keyDown)
{
    CAMERA.moveControl[MOVE_FRONT] = keyFront;
    CAMERA.moveControl[MOVE_BACK] = keyBack;
    CAMERA.moveControl[MOVE_RIGHT] = keyRight;
    CAMERA.moveControl[MOVE_LEFT] = keyLeft;
    CAMERA.moveControl[MOVE_UP] = keyUp;
    CAMERA.moveControl[MOVE_DOWN] = keyDown;
}

#endif // CAMERA_IMPLEMENTATION