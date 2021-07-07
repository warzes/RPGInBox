/**********************************************************************************************
*
*   raylib.core - Basic functions to manage windows, OpenGL context and input on multiple platforms
*
*   PLATFORMS SUPPORTED:
*       - PLATFORM_DESKTOP: Windows (Win32, Win64)
*       - PLATFORM_DESKTOP: Linux (X11 desktop mode)
*       - PLATFORM_DESKTOP: FreeBSD, OpenBSD, NetBSD, DragonFly (X11 desktop)
*       - PLATFORM_DESKTOP: OSX/macOS
*       - PLATFORM_ANDROID: Android (ARM, ARM64)
*       - PLATFORM_WEB:     HTML5 with WebAssembly
*
*   CONFIGURATION:
*
*   #define PLATFORM_DESKTOP
*       Windowing and input system configured for desktop platforms: Windows, Linux, OSX, FreeBSD, OpenBSD, NetBSD, DragonFly
*       NOTE: Oculus Rift CV1 requires PLATFORM_DESKTOP for mirror rendering - View [rlgl] module to enable it
*
*   #define PLATFORM_ANDROID
*       Windowing and input system configured for Android device, app activity managed internally in this module.
*       NOTE: OpenGL ES 2.0 is required and graphic device is managed by EGL
*
*   #define PLATFORM_WEB
*       Windowing and input system configured for HTML5 (run on browser), code converted from C to asm.js
*       using emscripten compiler. OpenGL ES 2.0 required for direct translation to WebGL equivalent code.
*
*   #define SUPPORT_DEFAULT_FONT (default)
*       Default font is loaded on window initialization to be available for the user to render simple text.
*       NOTE: If enabled, uses external module functions to load default raylib font (module: text)
*
*   #define SUPPORT_CAMERA_SYSTEM
*       Camera module is included (camera.h) and multiple predefined cameras are available: free, 1st/3rd person, orbital
*
*   #define SUPPORT_GESTURES_SYSTEM
*       Gestures module is included (gestures.h) to support gestures detection: tap, hold, swipe, drag
*
*   #define SUPPORT_MOUSE_GESTURES
*       Mouse gestures are directly mapped like touches and processed by gestures system.
*
*   #define SUPPORT_TOUCH_AS_MOUSE
*       Touch input and mouse input are shared. Mouse functions also return touch information.
*
*   #define SUPPORT_MOUSE_CURSOR_POINT
*       Draw a mouse pointer on screen
*
*   #define SUPPORT_BUSY_WAIT_LOOP
*       Use busy wait loop for timing sync, if not defined, a high-resolution timer is setup and used
*
*   #define SUPPORT_PARTIALBUSY_WAIT_LOOP
*       Use a partial-busy wait loop, in this case frame sleeps for most of the time and runs a busy-wait-loop at the end
*
*   #define SUPPORT_EVENTS_WAITING
*       Wait for events passively (sleeping while no events) instead of polling them actively every frame
*
*   #define SUPPORT_SCREEN_CAPTURE
*       Allow automatic screen capture of current screen pressing F12, defined in KeyCallback()
*
*   #define SUPPORT_GIF_RECORDING
*       Allow automatic gif recording of current screen pressing CTRL+F12, defined in KeyCallback()
*
*   #define SUPPORT_COMPRESSION_API
*       Support CompressData() and DecompressData() functions, those functions use zlib implementation
*       provided by stb_image and stb_image_write libraries, so, those libraries must be enabled on textures module
*       for linkage
*
*   #define SUPPORT_DATA_STORAGE
*       Support saving binary data automatically to a generated storage.data file. This file is managed internally

**********************************************************************************************/
#include "raylib.h"                 // Declares module functions
#include "config.h"             // Defines module configuration flags
#include "utils.h"                  // Required for: TRACELOG macros

#if (defined(__linux__) || defined(PLATFORM_WEB)) && _POSIX_C_SOURCE < 199309L
    #undef _POSIX_C_SOURCE
    #define _POSIX_C_SOURCE 199309L // Required for CLOCK_MONOTONIC if compiled with c99 without gnu ext.
#endif

#include "raymath.h"                // Required for: Vector3 and Matrix functions

#include "rlgl.h"                   // raylib OpenGL abstraction layer to OpenGL 1.1, 3.3+ or ES2

#if defined(SUPPORT_GESTURES_SYSTEM)
    #define GESTURES_IMPLEMENTATION
    #include "gestures.h"           // Gestures detection functionality
#endif

#if defined(SUPPORT_CAMERA_SYSTEM)
    #define CAMERA_IMPLEMENTATION
    #include "camera.h"             // Camera system functionality
#endif

#if defined(SUPPORT_GIF_RECORDING)
    //#define MSF_GIF_MALLOC RL_MALLOC
    //#define MSF_GIF_FREE RL_FREE

    #define MSF_GIF_IMPL
    #include "external/msf_gif.h"   // Support GIF recording
#endif

#if defined(SUPPORT_COMPRESSION_API)
    #define SINFL_IMPLEMENTATION
    #include "external/sinfl.h"

    #define SDEFL_IMPLEMENTATION
    #include "external/sdefl.h"
#endif

#include <stdlib.h>                 // Required for: srand(), rand(), atexit()
#include <string.h>                 // Required for: strrchr(), strcmp(), strlen()
#include <time.h>                   // Required for: time() [Used in InitTimer()]
#include <math.h>                   // Required for: tan() [Used in BeginMode3D()], atan2f() [Used in LoadVrStereoConfig()]

#include <sys/stat.h>               // Required for: stat() [Used in GetFileModTime()]

#if defined(PLATFORM_DESKTOP) && defined(_WIN32) && (defined(_MSC_VER) || defined(__TINYC__))
    #define DIRENT_MALLOC RL_MALLOC
    #define DIRENT_FREE RL_FREE

    #include "external/dirent.h"    // Required for: DIR, opendir(), closedir() [Used in GetDirectoryFiles()]
#else
    #include <dirent.h>             // Required for: DIR, opendir(), closedir() [Used in GetDirectoryFiles()]
#endif

#if defined(_WIN32)
    #include <direct.h>             // Required for: _getch(), _chdir()
    #define GETCWD _getcwd          // NOTE: MSDN recommends not to use getcwd(), chdir()
    #define CHDIR _chdir
    #include <io.h>                 // Required for _access() [Used in FileExists()]
#else
    #include <unistd.h>             // Required for: getch(), chdir() (POSIX), access()
    #define GETCWD getcwd
    #define CHDIR chdir
#endif

#if defined(PLATFORM_DESKTOP)
    #define GLFW_INCLUDE_NONE       // Disable the standard OpenGL header inclusion on GLFW3
                                    // NOTE: Already provided by rlgl implementation (on glad.h)
    #include "GLFW/glfw3.h"         // GLFW3 library: Windows, OpenGL context and Input management
                                    // NOTE: GLFW3 already includes gl.h (OpenGL) headers

    // Support retrieving native window handlers
    #if defined(_WIN32)
        #define GLFW_EXPOSE_NATIVE_WIN32
//        #include "GLFW/glfw3native.h"       // WARNING: It requires customization to avoid windows.h inclusion!

        #if defined(SUPPORT_WINMM_HIGHRES_TIMER) && !defined(SUPPORT_BUSY_WAIT_LOOP)
            // NOTE: Those functions require linking with winmm library
            unsigned int __stdcall timeBeginPeriod(unsigned int uPeriod);
            unsigned int __stdcall timeEndPeriod(unsigned int uPeriod);
        #endif
    #endif
    #if defined(__linux__) || defined(__FreeBSD__)
        #include <sys/time.h>               // Required for: timespec, nanosleep(), select() - POSIX

        //#define GLFW_EXPOSE_NATIVE_X11      // WARNING: Exposing Xlib.h > X.h results in dup symbols for Font type
        //#define GLFW_EXPOSE_NATIVE_WAYLAND
        //#define GLFW_EXPOSE_NATIVE_MIR
        #include "GLFW/glfw3native.h"       // Required for: glfwGetX11Window()
    #endif
    #if defined(__APPLE__)
        #include <unistd.h>                 // Required for: usleep()

        //#define GLFW_EXPOSE_NATIVE_COCOA    // WARNING: Fails due to type redefinition
        #include "GLFW/glfw3native.h"       // Required for: glfwGetCocoaWindow()
    #endif
#endif

#if defined(PLATFORM_ANDROID)
    //#include <android/sensor.h>           // Android sensors functions (accelerometer, gyroscope, light...)
    #include <android/window.h>             // Defines AWINDOW_FLAG_FULLSCREEN and others
    #include <android_native_app_glue.h>    // Defines basic app state struct and manages activity

    #include <EGL/egl.h>                    // Native platform windowing system interface
    //#include <GLES2/gl2.h>                // OpenGL ES 2.0 library (not required in this module, only in rlgl)
#endif

#if defined(PLATFORM_WEB)
    #define GLFW_INCLUDE_ES2            // GLFW3: Enable OpenGL ES 2.0 (translated to WebGL)
    #include "GLFW/glfw3.h"             // GLFW3 library: Windows, OpenGL context and Input management
    #include <sys/time.h>               // Required for: timespec, nanosleep(), select() - POSIX

    #include <emscripten/emscripten.h>  // Emscripten library - LLVM to JavaScript compiler
    #include <emscripten/html5.h>       // Emscripten HTML5 library
#endif

//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------

#ifndef MAX_FILEPATH_LENGTH
    #if defined(__linux__)
        #define MAX_FILEPATH_LENGTH     4096        // Maximum length for filepaths (Linux PATH_MAX default value)
    #else
        #define MAX_FILEPATH_LENGTH      512        // Maximum length supported for filepaths
    #endif
#endif

#ifndef MAX_KEYBOARD_KEYS
    #define MAX_KEYBOARD_KEYS            512        // Maximum number of keyboard keys supported
#endif
#ifndef MAX_MOUSE_BUTTONS
    #define MAX_MOUSE_BUTTONS              8        // Maximum number of mouse buttons supported
#endif
#ifndef MAX_GAMEPADS
    #define MAX_GAMEPADS                   4        // Maximum number of gamepads supported
#endif
#ifndef MAX_GAMEPAD_AXIS
    #define MAX_GAMEPAD_AXIS               8        // Maximum number of axis supported (per gamepad)
#endif
#ifndef MAX_GAMEPAD_BUTTONS
    #define MAX_GAMEPAD_BUTTONS           32        // Maximum number of buttons supported (per gamepad)
#endif
#ifndef MAX_TOUCH_POINTS
    #define MAX_TOUCH_POINTS              10        // Maximum number of touch points supported
#endif
#ifndef MAX_KEY_PRESSED_QUEUE
    #define MAX_KEY_PRESSED_QUEUE         16        // Maximum number of keys in the key input queue
#endif
#ifndef MAX_CHAR_PRESSED_QUEUE
    #define MAX_CHAR_PRESSED_QUEUE        16        // Maximum number of characters in the char input queue
#endif

#if defined(SUPPORT_DATA_STORAGE)
    #ifndef STORAGE_DATA_FILE
        #define STORAGE_DATA_FILE  "storage.data"   // Automatic storage filename
    #endif
#endif

#ifndef MAX_DECOMPRESSION_SIZE
    #define MAX_DECOMPRESSION_SIZE        64        // Maximum size allocated for decompression in MB
#endif

// Flags operation macros
#define FLAG_SET(n, f) ((n) |= (f))
#define FLAG_CLEAR(n, f) ((n) &= ~(f))
#define FLAG_TOGGLE(n, f) ((n) ^= (f))
#define FLAG_CHECK(n, f) ((n) & (f))

//----------------------------------------------------------------------------------
// Types and Structures Definition
//----------------------------------------------------------------------------------

typedef struct { int x; int y; } Point;
typedef struct { unsigned int width; unsigned int height; } Size;

// Core global state context data
typedef struct CoreData {
    struct {
#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
        GLFWwindow *handle;                 // GLFW window handle (graphic device)
#endif
#if defined(PLATFORM_ANDROID)
        EGLDisplay device;                  // Native display device (physical screen connection)
        EGLSurface surface;                 // Surface to draw on, framebuffers (connected to context)
        EGLContext context;                 // Graphic context, mode in which drawing can be done
        EGLConfig config;                   // Graphic config
#endif
        const char *title;                  // Window text title const pointer
        unsigned int flags;                 // Configuration flags (bit based), keeps window state
        bool ready;                         // Check if window has been initialized successfully
        bool fullscreen;                    // Check if fullscreen mode is enabled
        bool shouldClose;                   // Check if window set for closing
        bool resizedLastFrame;              // Check if window has been resized last frame

        Point position;                     // Window position on screen (required on fullscreen toggle)
        Size display;                       // Display width and height (monitor, device-screen, LCD, ...)
        Size screen;                        // Screen width and height (used render area)
        Size currentFbo;                    // Current render width and height (depends on active fbo)
        Size render;                        // Framebuffer width and height (render area, including black bars if required)
        Point renderOffset;                 // Offset from render area (must be divided by 2)
        Matrix screenScale;                 // Matrix to scale screen (framebuffer rendering)

        char **dropFilesPath;               // Store dropped files paths as strings
        int dropFilesCount;                 // Count dropped files strings

    } Window;
#if defined(PLATFORM_ANDROID)
    struct {
        bool appEnabled;                    // Flag to detect if app is active ** = true
        struct android_app *app;            // Android activity
        struct android_poll_source *source; // Android events polling source
        bool contextRebindRequired;         // Used to know context rebind required
    } Android;
#endif
    struct {
        const char *basePath;               // Base path for data storage
    } Storage;
    struct {
        struct {
            char currentKeyState[MAX_KEYBOARD_KEYS];        // Registers current frame key state
            char previousKeyState[MAX_KEYBOARD_KEYS];       // Registers previous frame key state

            int keyPressedQueue[MAX_KEY_PRESSED_QUEUE];     // Input keys queue
            int keyPressedQueueCount;       // Input keys queue count

            int charPressedQueue[MAX_CHAR_PRESSED_QUEUE];   // Input characters queue
            int charPressedQueueCount;      // Input characters queue count

        } Keyboard;
        struct {
            Vector2 offset;                 // Mouse offset
            Vector2 scale;                  // Mouse scaling
            Vector2 currentPosition;        // Mouse position on screen
            Vector2 previousPosition;       // Previous mouse position

            int cursor;                     // Tracks current mouse cursor
            bool cursorHidden;              // Track if cursor is hidden
            bool cursorOnScreen;            // Tracks if cursor is inside client area

            char currentButtonState[MAX_MOUSE_BUTTONS];     // Registers current mouse button state
            char previousButtonState[MAX_MOUSE_BUTTONS];    // Registers previous mouse button state
            float currentWheelMove;         // Registers current mouse wheel variation
            float previousWheelMove;        // Registers previous mouse wheel variation
        } Mouse;
        struct {
            Vector2 position[MAX_TOUCH_POINTS];         // Touch position on screen
            char currentTouchState[MAX_TOUCH_POINTS];   // Registers current touch state
            char previousTouchState[MAX_TOUCH_POINTS];  // Registers previous touch state
        } Touch;
        struct {
            int lastButtonPressed;          // Register last gamepad button pressed
            int axisCount;                  // Register number of available gamepad axis
            bool ready[MAX_GAMEPADS];       // Flag to know if gamepad is ready
            char currentButtonState[MAX_GAMEPADS][MAX_GAMEPAD_BUTTONS];     // Current gamepad buttons state
            char previousButtonState[MAX_GAMEPADS][MAX_GAMEPAD_BUTTONS];    // Previous gamepad buttons state
            float axisState[MAX_GAMEPADS][MAX_GAMEPAD_AXIS];                // Gamepad axis state
        } Gamepad;
    } Input;
    struct {
        double current;                     // Current time measure
        double previous;                    // Previous time measure
        double update;                      // Time measure for frame update
        double draw;                        // Time measure for frame draw
        double frame;                       // Time measure for one frame
        double target;                      // Desired time for one frame, if 0 not applied
#if defined(PLATFORM_ANDROID)
        unsigned long long base;            // Base time measure for hi-res timer
#endif
        unsigned int frameCounter;          // Frame counter
    } Time;
} CoreData;

//----------------------------------------------------------------------------------
// Global Variables Definition
//----------------------------------------------------------------------------------
static CoreData CORE = { 0 };               // Global CORE state context

static char **dirFilesPath = NULL;          // Store directory files paths as strings
static int dirFilesCount = 0;               // Count directory files strings

#if defined(SUPPORT_SCREEN_CAPTURE)
static int screenshotCounter = 0;           // Screenshots counter
#endif

#if defined(SUPPORT_GIF_RECORDING)
static int gifFramesCounter = 0;            // GIF frames counter
static bool gifRecording = false;           // GIF recording state
static MsfGifState gifState = { 0 };        // MSGIF context state
#endif

//-----------------------------------------------------------------------------------

//----------------------------------------------------------------------------------
// Other Modules Functions Declaration (required by core)
//----------------------------------------------------------------------------------
#if defined(SUPPORT_DEFAULT_FONT)
extern void LoadFontDefault(void);          // [Module: text] Loads default font on InitWindow()
extern void UnloadFontDefault(void);        // [Module: text] Unloads default font from GPU memory
#endif

//----------------------------------------------------------------------------------
// Module specific Functions Declaration
//----------------------------------------------------------------------------------
static void InitTimer(void);                            // Initialize timer (hi-resolution if available)
static bool InitGraphicsDevice(int width, int height);  // Initialize graphics device
static void SetupFramebuffer(int width, int height);    // Setup main framebuffer
static void SetupViewport(int width, int height);       // Set viewport for a provided width and height

#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
static void ErrorCallback(int error, const char *description);                             // GLFW3 Error Callback, runs on GLFW3 error
// Window callbacks events
static void WindowSizeCallback(GLFWwindow *window, int width, int height);                 // GLFW3 WindowSize Callback, runs when window is resized
#if !defined(PLATFORM_WEB)
static void WindowMaximizeCallback(GLFWwindow* window, int maximized);                     // GLFW3 Window Maximize Callback, runs when window is maximized
#endif
static void WindowIconifyCallback(GLFWwindow *window, int iconified);                      // GLFW3 WindowIconify Callback, runs when window is minimized/restored
static void WindowFocusCallback(GLFWwindow *window, int focused);                          // GLFW3 WindowFocus Callback, runs when window get/lose focus
static void WindowDropCallback(GLFWwindow *window, int count, const char **paths);         // GLFW3 Window Drop Callback, runs when drop files into window
// Input callbacks events
static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods);  // GLFW3 Keyboard Callback, runs on key pressed
static void CharCallback(GLFWwindow *window, unsigned int key);                            // GLFW3 Char Key Callback, runs on key pressed (get char value)
static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods);     // GLFW3 Mouse Button Callback, runs on mouse button pressed
static void MouseCursorPosCallback(GLFWwindow *window, double x, double y);                // GLFW3 Cursor Position Callback, runs on mouse move
static void MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset);       // GLFW3 Srolling Callback, runs on mouse wheel
static void CursorEnterCallback(GLFWwindow *window, int enter);                            // GLFW3 Cursor Enter Callback, cursor enters client area
#endif

#if defined(PLATFORM_ANDROID)
static void AndroidCommandCallback(struct android_app *app, int32_t cmd);                  // Process Android activity lifecycle commands
static int32_t AndroidInputCallback(struct android_app *app, AInputEvent *event);          // Process Android inputs
#endif

#if defined(PLATFORM_WEB)
static EM_BOOL EmscriptenTouchCallback(int eventType, const EmscriptenTouchEvent *touchEvent, void *userData);
static EM_BOOL EmscriptenGamepadCallback(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData);
static EM_BOOL EmscriptenResizeCallback(int eventType, const EmscriptenUiEvent *e, void *userData);

#endif

#if defined(_WIN32)
// NOTE: We declare Sleep() function symbol to avoid including windows.h (kernel32.lib linkage required)
void __stdcall Sleep(unsigned long msTimeout);          // Required for WaitTime()
#endif

//----------------------------------------------------------------------------------
// Module Functions Definition - Window and OpenGL Context Functions
//----------------------------------------------------------------------------------

#if defined(PLATFORM_ANDROID)
// To allow easier porting to android, we allow the user to define a
// main function which we call from android_main, defined by ourselves
extern int main(int argc, char *argv[]);

void android_main(struct android_app *app)
{
    char arg0[] = "raylib";     // NOTE: argv[] are mutable
    CORE.Android.app = app;

    // TODO: Should we maybe report != 0 return codes somewhere?
    (void)main(1, (char *[]) { arg0, NULL });
}

// TODO: Add this to header (if apps really need it)
struct android_app *GetAndroidApp(void)
{
    return CORE.Android.app;
}
#endif

// Initialize window and OpenGL context
// NOTE: data parameter could be used to pass any kind of required data to the initialization
void InitWindow(int width, int height, const char *title)
{
    TRACELOG(LOG_INFO, "Initializing raylib %s", RAYLIB_VERSION);

    if ((title != NULL) && (title[0] != 0)) CORE.Window.title = title;

    // Initialize required global values different than 0
    CORE.Input.Mouse.scale = (Vector2){ 1.0f, 1.0f };
    CORE.Input.Mouse.cursor = MOUSE_CURSOR_ARROW;
    CORE.Input.Gamepad.lastButtonPressed = -1;

#if defined(PLATFORM_ANDROID)
    CORE.Window.screen.width = width;
    CORE.Window.screen.height = height;
    CORE.Window.currentFbo.width = width;
    CORE.Window.currentFbo.height = height;

    // Set desired windows flags before initializing anything
    ANativeActivity_setWindowFlags(CORE.Android.app->activity, AWINDOW_FLAG_FULLSCREEN, 0);  //AWINDOW_FLAG_SCALED, AWINDOW_FLAG_DITHER

    int orientation = AConfiguration_getOrientation(CORE.Android.app->config);

    if (orientation == ACONFIGURATION_ORIENTATION_PORT) TRACELOG(LOG_INFO, "ANDROID: Window orientation set as portrait");
    else if (orientation == ACONFIGURATION_ORIENTATION_LAND) TRACELOG(LOG_INFO, "ANDROID: Window orientation set as landscape");

    // TODO: Automatic orientation doesn't seem to work
    if (width <= height)
    {
        AConfiguration_setOrientation(CORE.Android.app->config, ACONFIGURATION_ORIENTATION_PORT);
        TRACELOG(LOG_WARNING, "ANDROID: Window orientation changed to portrait");
    }
    else
    {
        AConfiguration_setOrientation(CORE.Android.app->config, ACONFIGURATION_ORIENTATION_LAND);
        TRACELOG(LOG_WARNING, "ANDROID: Window orientation changed to landscape");
    }

    //AConfiguration_getDensity(CORE.Android.app->config);
    //AConfiguration_getKeyboard(CORE.Android.app->config);
    //AConfiguration_getScreenSize(CORE.Android.app->config);
    //AConfiguration_getScreenLong(CORE.Android.app->config);

    // Initialize App command system
    // NOTE: On APP_CMD_INIT_WINDOW -> InitGraphicsDevice(), InitTimer(), LoadFontDefault()...
    CORE.Android.app->onAppCmd = AndroidCommandCallback;
    // Initialize input events system
    CORE.Android.app->onInputEvent = AndroidInputCallback;

    // Initialize assets manager
    InitAssetManager(CORE.Android.app->activity->assetManager, CORE.Android.app->activity->internalDataPath);

    // Initialize base path for storage
    CORE.Storage.basePath = CORE.Android.app->activity->internalDataPath;

    TRACELOG(LOG_INFO, "ANDROID: App initialized successfully");

    // Android ALooper_pollAll() variables
    int pollResult = 0;
    int pollEvents = 0;

    // Wait for window to be initialized (display and context)
    while (!CORE.Window.ready)
    {
        // Process events loop
        while ((pollResult = ALooper_pollAll(0, NULL, &pollEvents, (void**)&CORE.Android.source)) >= 0)
        {
            // Process this event
            if (CORE.Android.source != NULL) CORE.Android.source->process(CORE.Android.app, CORE.Android.source);

            // NOTE: Never close window, native activity is controlled by the system!
            //if (CORE.Android.app->destroyRequested != 0) CORE.Window.shouldClose = true;
        }
    }
#endif
#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
    // Initialize graphics device (display device and OpenGL context)
    // NOTE: returns true if window and graphic device has been initialized successfully
    CORE.Window.ready = InitGraphicsDevice(width, height);

    // If graphic device is no properly initialized, we end program
    if (!CORE.Window.ready)
    {
        TRACELOG(LOG_FATAL, "Failed to initialize Graphic Device");
        return;
    }

    // Initialize hi-res timer
    InitTimer();

    // Initialize random seed
    srand((unsigned int)time(NULL));

    // Initialize base path for storage
    CORE.Storage.basePath = GetWorkingDirectory();

#if defined(SUPPORT_DEFAULT_FONT)
    // Load default font
    // NOTE: External functions (defined in module: text)
    LoadFontDefault();
    Rectangle rec = GetFontDefault().recs[95];
    // NOTE: We setup a 1px padding on char rectangle to avoid pixel bleeding on MSAA filtering
    SetShapesTexture(GetFontDefault().texture, (Rectangle){ rec.x + 1, rec.y + 1, rec.width - 2, rec.height - 2 });
#else
    // Set default internal texture (1px white) and rectangle to be used for shapes drawing
    SetShapesTexture(rlGetTextureDefault(), (Rectangle){ 0.0f, 0.0f, 1.0f, 1.0f });
#endif
#if defined(PLATFORM_DESKTOP)
    if ((CORE.Window.flags & FLAG_WINDOW_HIGHDPI) > 0)
    {
        // Set default font texture filter for HighDPI (blurry)
        SetTextureFilter(GetFontDefault().texture, TEXTURE_FILTER_BILINEAR);
    }
#endif

#if defined(PLATFORM_WEB)
    // Check fullscreen change events(note this is done on the window since most
    // browsers don't support this on #canvas)
    emscripten_set_fullscreenchange_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, 1, EmscriptenResizeCallback);
    // Check Resize event (note this is done on the window since most browsers
    // don't support this on #canvas)
    emscripten_set_resize_callback(EMSCRIPTEN_EVENT_TARGET_WINDOW, NULL, 1, EmscriptenResizeCallback);
    // Trigger this once to get initial window sizing
    EmscriptenResizeCallback(EMSCRIPTEN_EVENT_RESIZE, NULL, NULL);
    // Support keyboard events
    //emscripten_set_keypress_callback("#canvas", NULL, 1, EmscriptenKeyboardCallback);
    //emscripten_set_keydown_callback("#canvas", NULL, 1, EmscriptenKeyboardCallback);

    // Support touch events
    emscripten_set_touchstart_callback("#canvas", NULL, 1, EmscriptenTouchCallback);
    emscripten_set_touchend_callback("#canvas", NULL, 1, EmscriptenTouchCallback);
    emscripten_set_touchmove_callback("#canvas", NULL, 1, EmscriptenTouchCallback);
    emscripten_set_touchcancel_callback("#canvas", NULL, 1, EmscriptenTouchCallback);

    // Support gamepad events (not provided by GLFW3 on emscripten)
    emscripten_set_gamepadconnected_callback(NULL, 1, EmscriptenGamepadCallback);
    emscripten_set_gamepaddisconnected_callback(NULL, 1, EmscriptenGamepadCallback);
#endif

    CORE.Input.Mouse.currentPosition.x = (float)CORE.Window.screen.width/2.0f;
    CORE.Input.Mouse.currentPosition.y = (float)CORE.Window.screen.height/2.0f;

#endif        // PLATFORM_DESKTOP || PLATFORM_WEB
}

// Close window and unload OpenGL context
void CloseWindowRaylib(void)
{
#if defined(SUPPORT_GIF_RECORDING)
    if (gifRecording)
    {
        MsfGifResult result = msf_gif_end(&gifState);
        msf_gif_free(result);
        gifRecording = false;
    }
#endif

#if defined(SUPPORT_DEFAULT_FONT)
    UnloadFontDefault();
#endif

    rlglClose();                // De-init rlgl

#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
    glfwDestroyWindow(CORE.Window.handle);
    glfwTerminate();
#endif

#if defined(_WIN32) && defined(SUPPORT_WINMM_HIGHRES_TIMER) && !defined(SUPPORT_BUSY_WAIT_LOOP)
    timeEndPeriod(1);           // Restore time period
#endif

#if defined(PLATFORM_ANDROID)
    // Close surface, context and display
    if (CORE.Window.device != EGL_NO_DISPLAY)
    {
        eglMakeCurrent(CORE.Window.device, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (CORE.Window.surface != EGL_NO_SURFACE)
        {
            eglDestroySurface(CORE.Window.device, CORE.Window.surface);
            CORE.Window.surface = EGL_NO_SURFACE;
        }

        if (CORE.Window.context != EGL_NO_CONTEXT)
        {
            eglDestroyContext(CORE.Window.device, CORE.Window.context);
            CORE.Window.context = EGL_NO_CONTEXT;
        }

        eglTerminate(CORE.Window.device);
        CORE.Window.device = EGL_NO_DISPLAY;
    }
#endif

    CORE.Window.ready = false;
    TRACELOG(LOG_INFO, "Window closed successfully");
}

// Check if KEY_ESCAPE pressed or Close icon pressed
bool WindowShouldClose(void)
{
#if defined(PLATFORM_WEB)
    // Emterpreter-Async required to run sync code
    // https://github.com/emscripten-core/emscripten/wiki/Emterpreter#emterpreter-async-run-synchronous-code
    // By default, this function is never called on a web-ready raylib example because we encapsulate
    // frame code in a UpdateDrawFrame() function, to allow browser manage execution asynchronously
    // but now emscripten allows sync code to be executed in an interpreted way, using emterpreter!
    emscripten_sleep(16);
    return false;
#endif

#if defined(PLATFORM_DESKTOP)
    if (CORE.Window.ready)
    {
        // While window minimized, stop loop execution
        while (IsWindowState(FLAG_WINDOW_MINIMIZED) && !IsWindowState(FLAG_WINDOW_ALWAYS_RUN)) glfwWaitEvents();

        CORE.Window.shouldClose = glfwWindowShouldClose(CORE.Window.handle);

        // Reset close status for next frame
        glfwSetWindowShouldClose(CORE.Window.handle, GLFW_FALSE);

        return CORE.Window.shouldClose;
    }
    else return true;
#endif

#if defined(PLATFORM_ANDROID)
    if (CORE.Window.ready) return CORE.Window.shouldClose;
    else return true;
#endif
}

// Check if window has been initialized successfully
bool IsWindowReady(void)
{
    return CORE.Window.ready;
}

// Check if window is currently fullscreen
bool IsWindowFullscreen(void)
{
    return CORE.Window.fullscreen;
}

// Check if window is currently hidden
bool IsWindowHidden(void)
{
#if defined(PLATFORM_DESKTOP)
    return ((CORE.Window.flags & FLAG_WINDOW_HIDDEN) > 0);
#endif
    return false;
}

// Check if window has been minimized
bool IsWindowMinimized(void)
{
#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
    return ((CORE.Window.flags & FLAG_WINDOW_MINIMIZED) > 0);
#else
    return false;
#endif
}

// Check if window has been maximized (only PLATFORM_DESKTOP)
bool IsWindowMaximized(void)
{
#if defined(PLATFORM_DESKTOP)
    return ((CORE.Window.flags & FLAG_WINDOW_MAXIMIZED) > 0);
#else
    return false;
#endif
}

// Check if window has the focus
bool IsWindowFocused(void)
{
#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
    return ((CORE.Window.flags & FLAG_WINDOW_UNFOCUSED) == 0);      // TODO!
#else
    return true;
#endif
}

// Check if window has been resizedLastFrame
bool IsWindowResized(void)
{
#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
    return CORE.Window.resizedLastFrame;
#else
    return false;
#endif
}

// Check if one specific window flag is enabled
bool IsWindowState(unsigned int flag)
{
    return ((CORE.Window.flags & flag) > 0);
}

// Toggle fullscreen mode (only PLATFORM_DESKTOP)
void ToggleFullscreen(void)
{
#if defined(PLATFORM_DESKTOP)
    // NOTE: glfwSetWindowMonitor() doesn't work properly (bugs)
    if (!CORE.Window.fullscreen)
    {
        // Store previous window position (in case we exit fullscreen)
        glfwGetWindowPos(CORE.Window.handle, &CORE.Window.position.x, &CORE.Window.position.y);

        int monitorCount = 0;
        GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

        int monitorIndex = GetCurrentMonitor();

        // Use current monitor, so we correctly get the display the window is on
        GLFWmonitor* monitor = monitorIndex < monitorCount ?  monitors[monitorIndex] : NULL;

        if (!monitor)
        {
            TRACELOG(LOG_WARNING, "GLFW: Failed to get monitor");

            CORE.Window.fullscreen = false;          // Toggle fullscreen flag
            CORE.Window.flags &= ~FLAG_FULLSCREEN_MODE;

            glfwSetWindowMonitor(CORE.Window.handle, NULL, 0, 0, CORE.Window.screen.width, CORE.Window.screen.height, GLFW_DONT_CARE);
            return;
        }

        CORE.Window.fullscreen = true;          // Toggle fullscreen flag
        CORE.Window.flags |= FLAG_FULLSCREEN_MODE;

        glfwSetWindowMonitor(CORE.Window.handle, monitor, 0, 0, CORE.Window.screen.width, CORE.Window.screen.height, GLFW_DONT_CARE);
    }
    else
    {
        CORE.Window.fullscreen = false;          // Toggle fullscreen flag
        CORE.Window.flags &= ~FLAG_FULLSCREEN_MODE;

        glfwSetWindowMonitor(CORE.Window.handle, NULL, CORE.Window.position.x, CORE.Window.position.y, CORE.Window.screen.width, CORE.Window.screen.height, GLFW_DONT_CARE);
    }

    // Try to enable GPU V-Sync, so frames are limited to screen refresh rate (60Hz -> 60 FPS)
    // NOTE: V-Sync can be enabled by graphic driver configuration
    if (CORE.Window.flags & FLAG_VSYNC_HINT) glfwSwapInterval(1);
#endif
#if defined(PLATFORM_WEB)
    EM_ASM
    (
        // This strategy works well while using raylib minimal web shell for emscripten,
        // it re-scales the canvas to fullscreen using monitor resolution, for tools this
        // is a good strategy but maybe games prefer to keep current canvas resolution and
        // display it in fullscreen, adjusting monitor resolution if possible
        if (document.fullscreenElement) document.exitFullscreen();
        else Module.requestFullscreen(false, true);
    );

/*
    if (!CORE.Window.fullscreen)
    {
        // Option 1: Request fullscreen for the canvas element
        // This option does not seem to work at all
        //emscripten_request_fullscreen("#canvas", false);

        // Option 2: Request fullscreen for the canvas element with strategy
        // This option does not seem to work at all
        // Ref: https://github.com/emscripten-core/emscripten/issues/5124
        // EmscriptenFullscreenStrategy strategy = {
            // .scaleMode = EMSCRIPTEN_FULLSCREEN_SCALE_STRETCH, //EMSCRIPTEN_FULLSCREEN_SCALE_ASPECT,
            // .canvasResolutionScaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF,
            // .filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT,
            // .canvasResizedCallback = EmscriptenWindowResizedCallback,
            // .canvasResizedCallbackUserData = NULL
        // };
        //emscripten_request_fullscreen_strategy("#canvas", EM_FALSE, &strategy);

        // Option 3: Request fullscreen for the canvas element with strategy
        // It works as expected but only inside the browser (client area)
        EmscriptenFullscreenStrategy strategy = {
            .scaleMode = EMSCRIPTEN_FULLSCREEN_SCALE_ASPECT,
            .canvasResolutionScaleMode = EMSCRIPTEN_FULLSCREEN_CANVAS_SCALE_STDDEF,
            .filteringMode = EMSCRIPTEN_FULLSCREEN_FILTERING_DEFAULT,
            .canvasResizedCallback = EmscriptenWindowResizedCallback,
            .canvasResizedCallbackUserData = NULL
        };
        emscripten_enter_soft_fullscreen("#canvas", &strategy);

        int width, height;
        emscripten_get_canvas_element_size("#canvas", &width, &height);
        TRACELOG(LOG_WARNING, "Emscripten: Enter fullscreen: Canvas size: %i x %i", width, height);
    }
    else
    {
        //emscripten_exit_fullscreen();
        emscripten_exit_soft_fullscreen();

        int width, height;
        emscripten_get_canvas_element_size("#canvas", &width, &height);
        TRACELOG(LOG_WARNING, "Emscripten: Exit fullscreen: Canvas size: %i x %i", width, height);
    }
*/

    CORE.Window.fullscreen = !CORE.Window.fullscreen;          // Toggle fullscreen flag
    CORE.Window.flags ^= FLAG_FULLSCREEN_MODE;
#endif
#if defined(PLATFORM_ANDROID)
    TRACELOG(LOG_WARNING, "SYSTEM: Failed to toggle to windowed mode");
#endif
}

// Set window state: maximized, if resizable (only PLATFORM_DESKTOP)
void MaximizeWindow(void)
{
#if defined(PLATFORM_DESKTOP)
    if (glfwGetWindowAttrib(CORE.Window.handle, GLFW_RESIZABLE) == GLFW_TRUE)
    {
        glfwMaximizeWindow(CORE.Window.handle);
        CORE.Window.flags |= FLAG_WINDOW_MAXIMIZED;
    }
#endif
}

// Set window state: minimized (only PLATFORM_DESKTOP)
void MinimizeWindow(void)
{
#if defined(PLATFORM_DESKTOP)
    // NOTE: Following function launches callback that sets appropiate flag!
    glfwIconifyWindow(CORE.Window.handle);
#endif
}

// Set window state: not minimized/maximized (only PLATFORM_DESKTOP)
void RestoreWindow(void)
{
#if defined(PLATFORM_DESKTOP)
    if (glfwGetWindowAttrib(CORE.Window.handle, GLFW_RESIZABLE) == GLFW_TRUE)
    {
        // Restores the specified window if it was previously iconified (minimized) or maximized
        glfwRestoreWindow(CORE.Window.handle);
        CORE.Window.flags &= ~FLAG_WINDOW_MINIMIZED;
        CORE.Window.flags &= ~FLAG_WINDOW_MAXIMIZED;
    }
#endif
}

// Set window configuration state using flags
void SetWindowState(unsigned int flags)
{
#if defined(PLATFORM_DESKTOP)
    // Check previous state and requested state to apply required changes
    // NOTE: In most cases the functions already change the flags internally

    // State change: FLAG_VSYNC_HINT
    if (((CORE.Window.flags & FLAG_VSYNC_HINT) != (flags & FLAG_VSYNC_HINT)) && ((flags & FLAG_VSYNC_HINT) > 0))
    {
        glfwSwapInterval(1);
        CORE.Window.flags |= FLAG_VSYNC_HINT;
    }

    // State change: FLAG_FULLSCREEN_MODE
    if ((CORE.Window.flags & FLAG_FULLSCREEN_MODE) != (flags & FLAG_FULLSCREEN_MODE))
    {
        ToggleFullscreen();     // NOTE: Window state flag updated inside function
    }

    // State change: FLAG_WINDOW_RESIZABLE
    if (((CORE.Window.flags & FLAG_WINDOW_RESIZABLE) != (flags & FLAG_WINDOW_RESIZABLE)) && ((flags & FLAG_WINDOW_RESIZABLE) > 0))
    {
        glfwSetWindowAttrib(CORE.Window.handle, GLFW_RESIZABLE, GLFW_TRUE);
        CORE.Window.flags |= FLAG_WINDOW_RESIZABLE;
    }

    // State change: FLAG_WINDOW_UNDECORATED
    if (((CORE.Window.flags & FLAG_WINDOW_UNDECORATED) != (flags & FLAG_WINDOW_UNDECORATED)) && (flags & FLAG_WINDOW_UNDECORATED))
    {
        glfwSetWindowAttrib(CORE.Window.handle, GLFW_DECORATED, GLFW_FALSE);
        CORE.Window.flags |= FLAG_WINDOW_UNDECORATED;
    }

    // State change: FLAG_WINDOW_HIDDEN
    if (((CORE.Window.flags & FLAG_WINDOW_HIDDEN) != (flags & FLAG_WINDOW_HIDDEN)) && ((flags & FLAG_WINDOW_HIDDEN) > 0))
    {
        glfwHideWindow(CORE.Window.handle);
        CORE.Window.flags |= FLAG_WINDOW_HIDDEN;
    }

    // State change: FLAG_WINDOW_MINIMIZED
    if (((CORE.Window.flags & FLAG_WINDOW_MINIMIZED) != (flags & FLAG_WINDOW_MINIMIZED)) && ((flags & FLAG_WINDOW_MINIMIZED) > 0))
    {
        //GLFW_ICONIFIED
        MinimizeWindow();       // NOTE: Window state flag updated inside function
    }

    // State change: FLAG_WINDOW_MAXIMIZED
    if (((CORE.Window.flags & FLAG_WINDOW_MAXIMIZED) != (flags & FLAG_WINDOW_MAXIMIZED)) && ((flags & FLAG_WINDOW_MAXIMIZED) > 0))
    {
        //GLFW_MAXIMIZED
        MaximizeWindow();       // NOTE: Window state flag updated inside function
    }

    // State change: FLAG_WINDOW_UNFOCUSED
    if (((CORE.Window.flags & FLAG_WINDOW_UNFOCUSED) != (flags & FLAG_WINDOW_UNFOCUSED)) && ((flags & FLAG_WINDOW_UNFOCUSED) > 0))
    {
        glfwSetWindowAttrib(CORE.Window.handle, GLFW_FOCUS_ON_SHOW, GLFW_FALSE);
        CORE.Window.flags |= FLAG_WINDOW_UNFOCUSED;
    }

    // State change: FLAG_WINDOW_TOPMOST
    if (((CORE.Window.flags & FLAG_WINDOW_TOPMOST) != (flags & FLAG_WINDOW_TOPMOST)) && ((flags & FLAG_WINDOW_TOPMOST) > 0))
    {
        glfwSetWindowAttrib(CORE.Window.handle, GLFW_FLOATING, GLFW_TRUE);
        CORE.Window.flags |= FLAG_WINDOW_TOPMOST;
    }

    // State change: FLAG_WINDOW_ALWAYS_RUN
    if (((CORE.Window.flags & FLAG_WINDOW_ALWAYS_RUN) != (flags & FLAG_WINDOW_ALWAYS_RUN)) && ((flags & FLAG_WINDOW_ALWAYS_RUN) > 0))
    {
        CORE.Window.flags |= FLAG_WINDOW_ALWAYS_RUN;
    }

    // The following states can not be changed after window creation

    // State change: FLAG_WINDOW_TRANSPARENT
    if (((CORE.Window.flags & FLAG_WINDOW_TRANSPARENT) != (flags & FLAG_WINDOW_TRANSPARENT)) && ((flags & FLAG_WINDOW_TRANSPARENT) > 0))
    {
        TRACELOG(LOG_WARNING, "WINDOW: Framebuffer transparency can only by configured before window initialization");
    }

    // State change: FLAG_WINDOW_HIGHDPI
    if (((CORE.Window.flags & FLAG_WINDOW_HIGHDPI) != (flags & FLAG_WINDOW_HIGHDPI)) && ((flags & FLAG_WINDOW_HIGHDPI) > 0))
    {
        TRACELOG(LOG_WARNING, "WINDOW: High DPI can only by configured before window initialization");
    }

    // State change: FLAG_MSAA_4X_HINT
    if (((CORE.Window.flags & FLAG_MSAA_4X_HINT) != (flags & FLAG_MSAA_4X_HINT)) && ((flags & FLAG_MSAA_4X_HINT) > 0))
    {
        TRACELOG(LOG_WARNING, "WINDOW: MSAA can only by configured before window initialization");
    }

    // State change: FLAG_INTERLACED_HINT
    if (((CORE.Window.flags & FLAG_INTERLACED_HINT) != (flags & FLAG_INTERLACED_HINT)) && ((flags & FLAG_INTERLACED_HINT) > 0))
    {
        TRACELOG(LOG_WARNING, "RPI: Interlaced mode can only by configured before window initialization");
    }
#endif
}

// Clear window configuration state flags
void ClearWindowState(unsigned int flags)
{
#if defined(PLATFORM_DESKTOP)
    // Check previous state and requested state to apply required changes
    // NOTE: In most cases the functions already change the flags internally

    // State change: FLAG_VSYNC_HINT
    if (((CORE.Window.flags & FLAG_VSYNC_HINT) > 0) && ((flags & FLAG_VSYNC_HINT) > 0))
    {
        glfwSwapInterval(0);
        CORE.Window.flags &= ~FLAG_VSYNC_HINT;
    }

    // State change: FLAG_FULLSCREEN_MODE
    if (((CORE.Window.flags & FLAG_FULLSCREEN_MODE) > 0) && ((flags & FLAG_FULLSCREEN_MODE) > 0))
    {
        ToggleFullscreen();     // NOTE: Window state flag updated inside function
    }

    // State change: FLAG_WINDOW_RESIZABLE
    if (((CORE.Window.flags & FLAG_WINDOW_RESIZABLE) > 0) && ((flags & FLAG_WINDOW_RESIZABLE) > 0))
    {
        glfwSetWindowAttrib(CORE.Window.handle, GLFW_RESIZABLE, GLFW_FALSE);
        CORE.Window.flags &= ~FLAG_WINDOW_RESIZABLE;
    }

    // State change: FLAG_WINDOW_UNDECORATED
    if (((CORE.Window.flags & FLAG_WINDOW_UNDECORATED) > 0) && ((flags & FLAG_WINDOW_UNDECORATED) > 0))
    {
        glfwSetWindowAttrib(CORE.Window.handle, GLFW_DECORATED, GLFW_TRUE);
        CORE.Window.flags &= ~FLAG_WINDOW_UNDECORATED;
    }

    // State change: FLAG_WINDOW_HIDDEN
    if (((CORE.Window.flags & FLAG_WINDOW_HIDDEN) > 0) && ((flags & FLAG_WINDOW_HIDDEN) > 0))
    {
        glfwShowWindow(CORE.Window.handle);
        CORE.Window.flags &= ~FLAG_WINDOW_HIDDEN;
    }

    // State change: FLAG_WINDOW_MINIMIZED
    if (((CORE.Window.flags & FLAG_WINDOW_MINIMIZED) > 0) && ((flags & FLAG_WINDOW_MINIMIZED) > 0))
    {
        RestoreWindow();       // NOTE: Window state flag updated inside function
    }

    // State change: FLAG_WINDOW_MAXIMIZED
    if (((CORE.Window.flags & FLAG_WINDOW_MAXIMIZED) > 0) && ((flags & FLAG_WINDOW_MAXIMIZED) > 0))
    {
        RestoreWindow();       // NOTE: Window state flag updated inside function
    }

    // State change: FLAG_WINDOW_UNFOCUSED
    if (((CORE.Window.flags & FLAG_WINDOW_UNFOCUSED) > 0) && ((flags & FLAG_WINDOW_UNFOCUSED) > 0))
    {
        glfwSetWindowAttrib(CORE.Window.handle, GLFW_FOCUS_ON_SHOW, GLFW_TRUE);
        CORE.Window.flags &= ~FLAG_WINDOW_UNFOCUSED;
    }

    // State change: FLAG_WINDOW_TOPMOST
    if (((CORE.Window.flags & FLAG_WINDOW_TOPMOST) > 0) && ((flags & FLAG_WINDOW_TOPMOST) > 0))
    {
        glfwSetWindowAttrib(CORE.Window.handle, GLFW_FLOATING, GLFW_FALSE);
        CORE.Window.flags &= ~FLAG_WINDOW_TOPMOST;
    }

    // State change: FLAG_WINDOW_ALWAYS_RUN
    if (((CORE.Window.flags & FLAG_WINDOW_ALWAYS_RUN) > 0) && ((flags & FLAG_WINDOW_ALWAYS_RUN) > 0))
    {
        CORE.Window.flags &= ~FLAG_WINDOW_ALWAYS_RUN;
    }

    // The following states can not be changed after window creation

    // State change: FLAG_WINDOW_TRANSPARENT
    if (((CORE.Window.flags & FLAG_WINDOW_TRANSPARENT) > 0) && ((flags & FLAG_WINDOW_TRANSPARENT) > 0))
    {
        TRACELOG(LOG_WARNING, "WINDOW: Framebuffer transparency can only by configured before window initialization");
    }

    // State change: FLAG_WINDOW_HIGHDPI
    if (((CORE.Window.flags & FLAG_WINDOW_HIGHDPI) > 0) && ((flags & FLAG_WINDOW_HIGHDPI) > 0))
    {
        TRACELOG(LOG_WARNING, "WINDOW: High DPI can only by configured before window initialization");
    }

    // State change: FLAG_MSAA_4X_HINT
    if (((CORE.Window.flags & FLAG_MSAA_4X_HINT) > 0) && ((flags & FLAG_MSAA_4X_HINT) > 0))
    {
        TRACELOG(LOG_WARNING, "WINDOW: MSAA can only by configured before window initialization");
    }

    // State change: FLAG_INTERLACED_HINT
    if (((CORE.Window.flags & FLAG_INTERLACED_HINT) > 0) && ((flags & FLAG_INTERLACED_HINT) > 0))
    {
        TRACELOG(LOG_WARNING, "RPI: Interlaced mode can only by configured before window initialization");
    }
#endif
}

// Set icon for window (only PLATFORM_DESKTOP)
// NOTE: Image must be in RGBA format, 8bit per channel
void SetWindowIcon(Image image)
{
#if defined(PLATFORM_DESKTOP)
    if (image.format == PIXELFORMAT_UNCOMPRESSED_R8G8B8A8)
    {
        GLFWimage icon[1] = { 0 };

        icon[0].width = image.width;
        icon[0].height = image.height;
        icon[0].pixels = (unsigned char *)image.data;

        // NOTE 1: We only support one image icon
        // NOTE 2: The specified image data is copied before this function returns
        glfwSetWindowIcon(CORE.Window.handle, 1, icon);
    }
    else TRACELOG(LOG_WARNING, "GLFW: Window icon image must be in R8G8B8A8 pixel format");
#endif
}

// Set title for window (only PLATFORM_DESKTOP)
void SetWindowTitle(const char *title)
{
    CORE.Window.title = title;
#if defined(PLATFORM_DESKTOP)
    glfwSetWindowTitle(CORE.Window.handle, title);
#endif
}

// Set window position on screen (windowed mode)
void SetWindowPosition(int x, int y)
{
#if defined(PLATFORM_DESKTOP)
    glfwSetWindowPos(CORE.Window.handle, x, y);
#endif
}

// Set monitor for the current window (fullscreen mode)
void SetWindowMonitor(int monitor)
{
#if defined(PLATFORM_DESKTOP)
    int monitorCount = 0;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

    if ((monitor >= 0) && (monitor < monitorCount))
    {
        TRACELOG(LOG_INFO, "GLFW: Selected fullscreen monitor: [%i] %s", monitor, glfwGetMonitorName(monitors[monitor]));

        const GLFWvidmode *mode = glfwGetVideoMode(monitors[monitor]);
        glfwSetWindowMonitor(CORE.Window.handle, monitors[monitor], 0, 0, mode->width, mode->height, mode->refreshRate);
    }
    else TRACELOG(LOG_WARNING, "GLFW: Failed to find selected monitor");
#endif
}

// Set window minimum dimensions (FLAG_WINDOW_RESIZABLE)
void SetWindowMinSize(int width, int height)
{
#if defined(PLATFORM_DESKTOP)
    const GLFWvidmode *mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    glfwSetWindowSizeLimits(CORE.Window.handle, width, height, mode->width, mode->height);
#endif
}

// Set window dimensions
// TODO: Issues on HighDPI scaling
void SetWindowSize(int width, int height)
{
#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
    glfwSetWindowSize(CORE.Window.handle, width, height);
#endif
#if defined(PLATFORM_WEB)
    //emscripten_set_canvas_size(width, height);  // DEPRECATED!

    // TODO: Below functions should be used to replace previous one but
    // they do not seem to work properly
    //emscripten_set_canvas_element_size("canvas", width, height);
    //emscripten_set_element_css_size("canvas", width, height);
#endif
}

// Get current screen width
int GetScreenWidth(void)
{
    return CORE.Window.currentFbo.width;
}

// Get current screen height
int GetScreenHeight(void)
{
    return CORE.Window.currentFbo.height;
}

// Get native window handle
void *GetWindowHandle(void)
{
#if defined(PLATFORM_DESKTOP) && defined(_WIN32)
    // NOTE: Returned handle is: void *HWND (windows.h)
    return glfwGetWin32Window(CORE.Window.handle);
#endif
#if defined(__linux__)
    // NOTE: Returned handle is: unsigned long Window (X.h)
    // typedef unsigned long XID;
    // typedef XID Window;
    //unsigned long id = (unsigned long)glfwGetX11Window(window);
    return NULL;    // TODO: Find a way to return value... cast to void *?
#endif
#if defined(__APPLE__)
    // NOTE: Returned handle is: (objc_object *)
    return NULL;    // TODO: return (void *)glfwGetCocoaWindow(window);
#endif

    return NULL;
}

// Get number of monitors
int GetMonitorCount(void)
{
#if defined(PLATFORM_DESKTOP)
    int monitorCount;
    glfwGetMonitors(&monitorCount);
    return monitorCount;
#else
    return 1;
#endif
}

// Get number of monitors
int GetCurrentMonitor(void)
{
#if defined(PLATFORM_DESKTOP)
    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);
    GLFWmonitor* monitor = NULL;

    if (monitorCount == 1) // easy out
        return 0;

    if (IsWindowFullscreen())
    {
        monitor = glfwGetWindowMonitor(CORE.Window.handle);
        for (int i = 0; i < monitorCount; i++)
        {
            if (monitors[i] == monitor)
                return i;
        }
        return 0;
    }
    else
    {
        int x = 0;
        int y = 0;

        glfwGetWindowPos(CORE.Window.handle, &x, &y);

        for (int i = 0; i < monitorCount; i++)
        {
            int mx = 0;
            int my = 0;

            int width = 0;
            int height = 0;

            monitor = monitors[i];
            glfwGetMonitorWorkarea(monitor, &mx, &my, &width, &height);
            if (x >= mx && x <= (mx + width) && y >= my && y <= (my + height))
                return i;
        }
    }
    return 0;
#else
    return 0;
#endif
}

// Get selected monitor width
Vector2 GetMonitorPosition(int monitor)
{
#if defined(PLATFORM_DESKTOP)
    int monitorCount;
    GLFWmonitor** monitors = glfwGetMonitors(&monitorCount);

    if ((monitor >= 0) && (monitor < monitorCount))
    {
        int x, y;
        glfwGetMonitorPos(monitors[monitor], &x, &y);

        return (Vector2){ (float)x, (float)y };
    }
    else TRACELOG(LOG_WARNING, "GLFW: Failed to find selected monitor");
#endif
    return (Vector2){ 0, 0 };
}

// Get selected monitor width (max available by monitor)
int GetMonitorWidth(int monitor)
{
#if defined(PLATFORM_DESKTOP)
    int monitorCount;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

    if ((monitor >= 0) && (monitor < monitorCount))
    {
        int count = 0;
        const GLFWvidmode *modes = glfwGetVideoModes(monitors[monitor], &count);

        // We return the maximum resolution available, the last one in the modes array
        if (count > 0) return modes[count - 1].width;
        else TRACELOG(LOG_WARNING, "GLFW: Failed to find video mode for selected monitor");
    }
    else TRACELOG(LOG_WARNING, "GLFW: Failed to find selected monitor");
#endif
    return 0;
}

// Get selected monitor width (max available by monitor)
int GetMonitorHeight(int monitor)
{
#if defined(PLATFORM_DESKTOP)
    int monitorCount;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

    if ((monitor >= 0) && (monitor < monitorCount))
    {
        int count = 0;
        const GLFWvidmode *modes = glfwGetVideoModes(monitors[monitor], &count);

        // We return the maximum resolution available, the last one in the modes array
        if (count > 0) return modes[count - 1].height;
        else TRACELOG(LOG_WARNING, "GLFW: Failed to find video mode for selected monitor");
    }
    else TRACELOG(LOG_WARNING, "GLFW: Failed to find selected monitor");
#endif
    return 0;
}

// Get selected monitor physical width in millimetres
int GetMonitorPhysicalWidth(int monitor)
{
#if defined(PLATFORM_DESKTOP)
    int monitorCount;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

    if ((monitor >= 0) && (monitor < monitorCount))
    {
        int physicalWidth;
        glfwGetMonitorPhysicalSize(monitors[monitor], &physicalWidth, NULL);
        return physicalWidth;
    }
    else TRACELOG(LOG_WARNING, "GLFW: Failed to find selected monitor");
#endif
    return 0;
}

// Get primary monitor physical height in millimetres
int GetMonitorPhysicalHeight(int monitor)
{
#if defined(PLATFORM_DESKTOP)
    int monitorCount;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

    if ((monitor >= 0) && (monitor < monitorCount))
    {
        int physicalHeight;
        glfwGetMonitorPhysicalSize(monitors[monitor], NULL, &physicalHeight);
        return physicalHeight;
    }
    else TRACELOG(LOG_WARNING, "GLFW: Failed to find selected monitor");
#endif
    return 0;
}

int GetMonitorRefreshRate(int monitor)
{
#if defined(PLATFORM_DESKTOP)
    int monitorCount;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

    if ((monitor >= 0) && (monitor < monitorCount))
    {
        const GLFWvidmode *vidmode = glfwGetVideoMode(monitors[monitor]);
        return vidmode->refreshRate;
    }
    else TRACELOG(LOG_WARNING, "GLFW: Failed to find selected monitor");
#endif
    return 0;
}

// Get window position XY on monitor
Vector2 GetWindowPosition(void)
{
    int x = 0;
    int y = 0;
#if defined(PLATFORM_DESKTOP)
    glfwGetWindowPos(CORE.Window.handle, &x, &y);
#endif
    return (Vector2){ (float)x, (float)y };
}

// Get window scale DPI factor
Vector2 GetWindowScaleDPI(void)
{
    Vector2 scale = { 1.0f, 1.0f };

#if defined(PLATFORM_DESKTOP)
    float xdpi = 1.0;
    float ydpi = 1.0;
    Vector2 windowPos = GetWindowPosition();

    int monitorCount = 0;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

    // Check window monitor
    for (int i = 0; i < monitorCount; i++)
    {
        glfwGetMonitorContentScale(monitors[i], &xdpi, &ydpi);

        int xpos, ypos, width, height;
        glfwGetMonitorWorkarea(monitors[i], &xpos, &ypos, &width, &height);

        if ((windowPos.x >= xpos) && (windowPos.x < xpos + width) &&
            (windowPos.y >= ypos) && (windowPos.y < ypos + height))
        {
            scale.x = xdpi;
            scale.y = ydpi;
            break;
        }
    }
#endif

    return scale;
}

// Get the human-readable, UTF-8 encoded name of the primary monitor
const char *GetMonitorName(int monitor)
{
#if defined(PLATFORM_DESKTOP)
    int monitorCount;
    GLFWmonitor **monitors = glfwGetMonitors(&monitorCount);

    if ((monitor >= 0) && (monitor < monitorCount))
    {
        return glfwGetMonitorName(monitors[monitor]);
    }
    else TRACELOG(LOG_WARNING, "GLFW: Failed to find selected monitor");
#endif
    return "";
}

// Get clipboard text content
// NOTE: returned string is allocated and freed by GLFW
const char *GetClipboardText(void)
{
#if defined(PLATFORM_DESKTOP)
    return glfwGetClipboardString(CORE.Window.handle);
#else
    return NULL;
#endif
}

// Set clipboard text content
void SetClipboardText(const char *text)
{
#if defined(PLATFORM_DESKTOP)
    glfwSetClipboardString(CORE.Window.handle, text);
#endif
}

// Show mouse cursor
void ShowCursor(void)
{
#if defined(PLATFORM_DESKTOP)
    glfwSetInputMode(CORE.Window.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#endif

    CORE.Input.Mouse.cursorHidden = false;
}

// Hides mouse cursor
void HideCursor(void)
{
#if defined(PLATFORM_DESKTOP)
    glfwSetInputMode(CORE.Window.handle, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
#endif

    CORE.Input.Mouse.cursorHidden = true;
}

// Check if cursor is not visible
bool IsCursorHidden(void)
{
    return CORE.Input.Mouse.cursorHidden;
}

// Enables cursor (unlock cursor)
void EnableCursor(void)
{
#if defined(PLATFORM_DESKTOP)
    glfwSetInputMode(CORE.Window.handle, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
#endif
#if defined(PLATFORM_WEB)
    emscripten_exit_pointerlock();
#endif

    CORE.Input.Mouse.cursorHidden = false;
}

// Disables cursor (lock cursor)
void DisableCursor(void)
{
#if defined(PLATFORM_DESKTOP)
    glfwSetInputMode(CORE.Window.handle, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
#endif
#if defined(PLATFORM_WEB)
    emscripten_request_pointerlock("#canvas", 1);
#endif

    CORE.Input.Mouse.cursorHidden = true;
}

// Check if cursor is on the current screen.
bool IsCursorOnScreen(void)
{
    return CORE.Input.Mouse.cursorOnScreen;
}

// Set background color (framebuffer clear color)
void ClearBackground(Color color)
{
    rlClearColor(color.r, color.g, color.b, color.a);   // Set clear color
    rlClearScreenBuffers();                             // Clear current framebuffers
}

// Setup canvas (framebuffer) to start drawing
void BeginDrawing(void)
{
    // WARNING: Previously to BeginDrawing() other render textures drawing could happen,
    // consequently the measure for update vs draw is not accurate (only the total frame time is accurate)

    CORE.Time.current = GetTime();      // Number of elapsed seconds since InitTimer()
    CORE.Time.update = CORE.Time.current - CORE.Time.previous;
    CORE.Time.previous = CORE.Time.current;

    rlLoadIdentity();                   // Reset current matrix (modelview)
    rlMultMatrixf(MatrixToFloat(CORE.Window.screenScale)); // Apply screen scaling

    //rlTranslatef(0.375, 0.375, 0);    // HACK to have 2D pixel-perfect drawing on OpenGL 1.1
                                        // NOTE: Not required with OpenGL 3.3+
}

// End canvas drawing and swap buffers (double buffering)
void EndDrawing(void)
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

#if defined(SUPPORT_MOUSE_CURSOR_POINT)
    // Draw a small rectangle on mouse position for user reference
    if (!CORE.Input.Mouse.cursorHidden)
    {
        DrawRectangle(CORE.Input.Mouse.currentPosition.x, CORE.Input.Mouse.currentPosition.y, 3, 3, MAROON);
        rlDrawRenderBatchActive();  // Update and draw internal render batch
    }
#endif

#if defined(SUPPORT_GIF_RECORDING)
    // Draw record indicator
    if (gifRecording)
    {
        #define GIF_RECORD_FRAMERATE    10
        gifFramesCounter++;

        // NOTE: We record one gif frame every 10 game frames
        if ((gifFramesCounter%GIF_RECORD_FRAMERATE) == 0)
        {
            // Get image data for the current frame (from backbuffer)
            // NOTE: This process is quite slow... :(
            unsigned char *screenData = rlReadScreenPixels(CORE.Window.screen.width, CORE.Window.screen.height);
            msf_gif_frame(&gifState, screenData, 10, 16, CORE.Window.screen.width*4);

            RL_FREE(screenData);    // Free image data
        }

        if (((gifFramesCounter/15)%2) == 1)
        {
            DrawCircle(30, CORE.Window.screen.height - 20, 10, MAROON);
            DrawText("GIF RECORDING", 50, CORE.Window.screen.height - 25, 10, RED);
        }

        rlDrawRenderBatchActive();  // Update and draw internal render batch
    }
#endif

#if !defined(SUPPORT_CUSTOM_FRAME_CONTROL)
    SwapScreenBuffer();                  // Copy back buffer to front buffer (screen)

    // Frame time control system
    CORE.Time.current = GetTime();
    CORE.Time.draw = CORE.Time.current - CORE.Time.previous;
    CORE.Time.previous = CORE.Time.current;

    CORE.Time.frame = CORE.Time.update + CORE.Time.draw;

    // Wait for some milliseconds...
    if (CORE.Time.frame < CORE.Time.target)
    {
        WaitTime((float)(CORE.Time.target - CORE.Time.frame)*1000.0f);

        CORE.Time.current = GetTime();
        double waitTime = CORE.Time.current - CORE.Time.previous;
        CORE.Time.previous = CORE.Time.current;

        CORE.Time.frame += waitTime;    // Total frame time: update + draw + wait
    }

    PollInputEvents();      // Poll user events (before next frame update)
#endif

    CORE.Time.frameCounter++;
}

// Initialize 2D mode with custom camera (2D)
void BeginMode2D(Camera2D camera)
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlLoadIdentity();               // Reset current matrix (modelview)

    // Apply 2d camera transformation to modelview
    rlMultMatrixf(MatrixToFloat(GetCameraMatrix2D(camera)));

    // Apply screen scaling if required
    rlMultMatrixf(MatrixToFloat(CORE.Window.screenScale));
}

// Ends 2D mode with custom camera
void EndMode2D(void)
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlLoadIdentity();               // Reset current matrix (modelview)
    rlMultMatrixf(MatrixToFloat(CORE.Window.screenScale)); // Apply screen scaling if required
}

// Initializes 3D mode with custom camera (3D)
void BeginMode3D(Camera3D camera)
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlMatrixMode(RL_PROJECTION);    // Switch to projection matrix
    rlPushMatrix();                 // Save previous matrix, which contains the settings for the 2d ortho projection
    rlLoadIdentity();               // Reset current matrix (projection)

    float aspect = (float)CORE.Window.currentFbo.width/(float)CORE.Window.currentFbo.height;

    // NOTE: zNear and zFar values are important when computing depth buffer values
    if (camera.projection == CAMERA_PERSPECTIVE)
    {
        // Setup perspective projection
        double top = RL_CULL_DISTANCE_NEAR*tan(camera.fovy*0.5*DEG2RAD);
        double right = top*aspect;

        rlFrustum(-right, right, -top, top, RL_CULL_DISTANCE_NEAR, RL_CULL_DISTANCE_FAR);
    }
    else if (camera.projection == CAMERA_ORTHOGRAPHIC)
    {
        // Setup orthographic projection
        double top = camera.fovy/2.0;
        double right = top*aspect;

        rlOrtho(-right, right, -top,top, RL_CULL_DISTANCE_NEAR, RL_CULL_DISTANCE_FAR);
    }

    rlMatrixMode(RL_MODELVIEW);     // Switch back to modelview matrix
    rlLoadIdentity();               // Reset current matrix (modelview)

    // Setup Camera view
    Matrix matView = MatrixLookAt(camera.position, camera.target, camera.up);
    rlMultMatrixf(MatrixToFloat(matView));      // Multiply modelview matrix by view matrix (camera)

    rlEnableDepthTest();            // Enable DEPTH_TEST for 3D
}

// Ends 3D mode and returns to default 2D orthographic mode
void EndMode3D(void)
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlMatrixMode(RL_PROJECTION);    // Switch to projection matrix
    rlPopMatrix();                  // Restore previous matrix (projection) from matrix stack

    rlMatrixMode(RL_MODELVIEW);     // Switch back to modelview matrix
    rlLoadIdentity();               // Reset current matrix (modelview)

    rlMultMatrixf(MatrixToFloat(CORE.Window.screenScale)); // Apply screen scaling if required

    rlDisableDepthTest();           // Disable DEPTH_TEST for 2D
}

// Initializes render texture for drawing
void BeginTextureMode(RenderTexture2D target)
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlEnableFramebuffer(target.id); // Enable render target

    // Set viewport to framebuffer size
    rlViewport(0, 0, target.texture.width, target.texture.height);

    rlMatrixMode(RL_PROJECTION);    // Switch to projection matrix
    rlLoadIdentity();               // Reset current matrix (projection)

    // Set orthographic projection to current framebuffer size
    // NOTE: Configured top-left corner as (0, 0)
    rlOrtho(0, target.texture.width, target.texture.height, 0, 0.0f, 1.0f);

    rlMatrixMode(RL_MODELVIEW);     // Switch back to modelview matrix
    rlLoadIdentity();               // Reset current matrix (modelview)

    //rlScalef(0.0f, -1.0f, 0.0f);  // Flip Y-drawing (?)

    // Setup current width/height for proper aspect ratio
    // calculation when using BeginMode3D()
    CORE.Window.currentFbo.width = target.texture.width;
    CORE.Window.currentFbo.height = target.texture.height;
}

// Ends drawing to render texture
void EndTextureMode(void)
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlDisableFramebuffer();         // Disable render target (fbo)

    // Set viewport to default framebuffer size
    SetupViewport(CORE.Window.render.width, CORE.Window.render.height);

    // Reset current fbo to screen size
    CORE.Window.currentFbo.width = CORE.Window.screen.width;
    CORE.Window.currentFbo.height = CORE.Window.screen.height;
}

// Begin custom shader mode
void BeginShaderMode(Shader shader)
{
    rlSetShader(shader);
}

// End custom shader mode (returns to default shader)
void EndShaderMode(void)
{
    rlSetShader(rlGetShaderDefault());
}

// Begin blending mode (alpha, additive, multiplied, subtract, custom)
// NOTE: Blend modes supported are enumerated in BlendMode enum
void BeginBlendMode(int mode)
{
    rlSetBlendMode(mode);
}

// End blending mode (reset to default: alpha blending)
void EndBlendMode(void)
{
    rlSetBlendMode(BLEND_ALPHA);
}

// Begin scissor mode (define screen area for following drawing)
// NOTE: Scissor rec refers to bottom-left corner, we change it to upper-left
void BeginScissorMode(int x, int y, int width, int height)
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch

    rlEnableScissorTest();
    rlScissor(x, CORE.Window.currentFbo.height - (y + height), width, height);
}

// End scissor mode
void EndScissorMode(void)
{
    rlDrawRenderBatchActive();      // Update and draw internal render batch
    rlDisableScissorTest();
}

// Begin VR drawing configuration
void BeginVrStereoMode(VrStereoConfig config)
{
    rlEnableStereoRender();

    // Set stereo render matrices
    rlSetMatrixProjectionStereo(config.projection[0], config.projection[1]);
    rlSetMatrixViewOffsetStereo(config.viewOffset[0], config.viewOffset[1]);
}

// End VR drawing process (and desktop mirror)
void EndVrStereoMode(void)
{
    rlDisableStereoRender();
}

// Load VR stereo config for VR simulator device parameters
VrStereoConfig LoadVrStereoConfig(VrDeviceInfo device)
{
    VrStereoConfig config = { 0 };

#if defined(GRAPHICS_API_OPENGL_33) || defined(GRAPHICS_API_OPENGL_ES2)
    // Compute aspect ratio
    float aspect = ((float)device.hResolution*0.5f)/(float)device.vResolution;

    // Compute lens parameters
    float lensShift = (device.hScreenSize*0.25f - device.lensSeparationDistance*0.5f)/device.hScreenSize;
    config.leftLensCenter[0] = 0.25f + lensShift;
    config.leftLensCenter[1] = 0.5f;
    config.rightLensCenter[0] = 0.75f - lensShift;
    config.rightLensCenter[1] = 0.5f;
    config.leftScreenCenter[0] = 0.25f;
    config.leftScreenCenter[1] = 0.5f;
    config.rightScreenCenter[0] = 0.75f;
    config.rightScreenCenter[1] = 0.5f;

    // Compute distortion scale parameters
    // NOTE: To get lens max radius, lensShift must be normalized to [-1..1]
    float lensRadius = fabsf(-1.0f - 4.0f*lensShift);
    float lensRadiusSq = lensRadius*lensRadius;
    float distortionScale = device.lensDistortionValues[0] +
                            device.lensDistortionValues[1]*lensRadiusSq +
                            device.lensDistortionValues[2]*lensRadiusSq*lensRadiusSq +
                            device.lensDistortionValues[3]*lensRadiusSq*lensRadiusSq*lensRadiusSq;

    float normScreenWidth = 0.5f;
    float normScreenHeight = 1.0f;
    config.scaleIn[0] = 2.0f/normScreenWidth;
    config.scaleIn[1] = 2.0f/normScreenHeight/aspect;
    config.scale[0] = normScreenWidth*0.5f/distortionScale;
    config.scale[1] = normScreenHeight*0.5f*aspect/distortionScale;

    // Fovy is normally computed with: 2*atan2f(device.vScreenSize, 2*device.eyeToScreenDistance)
    // ...but with lens distortion it is increased (see Oculus SDK Documentation)
    //float fovy = 2.0f*atan2f(device.vScreenSize*0.5f*distortionScale, device.eyeToScreenDistance);     // Really need distortionScale?
    float fovy = 2.0f*(float)atan2f(device.vScreenSize*0.5f, device.eyeToScreenDistance);

    // Compute camera projection matrices
    float projOffset = 4.0f*lensShift;      // Scaled to projection space coordinates [-1..1]
    Matrix proj = MatrixPerspective(fovy, aspect, RL_CULL_DISTANCE_NEAR, RL_CULL_DISTANCE_FAR);

    config.projection[0] = MatrixMultiply(proj, MatrixTranslate(projOffset, 0.0f, 0.0f));
    config.projection[1] = MatrixMultiply(proj, MatrixTranslate(-projOffset, 0.0f, 0.0f));

    // Compute camera transformation matrices
    // NOTE: Camera movement might seem more natural if we model the head.
    // Our axis of rotation is the base of our head, so we might want to add
    // some y (base of head to eye level) and -z (center of head to eye protrusion) to the camera positions.
    config.viewOffset[0] = MatrixTranslate(-device.interpupillaryDistance*0.5f, 0.075f, 0.045f);
    config.viewOffset[1] = MatrixTranslate(device.interpupillaryDistance*0.5f, 0.075f, 0.045f);

    // Compute eyes Viewports
    /*
    config.eyeViewportRight[0] = 0;
    config.eyeViewportRight[1] = 0;
    config.eyeViewportRight[2] = device.hResolution/2;
    config.eyeViewportRight[3] = device.vResolution;

    config.eyeViewportLeft[0] = device.hResolution/2;
    config.eyeViewportLeft[1] = 0;
    config.eyeViewportLeft[2] = device.hResolution/2;
    config.eyeViewportLeft[3] = device.vResolution;
    */
#else
    TRACELOG(LOG_WARNING, "RLGL: VR Simulator not supported on OpenGL 1.1");
#endif

    return config;
}

// Unload VR stereo config properties
void UnloadVrStereoConfig(VrStereoConfig config)
{
    //...
}

// Load shader from files and bind default locations
// NOTE: If shader string is NULL, using default vertex/fragment shaders
Shader LoadShader(const char *vsFileName, const char *fsFileName)
{
    Shader shader = { 0 };

    char *vShaderStr = LoadFileText(vsFileName);
    char *fShaderStr = LoadFileText(fsFileName);

    shader = LoadShaderFromMemory(vShaderStr, fShaderStr);

    UnloadFileText(vShaderStr);
    UnloadFileText(fShaderStr);

    return shader;
}

// Load shader from code strings and bind default locations
Shader LoadShaderFromMemory(const char *vsCode, const char *fsCode)
{
    Shader shader = { 0 };
    shader.locs = (int *)RL_CALLOC(MAX_SHADER_LOCATIONS, sizeof(int));

    // NOTE: All locations must be reseted to -1 (no location)
    for (int i = 0; i < MAX_SHADER_LOCATIONS; i++) shader.locs[i] = -1;

    shader.id = rlLoadShaderCode(vsCode, fsCode);

    // After shader loading, we TRY to set default location names
    if (shader.id > 0)
    {
        // Default shader attribute locations have been binded before linking:
        //          vertex position location    = 0
        //          vertex texcoord location    = 1
        //          vertex normal location      = 2
        //          vertex color location       = 3
        //          vertex tangent location     = 4
        //          vertex texcoord2 location   = 5

        // NOTE: If any location is not found, loc point becomes -1

        // Get handles to GLSL input attibute locations
        shader.locs[SHADER_LOC_VERTEX_POSITION] = rlGetLocationAttrib(shader.id, DEFAULT_SHADER_ATTRIB_NAME_POSITION);
        shader.locs[SHADER_LOC_VERTEX_TEXCOORD01] = rlGetLocationAttrib(shader.id, DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD);
        shader.locs[SHADER_LOC_VERTEX_TEXCOORD02] = rlGetLocationAttrib(shader.id, DEFAULT_SHADER_ATTRIB_NAME_TEXCOORD2);
        shader.locs[SHADER_LOC_VERTEX_NORMAL] = rlGetLocationAttrib(shader.id, DEFAULT_SHADER_ATTRIB_NAME_NORMAL);
        shader.locs[SHADER_LOC_VERTEX_TANGENT] = rlGetLocationAttrib(shader.id, DEFAULT_SHADER_ATTRIB_NAME_TANGENT);
        shader.locs[SHADER_LOC_VERTEX_COLOR] = rlGetLocationAttrib(shader.id, DEFAULT_SHADER_ATTRIB_NAME_COLOR);

        // Get handles to GLSL uniform locations (vertex shader)
        shader.locs[SHADER_LOC_MATRIX_MVP] = rlGetLocationUniform(shader.id, DEFAULT_SHADER_UNIFORM_NAME_MVP);
        shader.locs[SHADER_LOC_MATRIX_VIEW] = rlGetLocationUniform(shader.id, DEFAULT_SHADER_UNIFORM_NAME_VIEW);
        shader.locs[SHADER_LOC_MATRIX_PROJECTION] = rlGetLocationUniform(shader.id, DEFAULT_SHADER_UNIFORM_NAME_PROJECTION);
        shader.locs[SHADER_LOC_MATRIX_MODEL] = rlGetLocationUniform(shader.id, DEFAULT_SHADER_UNIFORM_NAME_MODEL);
        shader.locs[SHADER_LOC_MATRIX_NORMAL] = rlGetLocationUniform(shader.id, DEFAULT_SHADER_UNIFORM_NAME_NORMAL);

        // Get handles to GLSL uniform locations (fragment shader)
        shader.locs[SHADER_LOC_COLOR_DIFFUSE] = rlGetLocationUniform(shader.id, DEFAULT_SHADER_UNIFORM_NAME_COLOR);
        shader.locs[SHADER_LOC_MAP_DIFFUSE] = rlGetLocationUniform(shader.id, DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE0);  // SHADER_LOC_MAP_ALBEDO
        shader.locs[SHADER_LOC_MAP_SPECULAR] = rlGetLocationUniform(shader.id, DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE1); // SHADER_LOC_MAP_METALNESS
        shader.locs[SHADER_LOC_MAP_NORMAL] = rlGetLocationUniform(shader.id, DEFAULT_SHADER_SAMPLER2D_NAME_TEXTURE2);
    }

    return shader;
}

// Unload shader from GPU memory (VRAM)
void UnloadShader(Shader shader)
{
    if (shader.id != rlGetShaderDefault().id)
    {
        rlUnloadShaderProgram(shader.id);
        RL_FREE(shader.locs);
    }
}

// Get shader uniform location
int GetShaderLocation(Shader shader, const char *uniformName)
{
    return rlGetLocationUniform(shader.id, uniformName);
}

// Get shader attribute location
int GetShaderLocationAttrib(Shader shader, const char *attribName)
{
    return rlGetLocationAttrib(shader.id, attribName);
}

// Set shader uniform value
void SetShaderValue(Shader shader, int locIndex, const void *value, int uniformType)
{
    SetShaderValueV(shader, locIndex, value, uniformType, 1);
}

// Set shader uniform value vector
void SetShaderValueV(Shader shader, int locIndex, const void *value, int uniformType, int count)
{
    rlEnableShader(shader.id);
    rlSetUniform(locIndex, value, uniformType, count);
    //rlDisableShader();      // Avoid reseting current shader program, in case other uniforms are set
}

// Set shader uniform value (matrix 4x4)
void SetShaderValueMatrix(Shader shader, int locIndex, Matrix mat)
{
    rlEnableShader(shader.id);
    rlSetUniformMatrix(locIndex, mat);
    //rlDisableShader();
}

// Set shader uniform value for texture
void SetShaderValueTexture(Shader shader, int locIndex, Texture2D texture)
{
    rlEnableShader(shader.id);
    rlSetUniformSampler(locIndex, texture.id);
    //rlDisableShader();
}

// Get a ray trace from mouse position
Ray GetMouseRay(Vector2 mouse, Camera camera)
{
    Ray ray = { 0 };

    // Calculate normalized device coordinates
    // NOTE: y value is negative
    float x = (2.0f*mouse.x)/(float)GetScreenWidth() - 1.0f;
    float y = 1.0f - (2.0f*mouse.y)/(float)GetScreenHeight();
    float z = 1.0f;

    // Store values in a vector
    Vector3 deviceCoords = { x, y, z };

    // Calculate view matrix from camera look at
    Matrix matView = MatrixLookAt(camera.position, camera.target, camera.up);

    Matrix matProj = MatrixIdentity();

    if (camera.projection == CAMERA_PERSPECTIVE)
    {
        // Calculate projection matrix from perspective
        matProj = MatrixPerspective(camera.fovy*DEG2RAD, ((double)GetScreenWidth()/(double)GetScreenHeight()), RL_CULL_DISTANCE_NEAR, RL_CULL_DISTANCE_FAR);
    }
    else if (camera.projection == CAMERA_ORTHOGRAPHIC)
    {
        float aspect = (float)CORE.Window.screen.width/(float)CORE.Window.screen.height;
        double top = camera.fovy/2.0;
        double right = top*aspect;

        // Calculate projection matrix from orthographic
        matProj = MatrixOrtho(-right, right, -top, top, 0.01, 1000.0);
    }

    // Unproject far/near points
    Vector3 nearPoint = Vector3Unproject((Vector3){ deviceCoords.x, deviceCoords.y, 0.0f }, matProj, matView);
    Vector3 farPoint = Vector3Unproject((Vector3){ deviceCoords.x, deviceCoords.y, 1.0f }, matProj, matView);

    // Unproject the mouse cursor in the near plane.
    // We need this as the source position because orthographic projects, compared to perspect doesn't have a
    // convergence point, meaning that the "eye" of the camera is more like a plane than a point.
    Vector3 cameraPlanePointerPos = Vector3Unproject((Vector3){ deviceCoords.x, deviceCoords.y, -1.0f }, matProj, matView);

    // Calculate normalized direction vector
    Vector3 direction = Vector3Normalize(Vector3Subtract(farPoint, nearPoint));

    if (camera.projection == CAMERA_PERSPECTIVE) ray.position = camera.position;
    else if (camera.projection == CAMERA_ORTHOGRAPHIC) ray.position = cameraPlanePointerPos;

    // Apply calculated vectors to ray
    ray.direction = direction;

    return ray;
}

// Get transform matrix for camera
Matrix GetCameraMatrix(Camera camera)
{
    return MatrixLookAt(camera.position, camera.target, camera.up);
}

// Get camera 2d transform matrix
Matrix GetCameraMatrix2D(Camera2D camera)
{
    Matrix matTransform = { 0 };
    // The camera in world-space is set by
    //   1. Move it to target
    //   2. Rotate by -rotation and scale by (1/zoom)
    //      When setting higher scale, it's more intuitive for the world to become bigger (= camera become smaller),
    //      not for the camera getting bigger, hence the invert. Same deal with rotation.
    //   3. Move it by (-offset);
    //      Offset defines target transform relative to screen, but since we're effectively "moving" screen (camera)
    //      we need to do it into opposite direction (inverse transform)

    // Having camera transform in world-space, inverse of it gives the modelview transform.
    // Since (A*B*C)' = C'*B'*A', the modelview is
    //   1. Move to offset
    //   2. Rotate and Scale
    //   3. Move by -target
    Matrix matOrigin = MatrixTranslate(-camera.target.x, -camera.target.y, 0.0f);
    Matrix matRotation = MatrixRotate((Vector3){ 0.0f, 0.0f, 1.0f }, camera.rotation*DEG2RAD);
    Matrix matScale = MatrixScale(camera.zoom, camera.zoom, 1.0f);
    Matrix matTranslation = MatrixTranslate(camera.offset.x, camera.offset.y, 0.0f);

    matTransform = MatrixMultiply(MatrixMultiply(matOrigin, MatrixMultiply(matScale, matRotation)), matTranslation);

    return matTransform;
}

// Get the screen space position from a 3d world space position
Vector2 GetWorldToScreen(Vector3 position, Camera camera)
{
    Vector2 screenPosition = GetWorldToScreenEx(position, camera, GetScreenWidth(), GetScreenHeight());

    return screenPosition;
}

// Get size position for a 3d world space position (useful for texture drawing)
Vector2 GetWorldToScreenEx(Vector3 position, Camera camera, int width, int height)
{
    // Calculate projection matrix (from perspective instead of frustum
    Matrix matProj = MatrixIdentity();

    if (camera.projection == CAMERA_PERSPECTIVE)
    {
        // Calculate projection matrix from perspective
        matProj = MatrixPerspective(camera.fovy*DEG2RAD, ((double)width/(double)height), RL_CULL_DISTANCE_NEAR, RL_CULL_DISTANCE_FAR);
    }
    else if (camera.projection == CAMERA_ORTHOGRAPHIC)
    {
        float aspect = (float)CORE.Window.screen.width/(float)CORE.Window.screen.height;
        double top = camera.fovy/2.0;
        double right = top*aspect;

        // Calculate projection matrix from orthographic
        matProj = MatrixOrtho(-right, right, -top, top, RL_CULL_DISTANCE_NEAR, RL_CULL_DISTANCE_FAR);
    }

    // Calculate view matrix from camera look at (and transpose it)
    Matrix matView = MatrixLookAt(camera.position, camera.target, camera.up);

    // Convert world position vector to quaternion
    Quaternion worldPos = { position.x, position.y, position.z, 1.0f };

    // Transform world position to view
    worldPos = QuaternionTransform(worldPos, matView);

    // Transform result to projection (clip space position)
    worldPos = QuaternionTransform(worldPos, matProj);

    // Calculate normalized device coordinates (inverted y)
    Vector3 ndcPos = { worldPos.x/worldPos.w, -worldPos.y/worldPos.w, worldPos.z/worldPos.w };

    // Calculate 2d screen position vector
    Vector2 screenPosition = { (ndcPos.x + 1.0f)/2.0f*(float)width, (ndcPos.y + 1.0f)/2.0f*(float)height };

    return screenPosition;
}

// Get the screen space position for a 2d camera world space position
Vector2 GetWorldToScreen2D(Vector2 position, Camera2D camera)
{
    Matrix matCamera = GetCameraMatrix2D(camera);
    Vector3 transform = Vector3Transform((Vector3){ position.x, position.y, 0 }, matCamera);

    return (Vector2){ transform.x, transform.y };
}

// Get the world space position for a 2d camera screen space position
Vector2 GetScreenToWorld2D(Vector2 position, Camera2D camera)
{
    Matrix invMatCamera = MatrixInvert(GetCameraMatrix2D(camera));
    Vector3 transform = Vector3Transform((Vector3){ position.x, position.y, 0 }, invMatCamera);

    return (Vector2){ transform.x, transform.y };
}

// Set target FPS (maximum)
void SetTargetFPS(int fps)
{
    if (fps < 1) CORE.Time.target = 0.0;
    else CORE.Time.target = 1.0/(double)fps;

    TRACELOG(LOG_INFO, "TIMER: Target time per frame: %02.03f milliseconds", (float)CORE.Time.target*1000);
}

// Get current FPS
// NOTE: We calculate an average framerate
int GetFPS(void)
{
    int fps = 0;

#if !defined(SUPPORT_CUSTOM_FRAME_CONTROL)
    #define FPS_CAPTURE_FRAMES_COUNT    30      // 30 captures
    #define FPS_AVERAGE_TIME_SECONDS   0.5f     // 500 millisecondes
    #define FPS_STEP (FPS_AVERAGE_TIME_SECONDS/FPS_CAPTURE_FRAMES_COUNT)

    static int index = 0;
    static float history[FPS_CAPTURE_FRAMES_COUNT] = { 0 };
    static float average = 0, last = 0;
    float fpsFrame = GetFrameTime();

    if (fpsFrame == 0) return 0;

    if ((GetTime() - last) > FPS_STEP)
    {
        last = (float)GetTime();
        index = (index + 1)%FPS_CAPTURE_FRAMES_COUNT;
        average -= history[index];
        history[index] = fpsFrame/FPS_CAPTURE_FRAMES_COUNT;
        average += history[index];
    }

    fps = (int)roundf(1.0f/average);
#endif

    return fps;
}

// Get time in seconds for last frame drawn (delta time)
float GetFrameTime(void)
{
    return (float)CORE.Time.frame;
}

// Get elapsed time measure in seconds since InitTimer()
// NOTE: On PLATFORM_DESKTOP InitTimer() is called on InitWindow()
// NOTE: On PLATFORM_DESKTOP, timer is initialized on glfwInit()
double GetTime(void)
{
#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
    return glfwGetTime();   // Elapsed time since glfwInit()
#endif

#if defined(PLATFORM_ANDROID)
    struct timespec ts = { 0 };
    clock_gettime(CLOCK_MONOTONIC, &ts);
    unsigned long long int time = (unsigned long long int)ts.tv_sec*1000000000LLU + (unsigned long long int)ts.tv_nsec;

    return (double)(time - CORE.Time.base)*1e-9;  // Elapsed time since InitTimer()
#endif
}

// Setup window configuration flags (view FLAGS)
// NOTE: This function is expected to be called before window creation,
// because it setups some flags for the window creation process.
// To configure window states after creation, just use SetWindowState()
void SetConfigFlags(unsigned int flags)
{
    // Selected flags are set but not evaluated at this point,
    // flag evaluation happens at InitWindow() or SetWindowState()
    CORE.Window.flags |= flags;
}

// NOTE TRACELOG() function is located in [utils.h]

// Takes a screenshot of current screen (saved a .png)
void TakeScreenshot(const char *fileName)
{
    unsigned char *imgData = rlReadScreenPixels(CORE.Window.render.width, CORE.Window.render.height);
    Image image = { imgData, CORE.Window.render.width, CORE.Window.render.height, 1, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8 };

    char path[512] = { 0 };
    strcpy(path, TextFormat("%s/%s", CORE.Storage.basePath, fileName));

    ExportImage(image, path);
    RL_FREE(imgData);

#if defined(PLATFORM_WEB)
    // Download file from MEMFS (emscripten memory filesystem)
    // saveFileFromMEMFSToDisk() function is defined in raylib/src/shell.html
    emscripten_run_script(TextFormat("saveFileFromMEMFSToDisk('%s','%s')", GetFileName(path), GetFileName(path)));
#endif

    // TODO: Verification required for log
    TRACELOG(LOG_INFO, "SYSTEM: [%s] Screenshot taken successfully", path);
}

// Get a random value between min and max (both included)
int GetRandomValue(int min, int max)
{
    if (min > max)
    {
        int tmp = max;
        max = min;
        min = tmp;
    }

    return (rand()%(abs(max - min) + 1) + min);
}

// Check if the file exists
bool FileExists(const char *fileName)
{
    bool result = false;

#if defined(_WIN32)
    if (_access(fileName, 0) != -1) result = true;
#else
    if (access(fileName, F_OK) != -1) result = true;
#endif

    return result;
}

// Check file extension
// NOTE: Extensions checking is not case-sensitive
bool IsFileExtension(const char *fileName, const char *ext)
{
    bool result = false;
    const char *fileExt = GetFileExtension(fileName);

    if (fileExt != NULL)
    {
#if defined(SUPPORT_TEXT_MANIPULATION)
        int extCount = 0;
        const char **checkExts = TextSplit(ext, ';', &extCount);

        char fileExtLower[16] = { 0 };
        strcpy(fileExtLower, TextToLower(fileExt));

        for (int i = 0; i < extCount; i++)
        {
            if (TextIsEqual(fileExtLower, TextToLower(checkExts[i])))
            {
                result = true;
                break;
            }
        }
#else
        if (strcmp(fileExt, ext) == 0) result = true;
#endif
    }

    return result;
}

// Check if a directory path exists
bool DirectoryExists(const char *dirPath)
{
    bool result = false;
    DIR *dir = opendir(dirPath);

    if (dir != NULL)
    {
        result = true;
        closedir(dir);
    }

    return result;
}

// Get pointer to extension for a filename string (includes the dot: .png)
const char *GetFileExtension(const char *fileName)
{
    const char *dot = strrchr(fileName, '.');

    if (!dot || dot == fileName) return NULL;

    return dot;
}

// String pointer reverse break: returns right-most occurrence of charset in s
static const char *strprbrk(const char *s, const char *charset)
{
    const char *latestMatch = NULL;
    for (; s = strpbrk(s, charset), s != NULL; latestMatch = s++) { }
    return latestMatch;
}

// Get pointer to filename for a path string
const char *GetFileName(const char *filePath)
{
    const char *fileName = NULL;
    if (filePath != NULL) fileName = strprbrk(filePath, "\\/");

    if (!fileName) return filePath;

    return fileName + 1;
}

// Get filename string without extension (uses static string)
const char *GetFileNameWithoutExt(const char *filePath)
{
    #define MAX_FILENAMEWITHOUTEXT_LENGTH   128

    static char fileName[MAX_FILENAMEWITHOUTEXT_LENGTH] = { 0 };
    memset(fileName, 0, MAX_FILENAMEWITHOUTEXT_LENGTH);

    if (filePath != NULL) strcpy(fileName, GetFileName(filePath));   // Get filename with extension

    int len = (int)strlen(fileName);

    for (int i = 0; (i < len) && (i < MAX_FILENAMEWITHOUTEXT_LENGTH); i++)
    {
        if (fileName[i] == '.')
        {
            // NOTE: We break on first '.' found
            fileName[i] = '\0';
            break;
        }
    }

    return fileName;
}

// Get directory for a given filePath
const char *GetDirectoryPath(const char *filePath)
{
/*
    // NOTE: Directory separator is different in Windows and other platforms,
    // fortunately, Windows also support the '/' separator, that's the one should be used
    #if defined(_WIN32)
        char separator = '\\';
    #else
        char separator = '/';
    #endif
*/
    const char *lastSlash = NULL;
    static char dirPath[MAX_FILEPATH_LENGTH] = { 0 };
    memset(dirPath, 0, MAX_FILEPATH_LENGTH);

    // In case provided path does not contain a root drive letter (C:\, D:\) nor leading path separator (\, /),
    // we add the current directory path to dirPath
    if (filePath[1] != ':' && filePath[0] != '\\' && filePath[0] != '/')
    {
        // For security, we set starting path to current directory,
        // obtained path will be concated to this
        dirPath[0] = '.';
        dirPath[1] = '/';
    }

    lastSlash = strprbrk(filePath, "\\/");
    if (lastSlash)
    {
        if (lastSlash == filePath)
        {
            // The last and only slash is the leading one: path is in a root directory
            dirPath[0] = filePath[0];
            dirPath[1] = '\0';
        }
        else
        {
            // NOTE: Be careful, strncpy() is not safe, it does not care about '\0'
            memcpy(dirPath + (filePath[1] != ':' && filePath[0] != '\\' && filePath[0] != '/' ? 2 : 0), filePath, strlen(filePath) - (strlen(lastSlash) - 1));
            dirPath[strlen(filePath) - strlen(lastSlash) + (filePath[1] != ':' && filePath[0] != '\\' && filePath[0] != '/' ? 2 : 0)] = '\0';  // Add '\0' manually
        }
    }

    return dirPath;
}

// Get previous directory path for a given path
const char *GetPrevDirectoryPath(const char *dirPath)
{
    static char prevDirPath[MAX_FILEPATH_LENGTH] = { 0 };
    memset(prevDirPath, 0, MAX_FILEPATH_LENGTH);
    int pathLen = (int)strlen(dirPath);

    if (pathLen <= 3) strcpy(prevDirPath, dirPath);

    for (int i = (pathLen - 1); (i >= 0) && (pathLen > 3); i--)
    {
        if ((dirPath[i] == '\\') || (dirPath[i] == '/'))
        {
            // Check for root: "C:\" or "/"
            if (((i == 2) && (dirPath[1] ==':')) || (i == 0)) i++;

            strncpy(prevDirPath, dirPath, i);
            break;
        }
    }

    return prevDirPath;
}

// Get current working directory
const char *GetWorkingDirectory(void)
{
    static char currentDir[MAX_FILEPATH_LENGTH] = { 0 };
    memset(currentDir, 0, MAX_FILEPATH_LENGTH);

    char *path = GETCWD(currentDir, MAX_FILEPATH_LENGTH - 1);

    return path;
}

// Get filenames in a directory path (max 512 files)
// NOTE: Files count is returned by parameters pointer
char **GetDirectoryFiles(const char *dirPath, int *fileCount)
{
    #define MAX_DIRECTORY_FILES     512

    ClearDirectoryFiles();

    // Memory allocation for MAX_DIRECTORY_FILES
    dirFilesPath = (char **)RL_MALLOC(MAX_DIRECTORY_FILES*sizeof(char *));
    for (int i = 0; i < MAX_DIRECTORY_FILES; i++) dirFilesPath[i] = (char *)RL_MALLOC(MAX_FILEPATH_LENGTH*sizeof(char));

    int counter = 0;
    struct dirent *entity;
    DIR *dir = opendir(dirPath);

    if (dir != NULL)  // It's a directory
    {
        // TODO: Reading could be done in two passes,
        // first one to count files and second one to read names
        // That way we can allocate required memory, instead of a limited pool

        while ((entity = readdir(dir)) != NULL)
        {
            strcpy(dirFilesPath[counter], entity->d_name);
            counter++;
        }

        closedir(dir);
    }
    else TRACELOG(LOG_WARNING, "FILEIO: Failed to open requested directory");  // Maybe it's a file...

    dirFilesCount = counter;
    *fileCount = dirFilesCount;

    return dirFilesPath;
}

// Clear directory files paths buffers
void ClearDirectoryFiles(void)
{
    if (dirFilesCount > 0)
    {
        for (int i = 0; i < MAX_DIRECTORY_FILES; i++) RL_FREE(dirFilesPath[i]);

        RL_FREE(dirFilesPath);
    }

    dirFilesCount = 0;
}

// Change working directory, returns true on success
bool ChangeDirectory(const char *dir)
{
    bool result = CHDIR(dir);

    if (result != 0) TRACELOG(LOG_WARNING, "SYSTEM: Failed to change to directory: %s", dir);

    return (result == 0);
}

// Check if a file has been dropped into window
bool IsFileDropped(void)
{
    if (CORE.Window.dropFilesCount > 0) return true;
    else return false;
}

// Get dropped files names
char **GetDroppedFiles(int *count)
{
    *count = CORE.Window.dropFilesCount;
    return CORE.Window.dropFilesPath;
}

// Clear dropped files paths buffer
void ClearDroppedFiles(void)
{
    if (CORE.Window.dropFilesCount > 0)
    {
        for (int i = 0; i < CORE.Window.dropFilesCount; i++) RL_FREE(CORE.Window.dropFilesPath[i]);

        RL_FREE(CORE.Window.dropFilesPath);

        CORE.Window.dropFilesCount = 0;
    }
}

// Get file modification time (last write time)
long GetFileModTime(const char *fileName)
{
    struct stat result = { 0 };

    if (stat(fileName, &result) == 0)
    {
        time_t mod = result.st_mtime;

        return (long)mod;
    }

    return 0;
}

// Compress data (DEFLATE algorythm)
unsigned char *CompressData(unsigned char *data, int dataLength, int *compDataLength)
{
    #define COMPRESSION_QUALITY_DEFLATE  8

    unsigned char *compData = NULL;

#if defined(SUPPORT_COMPRESSION_API)
    // Compress data and generate a valid DEFLATE stream
    struct sdefl sdefl = { 0 };
    int bounds = sdefl_bound(dataLength);
    compData = (unsigned char *)RL_CALLOC(bounds, 1);
    *compDataLength = sdeflate(&sdefl, compData, data, dataLength, COMPRESSION_QUALITY_DEFLATE);   // Compression level 8, same as stbwi

    TraceLog(LOG_INFO, "SYSTEM: Compress data: Original size: %i -> Comp. size: %i", dataLength, *compDataLength);
#endif

    return compData;
}

// Decompress data (DEFLATE algorythm)
unsigned char *DecompressData(unsigned char *compData, int compDataLength, int *dataLength)
{
    unsigned char *data = NULL;

#if defined(SUPPORT_COMPRESSION_API)
    // Decompress data from a valid DEFLATE stream
    data = RL_CALLOC(MAX_DECOMPRESSION_SIZE*1024*1024, 1);
    int length = sinflate(data, compData, compDataLength);
    unsigned char *temp = RL_REALLOC(data, length);

    if (temp != NULL) data = temp;
    else TRACELOG(LOG_WARNING, "SYSTEM: Failed to re-allocate required decompression memory");

    *dataLength = length;

    TraceLog(LOG_INFO, "SYSTEM: Decompress data: Comp. size: %i -> Original size: %i", compDataLength, *dataLength);
#endif

    return data;
}

// Save integer value to storage file (to defined position)
// NOTE: Storage positions is directly related to file memory layout (4 bytes each integer)
bool SaveStorageValue(unsigned int position, int value)
{
    bool success = false;

#if defined(SUPPORT_DATA_STORAGE)
    char path[512] = { 0 };
    strcpy(path, TextFormat("%s/%s", CORE.Storage.basePath, STORAGE_DATA_FILE));

    unsigned int dataSize = 0;
    unsigned int newDataSize = 0;
    unsigned char *fileData = LoadFileData(path, &dataSize);
    unsigned char *newFileData = NULL;

    if (fileData != NULL)
    {
        if (dataSize <= (position*sizeof(int)))
        {
            // Increase data size up to position and store value
            newDataSize = (position + 1)*sizeof(int);
            newFileData = (unsigned char *)RL_REALLOC(fileData, newDataSize);

            if (newFileData != NULL)
            {
                // RL_REALLOC succeded
                int *dataPtr = (int *)newFileData;
                dataPtr[position] = value;
            }
            else
            {
                // RL_REALLOC failed
                TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to realloc data (%u), position in bytes (%u) bigger than actual file size", path, dataSize, position*sizeof(int));

                // We store the old size of the file
                newFileData = fileData;
                newDataSize = dataSize;
            }
        }
        else
        {
            // Store the old size of the file
            newFileData = fileData;
            newDataSize = dataSize;

            // Replace value on selected position
            int *dataPtr = (int *)newFileData;
            dataPtr[position] = value;
        }

        success = SaveFileData(path, newFileData, newDataSize);
        RL_FREE(newFileData);

        TRACELOG(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", path, value);
    }
    else
    {
        TRACELOG(LOG_INFO, "FILEIO: [%s] File created successfully", path);

        dataSize = (position + 1)*sizeof(int);
        fileData = (unsigned char *)RL_MALLOC(dataSize);
        int *dataPtr = (int *)fileData;
        dataPtr[position] = value;

        success = SaveFileData(path, fileData, dataSize);
        UnloadFileData(fileData);

        TRACELOG(LOG_INFO, "FILEIO: [%s] Saved storage value: %i", path, value);
    }
#endif

    return success;
}

// Load integer value from storage file (from defined position)
// NOTE: If requested position could not be found, value 0 is returned
int LoadStorageValue(unsigned int position)
{
    int value = 0;

#if defined(SUPPORT_DATA_STORAGE)
    char path[512] = { 0 };
    strcpy(path, TextFormat("%s/%s", CORE.Storage.basePath, STORAGE_DATA_FILE));

    unsigned int dataSize = 0;
    unsigned char *fileData = LoadFileData(path, &dataSize);

    if (fileData != NULL)
    {
        if (dataSize < (position*4)) TRACELOG(LOG_WARNING, "FILEIO: [%s] Failed to find storage position: %i", path, position);
        else
        {
            int *dataPtr = (int *)fileData;
            value = dataPtr[position];
        }

        UnloadFileData(fileData);

        TRACELOG(LOG_INFO, "FILEIO: [%s] Loaded storage value: %i", path, value);
    }
#endif
    return value;
}

// Open URL with default system browser (if available)
// NOTE: This function is only safe to use if you control the URL given.
// A user could craft a malicious string performing another action.
// Only call this function yourself not with user input or make sure to check the string yourself.
// Ref: https://github.com/raysan5/raylib/issues/686
void OpenURL(const char *url)
{
    // Small security check trying to avoid (partially) malicious code...
    // sorry for the inconvenience when you hit this point...
    if (strchr(url, '\'') != NULL)
    {
        TRACELOG(LOG_WARNING, "SYSTEM: Provided URL is not valid");
    }
    else
    {
#if defined(PLATFORM_DESKTOP)
        char *cmd = (char *)RL_CALLOC(strlen(url) + 10, sizeof(char));
    #if defined(_WIN32)
        sprintf(cmd, "explorer %s", url);
    #endif
    #if defined(__linux__) || defined(__FreeBSD__)
        sprintf(cmd, "xdg-open '%s'", url); // Alternatives: firefox, x-www-browser
    #endif
    #if defined(__APPLE__)
        sprintf(cmd, "open '%s'", url);
    #endif
        system(cmd);
        RL_FREE(cmd);
#endif
#if defined(PLATFORM_WEB)
        emscripten_run_script(TextFormat("window.open('%s', '_blank')", url));
#endif
    }
}

//----------------------------------------------------------------------------------
// Module Functions Definition - Input (Keyboard, Mouse, Gamepad) Functions
//----------------------------------------------------------------------------------
// Check if a key has been pressed once
bool IsKeyPressed(int key)
{
    bool pressed = false;

    if ((CORE.Input.Keyboard.previousKeyState[key] == 0) && (CORE.Input.Keyboard.currentKeyState[key] == 1)) pressed = true;
    else pressed = false;

    return pressed;
}

// Check if a key is being pressed (key held down)
bool IsKeyDown(int key)
{
    if (CORE.Input.Keyboard.currentKeyState[key] == 1) return true;
    else return false;
}

// Check if a key has been released once
bool IsKeyReleased(int key)
{
    bool released = false;

    if ((CORE.Input.Keyboard.previousKeyState[key] == 1) && (CORE.Input.Keyboard.currentKeyState[key] == 0)) released = true;
    else released = false;

    return released;
}

// Check if a key is NOT being pressed (key not held down)
bool IsKeyUp(int key)
{
    if (CORE.Input.Keyboard.currentKeyState[key] == 0) return true;
    else return false;
}

// Get the last key pressed
int GetKeyPressed(void)
{
    int value = 0;

    if (CORE.Input.Keyboard.keyPressedQueueCount > 0)
    {
        // Get character from the queue head
        value = CORE.Input.Keyboard.keyPressedQueue[0];

        // Shift elements 1 step toward the head.
        for (int i = 0; i < (CORE.Input.Keyboard.keyPressedQueueCount - 1); i++)
            CORE.Input.Keyboard.keyPressedQueue[i] = CORE.Input.Keyboard.keyPressedQueue[i + 1];

        // Reset last character in the queue
        CORE.Input.Keyboard.keyPressedQueue[CORE.Input.Keyboard.keyPressedQueueCount] = 0;
        CORE.Input.Keyboard.keyPressedQueueCount--;
    }

    return value;
}

// Get the last char pressed
int GetCharPressed(void)
{
    int value = 0;

    if (CORE.Input.Keyboard.charPressedQueueCount > 0)
    {
        // Get character from the queue head
        value = CORE.Input.Keyboard.charPressedQueue[0];

        // Shift elements 1 step toward the head.
        for (int i = 0; i < (CORE.Input.Keyboard.charPressedQueueCount - 1); i++)
            CORE.Input.Keyboard.charPressedQueue[i] = CORE.Input.Keyboard.charPressedQueue[i + 1];

        // Reset last character in the queue
        CORE.Input.Keyboard.charPressedQueue[CORE.Input.Keyboard.charPressedQueueCount] = 0;
        CORE.Input.Keyboard.charPressedQueueCount--;
    }

    return value;
}

// NOTE: Gamepad support not implemented in emscripten GLFW3 (PLATFORM_WEB)

// Check if a gamepad is available
bool IsGamepadAvailable(int gamepad)
{
    bool result = false;

    if ((gamepad < MAX_GAMEPADS) && CORE.Input.Gamepad.ready[gamepad]) result = true;

    return result;
}

// Check gamepad name (if available)
bool IsGamepadName(int gamepad, const char *name)
{
    bool result = false;
    const char *currentName = NULL;

    if (CORE.Input.Gamepad.ready[gamepad]) currentName = GetGamepadName(gamepad);
    if ((name != NULL) && (currentName != NULL)) result = (strcmp(name, currentName) == 0);

    return result;
}

// Get gamepad internal name id
const char *GetGamepadName(int gamepad)
{
#if defined(PLATFORM_DESKTOP)
    if (CORE.Input.Gamepad.ready[gamepad]) return glfwGetJoystickName(gamepad);
    else return NULL;
#endif
    return NULL;
}

// Get gamepad axis count
int GetGamepadAxisCount(int gamepad)
{
    return CORE.Input.Gamepad.axisCount;
}

// Get axis movement vector for a gamepad
float GetGamepadAxisMovement(int gamepad, int axis)
{
    float value = 0;

    if ((gamepad < MAX_GAMEPADS) && CORE.Input.Gamepad.ready[gamepad] && (axis < MAX_GAMEPAD_AXIS) &&
        (fabsf(CORE.Input.Gamepad.axisState[gamepad][axis]) > 0.1f)) value = CORE.Input.Gamepad.axisState[gamepad][axis];      // 0.1f = GAMEPAD_AXIS_MINIMUM_DRIFT/DELTA

    return value;
}

// Check if a gamepad button has been pressed once
bool IsGamepadButtonPressed(int gamepad, int button)
{
    bool pressed = false;

    if ((gamepad < MAX_GAMEPADS) && CORE.Input.Gamepad.ready[gamepad] && (button < MAX_GAMEPAD_BUTTONS) &&
        (CORE.Input.Gamepad.previousButtonState[gamepad][button] == 0) && (CORE.Input.Gamepad.currentButtonState[gamepad][button] == 1)) pressed = true;
    else pressed = false;

    return pressed;
}

// Check if a gamepad button is being pressed
bool IsGamepadButtonDown(int gamepad, int button)
{
    bool result = false;

    if ((gamepad < MAX_GAMEPADS) && CORE.Input.Gamepad.ready[gamepad] && (button < MAX_GAMEPAD_BUTTONS) &&
        (CORE.Input.Gamepad.currentButtonState[gamepad][button] == 1)) result = true;

    return result;
}

// Check if a gamepad button has NOT been pressed once
bool IsGamepadButtonReleased(int gamepad, int button)
{
    bool released = false;

    if ((gamepad < MAX_GAMEPADS) && CORE.Input.Gamepad.ready[gamepad] && (button < MAX_GAMEPAD_BUTTONS) &&
        (CORE.Input.Gamepad.previousButtonState[gamepad][button] == 1) && (CORE.Input.Gamepad.currentButtonState[gamepad][button] == 0)) released = true;
    else released = false;

    return released;
}

// Check if a gamepad button is NOT being pressed
bool IsGamepadButtonUp(int gamepad, int button)
{
    bool result = false;

    if ((gamepad < MAX_GAMEPADS) && CORE.Input.Gamepad.ready[gamepad] && (button < MAX_GAMEPAD_BUTTONS) &&
        (CORE.Input.Gamepad.currentButtonState[gamepad][button] == 0)) result = true;

    return result;
}

// Get the last gamepad button pressed
int GetGamepadButtonPressed(void)
{
    return CORE.Input.Gamepad.lastButtonPressed;
}

// Set internal gamepad mappings
int SetGamepadMappings(const char *mappings)
{
    int result = 0;

#if defined(PLATFORM_DESKTOP)
    result = glfwUpdateGamepadMappings(mappings);
#endif

    return result;
}

// Check if a mouse button has been pressed once
bool IsMouseButtonPressed(int button)
{
    bool pressed = false;

    if ((CORE.Input.Mouse.currentButtonState[button] == 1) && (CORE.Input.Mouse.previousButtonState[button] == 0)) pressed = true;

    // Map touches to mouse buttons checking
    if ((CORE.Input.Touch.currentTouchState[button] == 1) && (CORE.Input.Touch.previousTouchState[button] == 0)) pressed = true;

    return pressed;
}

// Check if a mouse button is being pressed
bool IsMouseButtonDown(int button)
{
    bool down = false;

    if (CORE.Input.Mouse.currentButtonState[button] == 1) down = true;

    // Map touches to mouse buttons checking
    if (CORE.Input.Touch.currentTouchState[button] == 1) down = true;

    return down;
}

// Check if a mouse button has been released once
bool IsMouseButtonReleased(int button)
{
    bool released = false;

    if ((CORE.Input.Mouse.currentButtonState[button] == 0) && (CORE.Input.Mouse.previousButtonState[button] == 1)) released = true;

    // Map touches to mouse buttons checking
    if ((CORE.Input.Touch.currentTouchState[button] == 0) && (CORE.Input.Touch.previousTouchState[button] == 1)) released = true;

    return released;
}

// Check if a mouse button is NOT being pressed
bool IsMouseButtonUp(int button)
{
    return !IsMouseButtonDown(button);
}

// Get mouse position X
int GetMouseX(void)
{
#if defined(PLATFORM_ANDROID)
    return (int)CORE.Input.Touch.position[0].x;
#else
    return (int)((CORE.Input.Mouse.currentPosition.x + CORE.Input.Mouse.offset.x)*CORE.Input.Mouse.scale.x);
#endif
}

// Get mouse position Y
int GetMouseY(void)
{
#if defined(PLATFORM_ANDROID)
    return (int)CORE.Input.Touch.position[0].y;
#else
    return (int)((CORE.Input.Mouse.currentPosition.y + CORE.Input.Mouse.offset.y)*CORE.Input.Mouse.scale.y);
#endif
}

// Get mouse position XY
Vector2 GetMousePosition(void)
{
    Vector2 position = { 0 };

#if defined(PLATFORM_ANDROID) || defined(PLATFORM_WEB)
    position = GetTouchPosition(0);
#else
    position.x = (CORE.Input.Mouse.currentPosition.x + CORE.Input.Mouse.offset.x)*CORE.Input.Mouse.scale.x;
    position.y = (CORE.Input.Mouse.currentPosition.y + CORE.Input.Mouse.offset.y)*CORE.Input.Mouse.scale.y;
#endif

    return position;
}

// Get mouse delta between frames
Vector2 GetMouseDelta(void)
{
    Vector2 delta = {0};

    delta.x = CORE.Input.Mouse.currentPosition.x - CORE.Input.Mouse.previousPosition.x;
    delta.y = CORE.Input.Mouse.currentPosition.y - CORE.Input.Mouse.previousPosition.y;

    return delta;
}

// Set mouse position XY
void SetMousePosition(int x, int y)
{
    CORE.Input.Mouse.currentPosition = (Vector2){ (float)x, (float)y };
#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
    // NOTE: emscripten not implemented
    glfwSetCursorPos(CORE.Window.handle, CORE.Input.Mouse.currentPosition.x, CORE.Input.Mouse.currentPosition.y);
#endif
}

// Set mouse offset
// NOTE: Useful when rendering to different size targets
void SetMouseOffset(int offsetX, int offsetY)
{
    CORE.Input.Mouse.offset = (Vector2){ (float)offsetX, (float)offsetY };
}

// Set mouse scaling
// NOTE: Useful when rendering to different size targets
void SetMouseScale(float scaleX, float scaleY)
{
    CORE.Input.Mouse.scale = (Vector2){ scaleX, scaleY };
}

// Get mouse wheel movement Y
float GetMouseWheelMove(void)
{
#if defined(PLATFORM_ANDROID)
    return 0.0f;
#endif
#if defined(PLATFORM_WEB)
    return CORE.Input.Mouse.previousWheelMove/100.0f;
#endif

    return CORE.Input.Mouse.previousWheelMove;
}

// Set mouse cursor
// NOTE: This is a no-op on platforms other than PLATFORM_DESKTOP
void SetMouseCursor(int cursor)
{
#if defined(PLATFORM_DESKTOP)
    CORE.Input.Mouse.cursor = cursor;
    if (cursor == MOUSE_CURSOR_DEFAULT) glfwSetCursor(CORE.Window.handle, NULL);
    else
    {
        // NOTE: We are relating internal GLFW enum values to our MouseCursor enum values
        glfwSetCursor(CORE.Window.handle, glfwCreateStandardCursor(0x00036000 + cursor));
    }
#endif
}

// Get touch position X for touch point 0 (relative to screen size)
int GetTouchX(void)
{
#if defined(PLATFORM_ANDROID) || defined(PLATFORM_WEB)
    return (int)CORE.Input.Touch.position[0].x;
#else
    return GetMouseX();
#endif
}

// Get touch position Y for touch point 0 (relative to screen size)
int GetTouchY(void)
{
#if defined(PLATFORM_ANDROID) || defined(PLATFORM_WEB)
    return (int)CORE.Input.Touch.position[0].y;
#else
    return GetMouseY();
#endif
}

// Get touch position XY for a touch point index (relative to screen size)
// TODO: Touch position should be scaled depending on display size and render size
Vector2 GetTouchPosition(int index)
{
    Vector2 position = { -1.0f, -1.0f };

#if defined(PLATFORM_DESKTOP)
    // TODO: GLFW does not support multi-touch input just yet
    // https://www.codeproject.com/Articles/668404/Programming-for-Multi-Touch
    // https://docs.microsoft.com/en-us/windows/win32/wintouch/getting-started-with-multi-touch-messages
    if (index == 0) position = GetMousePosition();
#endif
#if defined(PLATFORM_ANDROID)
    if (index < MAX_TOUCH_POINTS) position = CORE.Input.Touch.position[index];
    else TRACELOG(LOG_WARNING, "INPUT: Required touch point out of range (Max touch points: %i)", MAX_TOUCH_POINTS);

    if ((CORE.Window.screen.width > CORE.Window.display.width) || (CORE.Window.screen.height > CORE.Window.display.height))
    {
        position.x = position.x*((float)CORE.Window.screen.width/(float)(CORE.Window.display.width - CORE.Window.renderOffset.x)) - CORE.Window.renderOffset.x/2;
        position.y = position.y*((float)CORE.Window.screen.height/(float)(CORE.Window.display.height - CORE.Window.renderOffset.y)) - CORE.Window.renderOffset.y/2;
    }
    else
    {
        position.x = position.x*((float)CORE.Window.render.width/(float)CORE.Window.display.width) - CORE.Window.renderOffset.x/2;
        position.y = position.y*((float)CORE.Window.render.height/(float)CORE.Window.display.height) - CORE.Window.renderOffset.y/2;
    }
#endif
#if defined(PLATFORM_WEB)
    if (index < MAX_TOUCH_POINTS) position = CORE.Input.Touch.position[index];
    else TRACELOG(LOG_WARNING, "INPUT: Required touch point out of range (Max touch points: %i)", MAX_TOUCH_POINTS);

    // TODO: Touch position scaling required?
#endif

    return position;
}

//----------------------------------------------------------------------------------
// Module specific Functions Definition
//----------------------------------------------------------------------------------

// Initialize display device and framebuffer
// NOTE: width and height represent the screen (framebuffer) desired size, not actual display size
// If width or height are 0, default display size will be used for framebuffer size
// NOTE: returns false in case graphic device could not be created
static bool InitGraphicsDevice(int width, int height)
{
    CORE.Window.screen.width = width;            // User desired width
    CORE.Window.screen.height = height;          // User desired height
    CORE.Window.screenScale = MatrixIdentity();  // No draw scaling required by default

    // NOTE: Framebuffer (render area - CORE.Window.render.width, CORE.Window.render.height) could include black bars...
    // ...in top-down or left-right to match display aspect ratio (no weird scalings)

#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
    glfwSetErrorCallback(ErrorCallback);

#if defined(__APPLE__)
    glfwInitHint(GLFW_COCOA_CHDIR_RESOURCES, GLFW_FALSE);
#endif

    if (!glfwInit())
    {
        TRACELOG(LOG_WARNING, "GLFW: Failed to initialize GLFW");
        return false;
    }

    // NOTE: Getting video modes is not implemented in emscripten GLFW3 version
#if defined(PLATFORM_DESKTOP)
    // Find monitor resolution
    GLFWmonitor *monitor = glfwGetPrimaryMonitor();
    if (!monitor)
    {
        TRACELOG(LOG_WARNING, "GLFW: Failed to get primary monitor");
        return false;
    }
    const GLFWvidmode *mode = glfwGetVideoMode(monitor);

    CORE.Window.display.width = mode->width;
    CORE.Window.display.height = mode->height;

    // Set screen width/height to the display width/height if they are 0
    if (CORE.Window.screen.width == 0) CORE.Window.screen.width = CORE.Window.display.width;
    if (CORE.Window.screen.height == 0) CORE.Window.screen.height = CORE.Window.display.height;
#endif  // PLATFORM_DESKTOP

#if defined(PLATFORM_WEB)
    CORE.Window.display.width = CORE.Window.screen.width;
    CORE.Window.display.height = CORE.Window.screen.height;
#endif  // PLATFORM_WEB

    glfwDefaultWindowHints();                       // Set default windows hints
    //glfwWindowHint(GLFW_RED_BITS, 8);             // Framebuffer red color component bits
    //glfwWindowHint(GLFW_GREEN_BITS, 8);           // Framebuffer green color component bits
    //glfwWindowHint(GLFW_BLUE_BITS, 8);            // Framebuffer blue color component bits
    //glfwWindowHint(GLFW_ALPHA_BITS, 8);           // Framebuffer alpha color component bits
    //glfwWindowHint(GLFW_DEPTH_BITS, 24);          // Depthbuffer bits
    //glfwWindowHint(GLFW_REFRESH_RATE, 0);         // Refresh rate for fullscreen window
    //glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_API); // OpenGL API to use. Alternative: GLFW_OPENGL_ES_API
    //glfwWindowHint(GLFW_AUX_BUFFERS, 0);          // Number of auxiliar buffers

    // Check window creation flags
    if ((CORE.Window.flags & FLAG_FULLSCREEN_MODE) > 0) CORE.Window.fullscreen = true;

    if ((CORE.Window.flags & FLAG_WINDOW_HIDDEN) > 0) glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE); // Visible window
    else glfwWindowHint(GLFW_VISIBLE, GLFW_TRUE);     // Window initially hidden

    if ((CORE.Window.flags & FLAG_WINDOW_UNDECORATED) > 0) glfwWindowHint(GLFW_DECORATED, GLFW_FALSE); // Border and buttons on Window
    else glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);   // Decorated window

    if ((CORE.Window.flags & FLAG_WINDOW_RESIZABLE) > 0) glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE); // Resizable window
    else glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);  // Avoid window being resizable

    // Disable FLAG_WINDOW_MINIMIZED, not supported on initialization
    if ((CORE.Window.flags & FLAG_WINDOW_MINIMIZED) > 0) CORE.Window.flags &= ~FLAG_WINDOW_MINIMIZED;

    // Disable FLAG_WINDOW_MAXIMIZED, not supported on initialization
    if ((CORE.Window.flags & FLAG_WINDOW_MAXIMIZED) > 0) CORE.Window.flags &= ~FLAG_WINDOW_MAXIMIZED;

    if ((CORE.Window.flags & FLAG_WINDOW_UNFOCUSED) > 0) glfwWindowHint(GLFW_FOCUSED, GLFW_FALSE);
    else glfwWindowHint(GLFW_FOCUSED, GLFW_TRUE);

    if ((CORE.Window.flags & FLAG_WINDOW_TOPMOST) > 0) glfwWindowHint(GLFW_FLOATING, GLFW_TRUE);
    else glfwWindowHint(GLFW_FLOATING, GLFW_FALSE);

    // NOTE: Some GLFW flags are not supported on HTML5
#if defined(PLATFORM_DESKTOP)
    if ((CORE.Window.flags & FLAG_WINDOW_TRANSPARENT) > 0) glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_TRUE);     // Transparent framebuffer
    else glfwWindowHint(GLFW_TRANSPARENT_FRAMEBUFFER, GLFW_FALSE);  // Opaque framebuffer

    if ((CORE.Window.flags & FLAG_WINDOW_HIGHDPI) > 0)
    {
        // Resize window content area based on the monitor content scale.
        // NOTE: This hint only has an effect on platforms where screen coordinates and pixels always map 1:1 such as Windows and X11.
        // On platforms like macOS the resolution of the framebuffer is changed independently of the window size.
        glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_TRUE);   // Scale content area based on the monitor content scale where window is placed on
    #if defined(__APPLE__)
        glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
    #endif
    }
    else glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE);
#endif

    if (CORE.Window.flags & FLAG_MSAA_4X_HINT)
    {
        // NOTE: MSAA is only enabled for main framebuffer, not user-created FBOs
        TRACELOG(LOG_INFO, "DISPLAY: Trying to enable MSAA x4");
        glfwWindowHint(GLFW_SAMPLES, 4);   // Tries to enable multisampling x4 (MSAA), default is 0
    }

    // NOTE: When asking for an OpenGL context version, most drivers provide highest supported version
    // with forward compatibility to older OpenGL versions.
    // For example, if using OpenGL 1.1, driver can provide a 4.3 context forward compatible.

    // Check selection OpenGL version
    if (rlGetVersion() == OPENGL_33)
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);          // Choose OpenGL major version (just hint)
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);          // Choose OpenGL minor version (just hint)
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE); // Profiles Hint: Only 3.3 and above!
                                                                       // Values: GLFW_OPENGL_CORE_PROFILE, GLFW_OPENGL_ANY_PROFILE, GLFW_OPENGL_COMPAT_PROFILE
#if defined(__APPLE__)
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);  // OSX Requires fordward compatibility
#else
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_FALSE); // Fordward Compatibility Hint: Only 3.3 and above!
#endif
        //glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GLFW_TRUE); // Request OpenGL DEBUG context
    }
    else if (rlGetVersion() == OPENGL_ES_20)                    // Request OpenGL ES 2.0 context
    {
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
        glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#if defined(PLATFORM_DESKTOP)
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
#else
        glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
#endif
    }

#if defined(PLATFORM_DESKTOP)
    // NOTE: GLFW 3.4+ defers initialization of the Joystick subsystem on the first call to any Joystick related functions.
    // Forcing this initialization here avoids doing it on `PollInputEvents` called by `EndDrawing` after first frame has been just drawn.
    // The initialization will still happen and possible delays still occur, but before the window is shown, which is a nicer experience.
    // REF: https://github.com/raysan5/raylib/issues/1554
    if (MAX_GAMEPADS > 0) glfwSetJoystickCallback(NULL);
#endif

    if (CORE.Window.fullscreen)
    {
        // remember center for switchinging from fullscreen to window
        CORE.Window.position.x = CORE.Window.display.width/2 - CORE.Window.screen.width/2;
        CORE.Window.position.y = CORE.Window.display.height/2 - CORE.Window.screen.height/2;

        if (CORE.Window.position.x < 0) CORE.Window.position.x = 0;
        if (CORE.Window.position.y < 0) CORE.Window.position.y = 0;

        // Obtain recommended CORE.Window.display.width/CORE.Window.display.height from a valid videomode for the monitor
        int count = 0;
        const GLFWvidmode *modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);

        // Get closest video mode to desired CORE.Window.screen.width/CORE.Window.screen.height
        for (int i = 0; i < count; i++)
        {
            if ((unsigned int)modes[i].width >= CORE.Window.screen.width)
            {
                if ((unsigned int)modes[i].height >= CORE.Window.screen.height)
                {
                    CORE.Window.display.width = modes[i].width;
                    CORE.Window.display.height = modes[i].height;
                    break;
                }
            }
        }

#if defined(PLATFORM_DESKTOP)
        // If we are windowed fullscreen, ensures that window does not minimize when focus is lost
        if ((CORE.Window.screen.height == CORE.Window.display.height) && (CORE.Window.screen.width == CORE.Window.display.width))
        {
            glfwWindowHint(GLFW_AUTO_ICONIFY, 0);
        }
#endif
        TRACELOG(LOG_WARNING, "SYSTEM: Closest fullscreen videomode: %i x %i", CORE.Window.display.width, CORE.Window.display.height);

        // NOTE: ISSUE: Closest videomode could not match monitor aspect-ratio, for example,
        // for a desired screen size of 800x450 (16:9), closest supported videomode is 800x600 (4:3),
        // framebuffer is rendered correctly but once displayed on a 16:9 monitor, it gets stretched
        // by the sides to fit all monitor space...

        // Try to setup the most appropiate fullscreen framebuffer for the requested screenWidth/screenHeight
        // It considers device display resolution mode and setups a framebuffer with black bars if required (render size/offset)
        // Modified global variables: CORE.Window.screen.width/CORE.Window.screen.height - CORE.Window.render.width/CORE.Window.render.height - CORE.Window.renderOffset.x/CORE.Window.renderOffset.y - CORE.Window.screenScale
        // TODO: It is a quite cumbersome solution to display size vs requested size, it should be reviewed or removed...
        // HighDPI monitors are properly considered in a following similar function: SetupViewport()
        SetupFramebuffer(CORE.Window.display.width, CORE.Window.display.height);

        CORE.Window.handle = glfwCreateWindow(CORE.Window.display.width, CORE.Window.display.height, (CORE.Window.title != 0)? CORE.Window.title : " ", glfwGetPrimaryMonitor(), NULL);

        // NOTE: Full-screen change, not working properly...
        //glfwSetWindowMonitor(CORE.Window.handle, glfwGetPrimaryMonitor(), 0, 0, CORE.Window.screen.width, CORE.Window.screen.height, GLFW_DONT_CARE);
    }
    else
    {
        // No-fullscreen window creation
        CORE.Window.handle = glfwCreateWindow(CORE.Window.screen.width, CORE.Window.screen.height, (CORE.Window.title != 0)? CORE.Window.title : " ", NULL, NULL);

        if (CORE.Window.handle)
        {
#if defined(PLATFORM_DESKTOP)
            // Center window on screen
            int windowPosX = CORE.Window.display.width/2 - CORE.Window.screen.width/2;
            int windowPosY = CORE.Window.display.height/2 - CORE.Window.screen.height/2;

            if (windowPosX < 0) windowPosX = 0;
            if (windowPosY < 0) windowPosY = 0;

            glfwSetWindowPos(CORE.Window.handle, windowPosX, windowPosY);
#endif
            CORE.Window.render.width = CORE.Window.screen.width;
            CORE.Window.render.height = CORE.Window.screen.height;
        }
    }

    if (!CORE.Window.handle)
    {
        glfwTerminate();
        TRACELOG(LOG_WARNING, "GLFW: Failed to initialize Window");
        return false;
    }
    else
    {
        TRACELOG(LOG_INFO, "DISPLAY: Device initialized successfully");
#if defined(PLATFORM_DESKTOP)
        TRACELOG(LOG_INFO, "    > Display size: %i x %i", CORE.Window.display.width, CORE.Window.display.height);
#endif
        TRACELOG(LOG_INFO, "    > Render size:  %i x %i", CORE.Window.render.width, CORE.Window.render.height);
        TRACELOG(LOG_INFO, "    > Screen size:  %i x %i", CORE.Window.screen.width, CORE.Window.screen.height);
        TRACELOG(LOG_INFO, "    > Viewport offsets: %i, %i", CORE.Window.renderOffset.x, CORE.Window.renderOffset.y);
    }

    // Set window callback events
    glfwSetWindowSizeCallback(CORE.Window.handle, WindowSizeCallback);      // NOTE: Resizing not allowed by default!
#if !defined(PLATFORM_WEB)
    glfwSetWindowMaximizeCallback(CORE.Window.handle, WindowMaximizeCallback);
#endif
    glfwSetWindowIconifyCallback(CORE.Window.handle, WindowIconifyCallback);
    glfwSetWindowFocusCallback(CORE.Window.handle, WindowFocusCallback);
    glfwSetDropCallback(CORE.Window.handle, WindowDropCallback);
    // Set input callback events
    glfwSetKeyCallback(CORE.Window.handle, KeyCallback);
    glfwSetCharCallback(CORE.Window.handle, CharCallback);
    glfwSetMouseButtonCallback(CORE.Window.handle, MouseButtonCallback);
    glfwSetCursorPosCallback(CORE.Window.handle, MouseCursorPosCallback);   // Track mouse position changes
    glfwSetScrollCallback(CORE.Window.handle, MouseScrollCallback);
    glfwSetCursorEnterCallback(CORE.Window.handle, CursorEnterCallback);

    glfwMakeContextCurrent(CORE.Window.handle);

#if !defined(PLATFORM_WEB)
    glfwSwapInterval(0);        // No V-Sync by default
#endif

    // Try to enable GPU V-Sync, so frames are limited to screen refresh rate (60Hz -> 60 FPS)
    // NOTE: V-Sync can be enabled by graphic driver configuration
    if (CORE.Window.flags & FLAG_VSYNC_HINT)
    {
        // WARNING: It seems to hits a critical render path in Intel HD Graphics
        glfwSwapInterval(1);
        TRACELOG(LOG_INFO, "DISPLAY: Trying to enable VSYNC");
    }
#endif  // PLATFORM_DESKTOP || PLATFORM_WEB

#if defined(PLATFORM_ANDROID)
    CORE.Window.fullscreen = true;
    CORE.Window.flags |= FLAG_FULLSCREEN_MODE;

    EGLint samples = 0;
    EGLint sampleBuffer = 0;
    if (CORE.Window.flags & FLAG_MSAA_4X_HINT)
    {
        samples = 4;
        sampleBuffer = 1;
        TRACELOG(LOG_INFO, "DISPLAY: Trying to enable MSAA x4");
    }

    const EGLint framebufferAttribs[] =
    {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES2_BIT,     // Type of context support -> Required on RPI?
        EGL_RED_SIZE, 8,            // RED color bit depth (alternative: 5)
        EGL_GREEN_SIZE, 8,          // GREEN color bit depth (alternative: 6)
        EGL_BLUE_SIZE, 8,           // BLUE color bit depth (alternative: 5)
        //EGL_TRANSPARENT_TYPE, EGL_NONE, // Request transparent framebuffer (EGL_TRANSPARENT_RGB does not work on RPI)
        EGL_DEPTH_SIZE, 16,         // Depth buffer size (Required to use Depth testing!)
        //EGL_STENCIL_SIZE, 8,      // Stencil buffer size
        EGL_SAMPLE_BUFFERS, sampleBuffer,    // Activate MSAA
        EGL_SAMPLES, samples,       // 4x Antialiasing if activated (Free on MALI GPUs)
        EGL_NONE
    };

    const EGLint contextAttribs[] =
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

#if defined(PLATFORM_ANDROID)
    EGLint numConfigs = 0;

    // Get an EGL device connection
    CORE.Window.device = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if (CORE.Window.device == EGL_NO_DISPLAY)
    {
        TRACELOG(LOG_WARNING, "DISPLAY: Failed to initialize EGL device");
        return false;
    }

    // Initialize the EGL device connection
    if (eglInitialize(CORE.Window.device, NULL, NULL) == EGL_FALSE)
    {
        // If all of the calls to eglInitialize returned EGL_FALSE then an error has occurred.
        TRACELOG(LOG_WARNING, "DISPLAY: Failed to initialize EGL device");
        return false;
    }
    
    // Get an appropriate EGL framebuffer configuration
    eglChooseConfig(CORE.Window.device, framebufferAttribs, &CORE.Window.config, 1, &numConfigs);

    // Set rendering API
    eglBindAPI(EGL_OPENGL_ES_API);

    // Create an EGL rendering context
    CORE.Window.context = eglCreateContext(CORE.Window.device, CORE.Window.config, EGL_NO_CONTEXT, contextAttribs);
    if (CORE.Window.context == EGL_NO_CONTEXT)
    {
        TRACELOG(LOG_WARNING, "DISPLAY: Failed to create EGL context");
        return false;
    }
#endif

    // Create an EGL window surface
    //---------------------------------------------------------------------------------
#if defined(PLATFORM_ANDROID)
    EGLint displayFormat = 0;

    // EGL_NATIVE_VISUAL_ID is an attribute of the EGLConfig that is guaranteed to be accepted by ANativeWindow_setBuffersGeometry()
    // As soon as we picked a EGLConfig, we can safely reconfigure the ANativeWindow buffers to match, using EGL_NATIVE_VISUAL_ID
    eglGetConfigAttrib(CORE.Window.device, CORE.Window.config, EGL_NATIVE_VISUAL_ID, &displayFormat);

    // At this point we need to manage render size vs screen size
    // NOTE: This function use and modify global module variables:
    //  -> CORE.Window.screen.width/CORE.Window.screen.height
    //  -> CORE.Window.render.width/CORE.Window.render.height
    //  -> CORE.Window.screenScale
    SetupFramebuffer(CORE.Window.display.width, CORE.Window.display.height);

    ANativeWindow_setBuffersGeometry(CORE.Android.app->window, CORE.Window.render.width, CORE.Window.render.height, displayFormat);
    //ANativeWindow_setBuffersGeometry(CORE.Android.app->window, 0, 0, displayFormat);       // Force use of native display size

    CORE.Window.surface = eglCreateWindowSurface(CORE.Window.device, CORE.Window.config, CORE.Android.app->window, NULL);
#endif  // PLATFORM_ANDROID

    // There must be at least one frame displayed before the buffers are swapped
    //eglSwapInterval(CORE.Window.device, 1);

    if (eglMakeCurrent(CORE.Window.device, CORE.Window.surface, CORE.Window.surface, CORE.Window.context) == EGL_FALSE)
    {
        TRACELOG(LOG_WARNING, "DISPLAY: Failed to attach EGL rendering context to EGL surface");
        return false;
    }
    else
    {
        TRACELOG(LOG_INFO, "DISPLAY: Device initialized successfully");
        TRACELOG(LOG_INFO, "    > Display size: %i x %i", CORE.Window.display.width, CORE.Window.display.height);
        TRACELOG(LOG_INFO, "    > Render size:  %i x %i", CORE.Window.render.width, CORE.Window.render.height);
        TRACELOG(LOG_INFO, "    > Screen size:  %i x %i", CORE.Window.screen.width, CORE.Window.screen.height);
        TRACELOG(LOG_INFO, "    > Viewport offsets: %i, %i", CORE.Window.renderOffset.x, CORE.Window.renderOffset.y);
    }
#endif  // PLATFORM_ANDROID

    // Load OpenGL extensions
    // NOTE: GL procedures address loader is required to load extensions
#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
    rlLoadExtensions(glfwGetProcAddress);
#else
    rlLoadExtensions(eglGetProcAddress);
#endif

    // Initialize OpenGL context (states and resources)
    // NOTE: CORE.Window.screen.width and CORE.Window.screen.height not used, just stored as globals in rlgl
    rlglInit(CORE.Window.screen.width, CORE.Window.screen.height);

    int fbWidth = CORE.Window.render.width;
    int fbHeight = CORE.Window.render.height;

#if defined(PLATFORM_DESKTOP)
    if ((CORE.Window.flags & FLAG_WINDOW_HIGHDPI) > 0)
    {
        // NOTE: On APPLE platforms system should manage window/input scaling and also framebuffer scaling
        // Framebuffer scaling should be activated with: glfwWindowHint(GLFW_COCOA_RETINA_FRAMEBUFFER, GLFW_TRUE);
    #if !defined(__APPLE__)
        glfwGetFramebufferSize(CORE.Window.handle, &fbWidth, &fbHeight);

        // Screen scaling matrix is required in case desired screen area is different than display area
        CORE.Window.screenScale = MatrixScale((float)fbWidth/CORE.Window.screen.width, (float)fbHeight/CORE.Window.screen.height, 1.0f);

        // Mouse input scaling for the new screen size
        SetMouseScale((float)CORE.Window.screen.width/fbWidth, (float)CORE.Window.screen.height/fbHeight);
    #endif
    }
#endif

    // Setup default viewport
    SetupViewport(fbWidth, fbHeight);

    CORE.Window.currentFbo.width = CORE.Window.screen.width;
    CORE.Window.currentFbo.height = CORE.Window.screen.height;

    ClearBackground(RAYWHITE);      // Default background color for raylib games :P

#if defined(PLATFORM_ANDROID)
    CORE.Window.ready = true;
#endif

    if ((CORE.Window.flags & FLAG_WINDOW_MINIMIZED) > 0) MinimizeWindow();

    return true;
}

// Set viewport for a provided width and height
static void SetupViewport(int width, int height)
{
    CORE.Window.render.width = width;
    CORE.Window.render.height = height;

    // Set viewport width and height
    // NOTE: We consider render size (scaled) and offset in case black bars are required and
    // render area does not match full display area (this situation is only applicable on fullscreen mode)
#if defined(__APPLE__)
    float xScale = 1.0f, yScale = 1.0f;
    glfwGetWindowContentScale(CORE.Window.handle, &xScale, &yScale);
    rlViewport(CORE.Window.renderOffset.x/2*xScale, CORE.Window.renderOffset.y/2*yScale, (CORE.Window.render.width - CORE.Window.renderOffset.x)*xScale, (CORE.Window.render.height - CORE.Window.renderOffset.y)*yScale);
#else
    rlViewport(CORE.Window.renderOffset.x/2, CORE.Window.renderOffset.y/2, CORE.Window.render.width - CORE.Window.renderOffset.x, CORE.Window.render.height - CORE.Window.renderOffset.y);
#endif

    rlMatrixMode(RL_PROJECTION);        // Switch to projection matrix
    rlLoadIdentity();                   // Reset current matrix (projection)

    // Set orthographic projection to current framebuffer size
    // NOTE: Configured top-left corner as (0, 0)
    rlOrtho(0, CORE.Window.render.width, CORE.Window.render.height, 0, 0.0f, 1.0f);

    rlMatrixMode(RL_MODELVIEW);         // Switch back to modelview matrix
    rlLoadIdentity();                   // Reset current matrix (modelview)
}

// Compute framebuffer size relative to screen size and display size
// NOTE: Global variables CORE.Window.render.width/CORE.Window.render.height and CORE.Window.renderOffset.x/CORE.Window.renderOffset.y can be modified
static void SetupFramebuffer(int width, int height)
{
    // Calculate CORE.Window.render.width and CORE.Window.render.height, we have the display size (input params) and the desired screen size (global var)
    if ((CORE.Window.screen.width > CORE.Window.display.width) || (CORE.Window.screen.height > CORE.Window.display.height))
    {
        TRACELOG(LOG_WARNING, "DISPLAY: Downscaling required: Screen size (%ix%i) is bigger than display size (%ix%i)", CORE.Window.screen.width, CORE.Window.screen.height, CORE.Window.display.width, CORE.Window.display.height);

        // Downscaling to fit display with border-bars
        float widthRatio = (float)CORE.Window.display.width/(float)CORE.Window.screen.width;
        float heightRatio = (float)CORE.Window.display.height/(float)CORE.Window.screen.height;

        if (widthRatio <= heightRatio)
        {
            CORE.Window.render.width = CORE.Window.display.width;
            CORE.Window.render.height = (int)round((float)CORE.Window.screen.height*widthRatio);
            CORE.Window.renderOffset.x = 0;
            CORE.Window.renderOffset.y = (CORE.Window.display.height - CORE.Window.render.height);
        }
        else
        {
            CORE.Window.render.width = (int)round((float)CORE.Window.screen.width*heightRatio);
            CORE.Window.render.height = CORE.Window.display.height;
            CORE.Window.renderOffset.x = (CORE.Window.display.width - CORE.Window.render.width);
            CORE.Window.renderOffset.y = 0;
        }

        // Screen scaling required
        float scaleRatio = (float)CORE.Window.render.width/(float)CORE.Window.screen.width;
        CORE.Window.screenScale = MatrixScale(scaleRatio, scaleRatio, 1.0f);

        // NOTE: We render to full display resolution!
        // We just need to calculate above parameters for downscale matrix and offsets
        CORE.Window.render.width = CORE.Window.display.width;
        CORE.Window.render.height = CORE.Window.display.height;

        TRACELOG(LOG_WARNING, "DISPLAY: Downscale matrix generated, content will be rendered at (%ix%i)", CORE.Window.render.width, CORE.Window.render.height);
    }
    else if ((CORE.Window.screen.width < CORE.Window.display.width) || (CORE.Window.screen.height < CORE.Window.display.height))
    {
        // Required screen size is smaller than display size
        TRACELOG(LOG_INFO, "DISPLAY: Upscaling required: Screen size (%ix%i) smaller than display size (%ix%i)", CORE.Window.screen.width, CORE.Window.screen.height, CORE.Window.display.width, CORE.Window.display.height);

        if ((CORE.Window.screen.width == 0) || (CORE.Window.screen.height == 0))
        {
            CORE.Window.screen.width = CORE.Window.display.width;
            CORE.Window.screen.height = CORE.Window.display.height;
        }

        // Upscaling to fit display with border-bars
        float displayRatio = (float)CORE.Window.display.width/(float)CORE.Window.display.height;
        float screenRatio = (float)CORE.Window.screen.width/(float)CORE.Window.screen.height;

        if (displayRatio <= screenRatio)
        {
            CORE.Window.render.width = CORE.Window.screen.width;
            CORE.Window.render.height = (int)round((float)CORE.Window.screen.width/displayRatio);
            CORE.Window.renderOffset.x = 0;
            CORE.Window.renderOffset.y = (CORE.Window.render.height - CORE.Window.screen.height);
        }
        else
        {
            CORE.Window.render.width = (int)round((float)CORE.Window.screen.height*displayRatio);
            CORE.Window.render.height = CORE.Window.screen.height;
            CORE.Window.renderOffset.x = (CORE.Window.render.width - CORE.Window.screen.width);
            CORE.Window.renderOffset.y = 0;
        }
    }
    else
    {
        CORE.Window.render.width = CORE.Window.screen.width;
        CORE.Window.render.height = CORE.Window.screen.height;
        CORE.Window.renderOffset.x = 0;
        CORE.Window.renderOffset.y = 0;
    }
}

// Initialize hi-resolution timer
static void InitTimer(void)
{
// Setting a higher resolution can improve the accuracy of time-out intervals in wait functions.
// However, it can also reduce overall system performance, because the thread scheduler switches tasks more often.
// High resolutions can also prevent the CPU power management system from entering power-saving modes.
// Setting a higher resolution does not improve the accuracy of the high-resolution performance counter.
#if defined(_WIN32) && defined(SUPPORT_WINMM_HIGHRES_TIMER) && !defined(SUPPORT_BUSY_WAIT_LOOP)
    timeBeginPeriod(1);                 // Setup high-resolution timer to 1ms (granularity of 1-2 ms)
#endif

#if defined(PLATFORM_ANDROID)
    struct timespec now = { 0 };

    if (clock_gettime(CLOCK_MONOTONIC, &now) == 0)  // Success
    {
        CORE.Time.base = (unsigned long long int)now.tv_sec*1000000000LLU + (unsigned long long int)now.tv_nsec;
    }
    else TRACELOG(LOG_WARNING, "TIMER: Hi-resolution timer not available");
#endif

    CORE.Time.previous = GetTime();     // Get time as double
}

// Wait for some milliseconds (stop program execution)
// NOTE: Sleep() granularity could be around 10 ms, it means, Sleep() could
// take longer than expected... for that reason we use the busy wait loop
// Ref: http://stackoverflow.com/questions/43057578/c-programming-win32-games-sleep-taking-longer-than-expected
// Ref: http://www.geisswerks.com/ryan/FAQS/timing.html --> All about timming on Win32!
void WaitTime(float ms)
{
#if defined(SUPPORT_BUSY_WAIT_LOOP)
    double previousTime = GetTime();
    double currentTime = 0.0;

    // Busy wait loop
    while ((currentTime - previousTime) < ms/1000.0f) currentTime = GetTime();
#else
    #if defined(SUPPORT_PARTIALBUSY_WAIT_LOOP)
        double busyWait = ms*0.05;     // NOTE: We are using a busy wait of 5% of the time
        ms -= (float)busyWait;
    #endif

    // System halt functions
    #if defined(_WIN32)
        Sleep((unsigned int)ms);
    #endif
    #if defined(__linux__) || defined(__FreeBSD__) || defined(__EMSCRIPTEN__)
        struct timespec req = { 0 };
        time_t sec = (int)(ms/1000.0f);
        ms -= (sec*1000);
        req.tv_sec = sec;
        req.tv_nsec = ms*1000000L;

        // NOTE: Use nanosleep() on Unix platforms... usleep() it's deprecated.
        while (nanosleep(&req, &req) == -1) continue;
    #endif
    #if defined(__APPLE__)
        usleep(ms*1000.0f);
    #endif

    #if defined(SUPPORT_PARTIALBUSY_WAIT_LOOP)
        double previousTime = GetTime();
        double currentTime = 0.0;

        // Partial busy wait loop (only a fraction of the total wait time)
        while ((currentTime - previousTime) < busyWait/1000.0f) currentTime = GetTime();
    #endif
#endif
}

// Swap back buffer with front buffer (screen drawing)
void SwapScreenBuffer(void)
{
#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
    glfwSwapBuffers(CORE.Window.handle);
#endif

#if defined(PLATFORM_ANDROID)
    eglSwapBuffers(CORE.Window.device, CORE.Window.surface);
#endif  // PLATFORM_ANDROID 
}

// Register all input events
void PollInputEvents(void)
{
#if defined(SUPPORT_GESTURES_SYSTEM)
    // NOTE: Gestures update must be called every frame to reset gestures correctly
    // because ProcessGestureEvent() is just called on an event, not every frame
    UpdateGestures();
#endif

    // Reset keys/chars pressed registered
    CORE.Input.Keyboard.keyPressedQueueCount = 0;
    CORE.Input.Keyboard.charPressedQueueCount = 0;

    // Reset last gamepad button/axis registered state
    CORE.Input.Gamepad.lastButtonPressed = -1;
    CORE.Input.Gamepad.axisCount = 0;

#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
    // Keyboard/Mouse input polling (automatically managed by GLFW3 through callback)

    // Register previous keys states
    for (int i = 0; i < 512; i++) CORE.Input.Keyboard.previousKeyState[i] = CORE.Input.Keyboard.currentKeyState[i];

    // Register previous mouse states
    for (int i = 0; i < 3; i++) CORE.Input.Mouse.previousButtonState[i] = CORE.Input.Mouse.currentButtonState[i];

    // Register previous mouse wheel state
    CORE.Input.Mouse.previousWheelMove = CORE.Input.Mouse.currentWheelMove;
    CORE.Input.Mouse.currentWheelMove = 0.0f;

    // Register previous mouse position
    CORE.Input.Mouse.previousPosition = CORE.Input.Mouse.currentPosition;
#endif

    // Register previous touch states
    for (int i = 0; i < MAX_TOUCH_POINTS; i++) CORE.Input.Touch.previousTouchState[i] = CORE.Input.Touch.currentTouchState[i];

#if defined(PLATFORM_DESKTOP)
    // Check if gamepads are ready
    // NOTE: We do it here in case of disconnection
    for (int i = 0; i < MAX_GAMEPADS; i++)
    {
        if (glfwJoystickPresent(i)) CORE.Input.Gamepad.ready[i] = true;
        else CORE.Input.Gamepad.ready[i] = false;
    }

    // Register gamepads buttons events
    for (int i = 0; i < MAX_GAMEPADS; i++)
    {
        if (CORE.Input.Gamepad.ready[i])     // Check if gamepad is available
        {
            // Register previous gamepad states
            for (int k = 0; k < MAX_GAMEPAD_BUTTONS; k++) CORE.Input.Gamepad.previousButtonState[i][k] = CORE.Input.Gamepad.currentButtonState[i][k];

            // Get current gamepad state
            // NOTE: There is no callback available, so we get it manually
            // Get remapped buttons
            GLFWgamepadstate state = { 0 };
            glfwGetGamepadState(i, &state); // This remapps all gamepads so they have their buttons mapped like an xbox controller

            const unsigned char *buttons = state.buttons;

            for (int k = 0; (buttons != NULL) && (k < GLFW_GAMEPAD_BUTTON_DPAD_LEFT + 1) && (k < MAX_GAMEPAD_BUTTONS); k++)
            {
                GamepadButton button = -1;

                switch (k)
                {
                    case GLFW_GAMEPAD_BUTTON_Y: button = GAMEPAD_BUTTON_RIGHT_FACE_UP; break;
                    case GLFW_GAMEPAD_BUTTON_B: button = GAMEPAD_BUTTON_RIGHT_FACE_RIGHT; break;
                    case GLFW_GAMEPAD_BUTTON_A: button = GAMEPAD_BUTTON_RIGHT_FACE_DOWN; break;
                    case GLFW_GAMEPAD_BUTTON_X: button = GAMEPAD_BUTTON_RIGHT_FACE_LEFT; break;

                    case GLFW_GAMEPAD_BUTTON_LEFT_BUMPER: button = GAMEPAD_BUTTON_LEFT_TRIGGER_1; break;
                    case GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER: button = GAMEPAD_BUTTON_RIGHT_TRIGGER_1; break;

                    case GLFW_GAMEPAD_BUTTON_BACK: button = GAMEPAD_BUTTON_MIDDLE_LEFT; break;
                    case GLFW_GAMEPAD_BUTTON_GUIDE: button = GAMEPAD_BUTTON_MIDDLE; break;
                    case GLFW_GAMEPAD_BUTTON_START: button = GAMEPAD_BUTTON_MIDDLE_RIGHT; break;

                    case GLFW_GAMEPAD_BUTTON_DPAD_UP: button = GAMEPAD_BUTTON_LEFT_FACE_UP; break;
                    case GLFW_GAMEPAD_BUTTON_DPAD_RIGHT: button = GAMEPAD_BUTTON_LEFT_FACE_RIGHT; break;
                    case GLFW_GAMEPAD_BUTTON_DPAD_DOWN: button = GAMEPAD_BUTTON_LEFT_FACE_DOWN; break;
                    case GLFW_GAMEPAD_BUTTON_DPAD_LEFT: button = GAMEPAD_BUTTON_LEFT_FACE_LEFT; break;

                    case GLFW_GAMEPAD_BUTTON_LEFT_THUMB: button = GAMEPAD_BUTTON_LEFT_THUMB; break;
                    case GLFW_GAMEPAD_BUTTON_RIGHT_THUMB: button = GAMEPAD_BUTTON_RIGHT_THUMB; break;
                    default: break;
                }

                if (button != -1)   // Check for valid button
                {
                    if (buttons[k] == GLFW_PRESS)
                    {
                        CORE.Input.Gamepad.currentButtonState[i][button] = 1;
                        CORE.Input.Gamepad.lastButtonPressed = button;
                    }
                    else CORE.Input.Gamepad.currentButtonState[i][button] = 0;
                }
            }

            // Get current axis state
            const float *axes = state.axes;

            for (int k = 0; (axes != NULL) && (k < GLFW_GAMEPAD_AXIS_LAST + 1) && (k < MAX_GAMEPAD_AXIS); k++)
            {
                CORE.Input.Gamepad.axisState[i][k] = axes[k];
            }

            // Register buttons for 2nd triggers (because GLFW doesn't count these as buttons but rather axis)
            CORE.Input.Gamepad.currentButtonState[i][GAMEPAD_BUTTON_LEFT_TRIGGER_2] = (char)(CORE.Input.Gamepad.axisState[i][GAMEPAD_AXIS_LEFT_TRIGGER] > 0.1);
            CORE.Input.Gamepad.currentButtonState[i][GAMEPAD_BUTTON_RIGHT_TRIGGER_2] = (char)(CORE.Input.Gamepad.axisState[i][GAMEPAD_AXIS_RIGHT_TRIGGER] > 0.1);

            CORE.Input.Gamepad.axisCount = GLFW_GAMEPAD_AXIS_LAST + 1;
        }
    }

    CORE.Window.resizedLastFrame = false;

#if defined(SUPPORT_EVENTS_WAITING)
    glfwWaitEvents();
#else
    glfwPollEvents();       // Register keyboard/mouse events (callbacks)... and window events!
#endif
#endif  // PLATFORM_DESKTOP

// Gamepad support using emscripten API
// NOTE: GLFW3 joystick functionality not available in web
#if defined(PLATFORM_WEB)
    // Get number of gamepads connected
    int numGamepads = 0;
    if (emscripten_sample_gamepad_data() == EMSCRIPTEN_RESULT_SUCCESS) numGamepads = emscripten_get_num_gamepads();

    for (int i = 0; (i < numGamepads) && (i < MAX_GAMEPADS); i++)
    {
        // Register previous gamepad button states
        for (int k = 0; k < MAX_GAMEPAD_BUTTONS; k++) CORE.Input.Gamepad.previousButtonState[i][k] = CORE.Input.Gamepad.currentButtonState[i][k];

        EmscriptenGamepadEvent gamepadState;

        int result = emscripten_get_gamepad_status(i, &gamepadState);

        if (result == EMSCRIPTEN_RESULT_SUCCESS)
        {
            // Register buttons data for every connected gamepad
            for (int j = 0; (j < gamepadState.numButtons) && (j < MAX_GAMEPAD_BUTTONS); j++)
            {
                GamepadButton button = -1;

                // Gamepad Buttons reference: https://www.w3.org/TR/gamepad/#gamepad-interface
                switch (j)
                {
                    case 0: button = GAMEPAD_BUTTON_RIGHT_FACE_DOWN; break;
                    case 1: button = GAMEPAD_BUTTON_RIGHT_FACE_RIGHT; break;
                    case 2: button = GAMEPAD_BUTTON_RIGHT_FACE_LEFT; break;
                    case 3: button = GAMEPAD_BUTTON_RIGHT_FACE_UP; break;
                    case 4: button = GAMEPAD_BUTTON_LEFT_TRIGGER_1; break;
                    case 5: button = GAMEPAD_BUTTON_RIGHT_TRIGGER_1; break;
                    case 6: button = GAMEPAD_BUTTON_LEFT_TRIGGER_2; break;
                    case 7: button = GAMEPAD_BUTTON_RIGHT_TRIGGER_2; break;
                    case 8: button = GAMEPAD_BUTTON_MIDDLE_LEFT; break;
                    case 9: button = GAMEPAD_BUTTON_MIDDLE_RIGHT; break;
                    case 10: button = GAMEPAD_BUTTON_LEFT_THUMB; break;
                    case 11: button = GAMEPAD_BUTTON_RIGHT_THUMB; break;
                    case 12: button = GAMEPAD_BUTTON_LEFT_FACE_UP; break;
                    case 13: button = GAMEPAD_BUTTON_LEFT_FACE_DOWN; break;
                    case 14: button = GAMEPAD_BUTTON_LEFT_FACE_LEFT; break;
                    case 15: button = GAMEPAD_BUTTON_LEFT_FACE_RIGHT; break;
                    default: break;
                }

                if (button != -1)   // Check for valid button
                {
                    if (gamepadState.digitalButton[j] == 1)
                    {
                        CORE.Input.Gamepad.currentButtonState[i][button] = 1;
                        CORE.Input.Gamepad.lastButtonPressed = button;
                    }
                    else CORE.Input.Gamepad.currentButtonState[i][button] = 0;
                }

                //TRACELOGD("INPUT: Gamepad %d, button %d: Digital: %d, Analog: %g", gamepadState.index, j, gamepadState.digitalButton[j], gamepadState.analogButton[j]);
            }

            // Register axis data for every connected gamepad
            for (int j = 0; (j < gamepadState.numAxes) && (j < MAX_GAMEPAD_AXIS); j++)
            {
                CORE.Input.Gamepad.axisState[i][j] = gamepadState.axis[j];
            }

            CORE.Input.Gamepad.axisCount = gamepadState.numAxes;
        }
    }
#endif

#if defined(PLATFORM_ANDROID)
    // Register previous keys states
    // NOTE: Android supports up to 260 keys
    for (int i = 0; i < 260; i++) CORE.Input.Keyboard.previousKeyState[i] = CORE.Input.Keyboard.currentKeyState[i];

    // Android ALooper_pollAll() variables
    int pollResult = 0;
    int pollEvents = 0;

    // Poll Events (registered events)
    // NOTE: Activity is paused if not enabled (CORE.Android.appEnabled)
    while ((pollResult = ALooper_pollAll(CORE.Android.appEnabled? 0 : -1, NULL, &pollEvents, (void**)&CORE.Android.source)) >= 0)
    {
        // Process this event
        if (CORE.Android.source != NULL) CORE.Android.source->process(CORE.Android.app, CORE.Android.source);

        // NOTE: Never close window, native activity is controlled by the system!
        if (CORE.Android.app->destroyRequested != 0)
        {
            //CORE.Window.shouldClose = true;
            //ANativeActivity_finish(CORE.Android.app->activity);
        }
    }
#endif
}

#if defined(PLATFORM_DESKTOP) || defined(PLATFORM_WEB)
// GLFW3 Error Callback, runs on GLFW3 error
static void ErrorCallback(int error, const char *description)
{
    TRACELOG(LOG_WARNING, "GLFW: Error: %i Description: %s", error, description);
}

#if defined(PLATFORM_WEB)
EM_JS(int, GetCanvasWidth, (), { return canvas.clientWidth; });
EM_JS(int, GetCanvasHeight, (), { return canvas.clientHeight; });

static EM_BOOL EmscriptenResizeCallback(int eventType, const EmscriptenUiEvent *e, void *userData)
{
    // Don't resize non-resizeable windows
    if ((CORE.Window.flags & FLAG_WINDOW_RESIZABLE) == 0) return true;

    // This event is called whenever the window changes sizes,
    // so the size of the canvas object is explicitly retrieved below
    int width = GetCanvasWidth();
    int height = GetCanvasHeight();
    emscripten_set_canvas_element_size("#canvas",width,height);

    SetupViewport(width, height);    // Reset viewport and projection matrix for new size

    CORE.Window.currentFbo.width = width;
    CORE.Window.currentFbo.height = height;
    CORE.Window.resizedLastFrame = true;

    if (IsWindowFullscreen()) return true;

    // Set current screen size
    CORE.Window.screen.width = width;
    CORE.Window.screen.height = height;

    // NOTE: Postprocessing texture is not scaled to new size
}
#endif

// GLFW3 WindowSize Callback, runs when window is resizedLastFrame
// NOTE: Window resizing not allowed by default
static void WindowSizeCallback(GLFWwindow *window, int width, int height)
{
    SetupViewport(width, height);    // Reset viewport and projection matrix for new size
    CORE.Window.currentFbo.width = width;
    CORE.Window.currentFbo.height = height;
    CORE.Window.resizedLastFrame = true;

    if (IsWindowFullscreen()) return;

    // Set current screen size
    CORE.Window.screen.width = width;
    CORE.Window.screen.height = height;
    // NOTE: Postprocessing texture is not scaled to new size

}

// GLFW3 WindowIconify Callback, runs when window is minimized/restored
static void WindowIconifyCallback(GLFWwindow *window, int iconified)
{
    if (iconified) CORE.Window.flags |= FLAG_WINDOW_MINIMIZED;  // The window was iconified
    else CORE.Window.flags &= ~FLAG_WINDOW_MINIMIZED;           // The window was restored
}

#if !defined(PLATFORM_WEB)
// GLFW3 WindowMaximize Callback, runs when window is maximized/restored
static void WindowMaximizeCallback(GLFWwindow *window, int maximized)
{
    if (maximized) CORE.Window.flags |= FLAG_WINDOW_MAXIMIZED;  // The window was maximized
    else CORE.Window.flags &= ~FLAG_WINDOW_MAXIMIZED;           // The window was restored
}
#endif

// GLFW3 WindowFocus Callback, runs when window get/lose focus
static void WindowFocusCallback(GLFWwindow *window, int focused)
{
    if (focused) CORE.Window.flags &= ~FLAG_WINDOW_UNFOCUSED;   // The window was focused
    else CORE.Window.flags |= FLAG_WINDOW_UNFOCUSED;            // The window lost focus
}

// GLFW3 Keyboard Callback, runs on key pressed
static void KeyCallback(GLFWwindow *window, int key, int scancode, int action, int mods)
{
    //TRACELOG(LOG_DEBUG, "Key Callback: KEY:%i(%c) - SCANCODE:%i (STATE:%i)", key, key, scancode, action);

#if defined(SUPPORT_SCREEN_CAPTURE)
    if (key == GLFW_KEY_F12 && action == GLFW_PRESS)
    {
#if defined(SUPPORT_GIF_RECORDING)
        if (mods == GLFW_MOD_CONTROL)
        {
            if (gifRecording)
            {
                gifRecording = false;

                MsfGifResult result = msf_gif_end(&gifState);

                SaveFileData(TextFormat("%s/screenrec%03i.gif", CORE.Storage.basePath, screenshotCounter), result.data, (unsigned int)result.dataSize);
                msf_gif_free(result);

            #if defined(PLATFORM_WEB)
                // Download file from MEMFS (emscripten memory filesystem)
                // saveFileFromMEMFSToDisk() function is defined in raylib/templates/web_shel/shell.html
                emscripten_run_script(TextFormat("saveFileFromMEMFSToDisk('%s','%s')", TextFormat("screenrec%03i.gif", screenshotCounter - 1), TextFormat("screenrec%03i.gif", screenshotCounter - 1)));
            #endif

                TRACELOG(LOG_INFO, "SYSTEM: Finish animated GIF recording");
            }
            else
            {
                gifRecording = true;
                gifFramesCounter = 0;

                msf_gif_begin(&gifState, CORE.Window.screen.width, CORE.Window.screen.height);
                screenshotCounter++;

                TRACELOG(LOG_INFO, "SYSTEM: Start animated GIF recording: %s", TextFormat("screenrec%03i.gif", screenshotCounter));
            }
        }
        else
#endif  // SUPPORT_GIF_RECORDING
        {
            TakeScreenshot(TextFormat("screenshot%03i.png", screenshotCounter));
            screenshotCounter++;
        }
    }
#endif  // SUPPORT_SCREEN_CAPTURE
    else
    {
        // WARNING: GLFW could return GLFW_REPEAT, we need to consider it as 1
        // to work properly with our implementation (IsKeyDown/IsKeyUp checks)
        if (action == GLFW_RELEASE) CORE.Input.Keyboard.currentKeyState[key] = 0;
        else CORE.Input.Keyboard.currentKeyState[key] = 1;

        // Check if there is space available in the key queue
        if ((CORE.Input.Keyboard.keyPressedQueueCount < MAX_KEY_PRESSED_QUEUE) && (action == GLFW_PRESS))
        {
            // Add character to the queue
            CORE.Input.Keyboard.keyPressedQueue[CORE.Input.Keyboard.keyPressedQueueCount] = key;
            CORE.Input.Keyboard.keyPressedQueueCount++;
        }
    }
}

// GLFW3 Char Key Callback, runs on key down (gets equivalent unicode char value)
static void CharCallback(GLFWwindow *window, unsigned int key)
{
    //TRACELOG(LOG_DEBUG, "Char Callback: KEY:%i(%c)", key, key);

    // NOTE: Registers any key down considering OS keyboard layout but
    // do not detects action events, those should be managed by user...
    // Ref: https://github.com/glfw/glfw/issues/668#issuecomment-166794907
    // Ref: https://www.glfw.org/docs/latest/input_guide.html#input_char

    // Check if there is space available in the queue
    if (CORE.Input.Keyboard.charPressedQueueCount < MAX_KEY_PRESSED_QUEUE)
    {
        // Add character to the queue
        CORE.Input.Keyboard.charPressedQueue[CORE.Input.Keyboard.charPressedQueueCount] = key;
        CORE.Input.Keyboard.charPressedQueueCount++;
    }
}

// GLFW3 Mouse Button Callback, runs on mouse button pressed
static void MouseButtonCallback(GLFWwindow *window, int button, int action, int mods)
{
    // WARNING: GLFW could only return GLFW_PRESS (1) or GLFW_RELEASE (0) for now,
    // but future releases may add more actions (i.e. GLFW_REPEAT)
    CORE.Input.Mouse.currentButtonState[button] = action;

#if defined(SUPPORT_GESTURES_SYSTEM) && defined(SUPPORT_MOUSE_GESTURES)
    // Process mouse events as touches to be able to use mouse-gestures
    GestureEvent gestureEvent = { 0 };

    // Register touch actions
    if ((CORE.Input.Mouse.currentButtonState[button] == 1) && (CORE.Input.Mouse.previousButtonState[button] == 0)) gestureEvent.touchAction = TOUCH_DOWN;
    else if ((CORE.Input.Mouse.currentButtonState[button] == 0) && (CORE.Input.Mouse.previousButtonState[button] == 1)) gestureEvent.touchAction = TOUCH_UP;

    // NOTE: TOUCH_MOVE event is registered in MouseCursorPosCallback()

    // Assign a pointer ID
    gestureEvent.pointerId[0] = 0;

    // Register touch points count
    gestureEvent.pointCount = 1;

    // Register touch points position, only one point registered
    gestureEvent.position[0] = GetMousePosition();

    // Normalize gestureEvent.position[0] for CORE.Window.screen.width and CORE.Window.screen.height
    gestureEvent.position[0].x /= (float)GetScreenWidth();
    gestureEvent.position[0].y /= (float)GetScreenHeight();

    // Gesture data is sent to gestures system for processing
    ProcessGestureEvent(gestureEvent);
#endif
}

// GLFW3 Cursor Position Callback, runs on mouse move
static void MouseCursorPosCallback(GLFWwindow *window, double x, double y)
{
    CORE.Input.Mouse.currentPosition.x = (float)x;
    CORE.Input.Mouse.currentPosition.y = (float)y;
    CORE.Input.Touch.position[0] = CORE.Input.Mouse.currentPosition;

#if defined(SUPPORT_GESTURES_SYSTEM) && defined(SUPPORT_MOUSE_GESTURES)
    // Process mouse events as touches to be able to use mouse-gestures
    GestureEvent gestureEvent = { 0 };

    gestureEvent.touchAction = TOUCH_MOVE;

    // Assign a pointer ID
    gestureEvent.pointerId[0] = 0;

    // Register touch points count
    gestureEvent.pointCount = 1;

    // Register touch points position, only one point registered
    gestureEvent.position[0] = CORE.Input.Touch.position[0];

    // Normalize gestureEvent.position[0] for CORE.Window.screen.width and CORE.Window.screen.height
    gestureEvent.position[0].x /= (float)GetScreenWidth();
    gestureEvent.position[0].y /= (float)GetScreenHeight();

    // Gesture data is sent to gestures system for processing
    ProcessGestureEvent(gestureEvent);
#endif
}

// GLFW3 Srolling Callback, runs on mouse wheel
static void MouseScrollCallback(GLFWwindow *window, double xoffset, double yoffset)
{
    CORE.Input.Mouse.currentWheelMove = (float)yoffset;
}

// GLFW3 CursorEnter Callback, when cursor enters the window
static void CursorEnterCallback(GLFWwindow *window, int enter)
{
    if (enter == true) CORE.Input.Mouse.cursorOnScreen = true;
    else CORE.Input.Mouse.cursorOnScreen = false;
}

// GLFW3 Window Drop Callback, runs when drop files into window
// NOTE: Paths are stored in dynamic memory for further retrieval
// Everytime new files are dropped, old ones are discarded
static void WindowDropCallback(GLFWwindow *window, int count, const char **paths)
{
    ClearDroppedFiles();

    CORE.Window.dropFilesPath = (char **)RL_MALLOC(count*sizeof(char *));

    for (int i = 0; i < count; i++)
    {
        CORE.Window.dropFilesPath[i] = (char *)RL_MALLOC(MAX_FILEPATH_LENGTH*sizeof(char));
        strcpy(CORE.Window.dropFilesPath[i], paths[i]);
    }

    CORE.Window.dropFilesCount = count;
}
#endif

#if defined(PLATFORM_ANDROID)
// ANDROID: Process activity lifecycle commands
static void AndroidCommandCallback(struct android_app *app, int32_t cmd)
{
    switch (cmd)
    {
        case APP_CMD_START:
        {
            //rendering = true;
        } break;
        case APP_CMD_RESUME: break;
        case APP_CMD_INIT_WINDOW:
        {
            if (app->window != NULL)
            {
                if (CORE.Android.contextRebindRequired)
                {
                    // Reset screen scaling to full display size
                    EGLint displayFormat = 0;
                    eglGetConfigAttrib(CORE.Window.device, CORE.Window.config, EGL_NATIVE_VISUAL_ID, &displayFormat);
                    ANativeWindow_setBuffersGeometry(app->window, CORE.Window.render.width, CORE.Window.render.height, displayFormat);

                    // Recreate display surface and re-attach OpenGL context
                    CORE.Window.surface = eglCreateWindowSurface(CORE.Window.device, CORE.Window.config, app->window, NULL);
                    eglMakeCurrent(CORE.Window.device, CORE.Window.surface, CORE.Window.surface, CORE.Window.context);

                    CORE.Android.contextRebindRequired = false;
                }
                else
                {
                    CORE.Window.display.width = ANativeWindow_getWidth(CORE.Android.app->window);
                    CORE.Window.display.height = ANativeWindow_getHeight(CORE.Android.app->window);

                    // Initialize graphics device (display device and OpenGL context)
                    InitGraphicsDevice(CORE.Window.screen.width, CORE.Window.screen.height);

                    // Initialize hi-res timer
                    InitTimer();

                    // Initialize random seed
                    srand((unsigned int)time(NULL));

                #if defined(SUPPORT_DEFAULT_FONT)
                    // Load default font
                    // NOTE: External function (defined in module: text)
                    LoadFontDefault();
                    Rectangle rec = GetFontDefault().recs[95];
                    // NOTE: We setup a 1px padding on char rectangle to avoid pixel bleeding on MSAA filtering
                    SetShapesTexture(GetFontDefault().texture, (Rectangle){ rec.x + 1, rec.y + 1, rec.width - 2, rec.height - 2 });
                #endif

                    // TODO: GPU assets reload in case of lost focus (lost context)
                    // NOTE: This problem has been solved just unbinding and rebinding context from display
                    /*
                    if (assetsReloadRequired)
                    {
                        for (int i = 0; i < assetsCount; i++)
                        {
                            // TODO: Unload old asset if required

                            // Load texture again to pointed texture
                            (*textureAsset + i) = LoadTexture(assetPath[i]);
                        }
                    }
                    */
                }
            }
        } break;
        case APP_CMD_GAINED_FOCUS:
        {
            CORE.Android.appEnabled = true;
            //ResumeMusicStream();
        } break;
        case APP_CMD_PAUSE: break;
        case APP_CMD_LOST_FOCUS:
        {
            CORE.Android.appEnabled = false;
            //PauseMusicStream();
        } break;
        case APP_CMD_TERM_WINDOW:
        {
            // Dettach OpenGL context and destroy display surface
            // NOTE 1: Detaching context before destroying display surface avoids losing our resources (textures, shaders, VBOs...)
            // NOTE 2: In some cases (too many context loaded), OS could unload context automatically... :(
            eglMakeCurrent(CORE.Window.device, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
            eglDestroySurface(CORE.Window.device, CORE.Window.surface);

            CORE.Android.contextRebindRequired = true;
        } break;
        case APP_CMD_SAVE_STATE: break;
        case APP_CMD_STOP: break;
        case APP_CMD_DESTROY:
        {
            // TODO: Finish activity?
            //ANativeActivity_finish(CORE.Android.app->activity);
        } break;
        case APP_CMD_CONFIG_CHANGED:
        {
            //AConfiguration_fromAssetManager(CORE.Android.app->config, CORE.Android.app->activity->assetManager);
            //print_cur_config(CORE.Android.app);

            // Check screen orientation here!
        } break;
        default: break;
    }
}

// ANDROID: Get input events
static int32_t AndroidInputCallback(struct android_app *app, AInputEvent *event)
{
    // If additional inputs are required check:
    // https://developer.android.com/ndk/reference/group/input
    // https://developer.android.com/training/game-controllers/controller-input

    int type = AInputEvent_getType(event);
    int source = AInputEvent_getSource(event);

    if (type == AINPUT_EVENT_TYPE_MOTION)
    {
        if (((source & AINPUT_SOURCE_JOYSTICK) == AINPUT_SOURCE_JOYSTICK) ||
            ((source & AINPUT_SOURCE_GAMEPAD) == AINPUT_SOURCE_GAMEPAD))
        {
            // Get first touch position
            CORE.Input.Touch.position[0].x = AMotionEvent_getX(event, 0);
            CORE.Input.Touch.position[0].y = AMotionEvent_getY(event, 0);

            // Get second touch position
            CORE.Input.Touch.position[1].x = AMotionEvent_getX(event, 1);
            CORE.Input.Touch.position[1].y = AMotionEvent_getY(event, 1);

            int32_t keycode = AKeyEvent_getKeyCode(event);
            if (AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_DOWN)
            {
                CORE.Input.Keyboard.currentKeyState[keycode] = 1;   // Key down

                CORE.Input.Keyboard.keyPressedQueue[CORE.Input.Keyboard.keyPressedQueueCount] = keycode;
                CORE.Input.Keyboard.keyPressedQueueCount++;
            }
            else CORE.Input.Keyboard.currentKeyState[keycode] = 0;  // Key up

            // Stop processing gamepad buttons
            return 1;
        }
    }
    else if (type == AINPUT_EVENT_TYPE_KEY)
    {
        int32_t keycode = AKeyEvent_getKeyCode(event);
        //int32_t AKeyEvent_getMetaState(event);

        // Save current button and its state
        // NOTE: Android key action is 0 for down and 1 for up
        if (AKeyEvent_getAction(event) == AKEY_EVENT_ACTION_DOWN)
        {
            CORE.Input.Keyboard.currentKeyState[keycode] = 1;   // Key down

            CORE.Input.Keyboard.keyPressedQueue[CORE.Input.Keyboard.keyPressedQueueCount] = keycode;
            CORE.Input.Keyboard.keyPressedQueueCount++;
        }
        else CORE.Input.Keyboard.currentKeyState[keycode] = 0;  // Key up

        if (keycode == AKEYCODE_POWER)
        {
            // Let the OS handle input to avoid app stuck. Behaviour: CMD_PAUSE -> CMD_SAVE_STATE -> CMD_STOP -> CMD_CONFIG_CHANGED -> CMD_LOST_FOCUS
            // Resuming Behaviour: CMD_START -> CMD_RESUME -> CMD_CONFIG_CHANGED -> CMD_CONFIG_CHANGED -> CMD_GAINED_FOCUS
            // It seems like locking mobile, screen size (CMD_CONFIG_CHANGED) is affected.
            // NOTE: AndroidManifest.xml must have <activity android:configChanges="orientation|keyboardHidden|screenSize" >
            // Before that change, activity was calling CMD_TERM_WINDOW and CMD_DESTROY when locking mobile, so that was not a normal behaviour
            return 0;
        }
        else if ((keycode == AKEYCODE_BACK) || (keycode == AKEYCODE_MENU))
        {
            // Eat BACK_BUTTON and AKEYCODE_MENU, just do nothing... and don't let to be handled by OS!
            return 1;
        }
        else if ((keycode == AKEYCODE_VOLUME_UP) || (keycode == AKEYCODE_VOLUME_DOWN))
        {
            // Set default OS behaviour
            return 0;
        }

        return 0;
    }

    CORE.Input.Touch.position[0].x = AMotionEvent_getX(event, 0);
    CORE.Input.Touch.position[0].y = AMotionEvent_getY(event, 0);

    int32_t action = AMotionEvent_getAction(event);
    unsigned int flags = action & AMOTION_EVENT_ACTION_MASK;

    if (flags == AMOTION_EVENT_ACTION_DOWN || flags == AMOTION_EVENT_ACTION_MOVE)
    {
        CORE.Input.Touch.currentTouchState[MOUSE_BUTTON_LEFT] = 1;
    }
    else if (flags == AMOTION_EVENT_ACTION_UP)
    {
        CORE.Input.Touch.currentTouchState[MOUSE_BUTTON_LEFT] = 0;
    }

#if defined(SUPPORT_GESTURES_SYSTEM)

    GestureEvent gestureEvent = { 0 };

    // Register touch actions
    if (flags == AMOTION_EVENT_ACTION_DOWN) gestureEvent.touchAction = TOUCH_DOWN;
    else if (flags == AMOTION_EVENT_ACTION_UP) gestureEvent.touchAction = TOUCH_UP;
    else if (flags == AMOTION_EVENT_ACTION_MOVE) gestureEvent.touchAction = TOUCH_MOVE;

    // Register touch points count
    // NOTE: Documentation says pointerCount is Always >= 1,
    // but in practice it can be 0 or over a million
    gestureEvent.pointCount = AMotionEvent_getPointerCount(event);

    // Only enable gestures for 1-3 touch points
    if ((gestureEvent.pointCount > 0) && (gestureEvent.pointCount < 4))
    {
        // Register touch points id
        // NOTE: Only two points registered
        gestureEvent.pointerId[0] = AMotionEvent_getPointerId(event, 0);
        gestureEvent.pointerId[1] = AMotionEvent_getPointerId(event, 1);

        // Register touch points position
        gestureEvent.position[0] = (Vector2){ AMotionEvent_getX(event, 0), AMotionEvent_getY(event, 0) };
        gestureEvent.position[1] = (Vector2){ AMotionEvent_getX(event, 1), AMotionEvent_getY(event, 1) };

        // Normalize gestureEvent.position[x] for screenWidth and screenHeight
        gestureEvent.position[0].x /= (float)GetScreenWidth();
        gestureEvent.position[0].y /= (float)GetScreenHeight();

        gestureEvent.position[1].x /= (float)GetScreenWidth();
        gestureEvent.position[1].y /= (float)GetScreenHeight();

        // Gesture data is sent to gestures system for processing
        ProcessGestureEvent(gestureEvent);
    }
#endif

    return 0;
}
#endif

#if defined(PLATFORM_WEB)
// Register touch input events
static EM_BOOL EmscriptenTouchCallback(int eventType, const EmscriptenTouchEvent *touchEvent, void *userData)
{
    for (int i = 0; i < touchEvent->numTouches; i++)
    {
        if (eventType == EMSCRIPTEN_EVENT_TOUCHSTART) CORE.Input.Touch.currentTouchState[i] = 1;
        else if (eventType == EMSCRIPTEN_EVENT_TOUCHEND) CORE.Input.Touch.currentTouchState[i] = 0;
    }

#if defined(SUPPORT_GESTURES_SYSTEM)
    GestureEvent gestureEvent = { 0 };

    // Register touch actions
    if (eventType == EMSCRIPTEN_EVENT_TOUCHSTART) gestureEvent.touchAction = TOUCH_DOWN;
    else if (eventType == EMSCRIPTEN_EVENT_TOUCHEND) gestureEvent.touchAction = TOUCH_UP;
    else if (eventType == EMSCRIPTEN_EVENT_TOUCHMOVE) gestureEvent.touchAction = TOUCH_MOVE;

    // Register touch points count
    gestureEvent.pointCount = touchEvent->numTouches;

    // Register touch points id
    gestureEvent.pointerId[0] = touchEvent->touches[0].identifier;
    gestureEvent.pointerId[1] = touchEvent->touches[1].identifier;

    // Register touch points position
    // NOTE: Only two points registered
    gestureEvent.position[0] = (Vector2){ touchEvent->touches[0].targetX, touchEvent->touches[0].targetY };
    gestureEvent.position[1] = (Vector2){ touchEvent->touches[1].targetX, touchEvent->touches[1].targetY };

    double canvasWidth, canvasHeight;
    // NOTE: emscripten_get_canvas_element_size() returns canvas.width and canvas.height but
    // we are looking for actual CSS size: canvas.style.width and canvas.style.height
    //EMSCRIPTEN_RESULT res = emscripten_get_canvas_element_size("#canvas", &canvasWidth, &canvasHeight);
    emscripten_get_element_css_size("#canvas", &canvasWidth, &canvasHeight);

    // Normalize gestureEvent.position[x] for CORE.Window.screen.width and CORE.Window.screen.height
    gestureEvent.position[0].x *= ((float)GetScreenWidth()/(float)canvasWidth);
    gestureEvent.position[0].y *= ((float)GetScreenHeight()/(float)canvasHeight);
    gestureEvent.position[1].x *= ((float)GetScreenWidth()/(float)canvasWidth);
    gestureEvent.position[1].y *= ((float)GetScreenHeight()/(float)canvasHeight);

    CORE.Input.Touch.position[0] = gestureEvent.position[0];
    CORE.Input.Touch.position[1] = gestureEvent.position[1];

    // Gesture data is sent to gestures system for processing
    ProcessGestureEvent(gestureEvent);
#else
    // Support only simple touch position
    if (eventType == EMSCRIPTEN_EVENT_TOUCHSTART)
    {
        // Get first touch position
        CORE.Input.Touch.position[0] = (Vector2){ touchEvent->touches[0].targetX, touchEvent->touches[0].targetY };

        double canvasWidth, canvasHeight;
        //EMSCRIPTEN_RESULT res = emscripten_get_canvas_element_size("#canvas", &canvasWidth, &canvasHeight);
        emscripten_get_element_css_size("#canvas", &canvasWidth, &canvasHeight);

        // Normalize gestureEvent.position[x] for screenWidth and screenHeight
        CORE.Input.Touch.position[0].x *= ((float)GetScreenWidth()/(float)canvasWidth);
        CORE.Input.Touch.position[0].y *= ((float)GetScreenHeight()/(float)canvasHeight);
    }
#endif

    return 1;
}

// Register connected/disconnected gamepads events
static EM_BOOL EmscriptenGamepadCallback(int eventType, const EmscriptenGamepadEvent *gamepadEvent, void *userData)
{
    /*
    TRACELOGD("%s: timeStamp: %g, connected: %d, index: %ld, numAxes: %d, numButtons: %d, id: \"%s\", mapping: \"%s\"",
           eventType != 0? emscripten_event_type_to_string(eventType) : "Gamepad state",
           gamepadEvent->timestamp, gamepadEvent->connected, gamepadEvent->index, gamepadEvent->numAxes, gamepadEvent->numButtons, gamepadEvent->id, gamepadEvent->mapping);

    for (int i = 0; i < gamepadEvent->numAxes; ++i) TRACELOGD("Axis %d: %g", i, gamepadEvent->axis[i]);
    for (int i = 0; i < gamepadEvent->numButtons; ++i) TRACELOGD("Button %d: Digital: %d, Analog: %g", i, gamepadEvent->digitalButton[i], gamepadEvent->analogButton[i]);
    */

    if ((gamepadEvent->connected) && (gamepadEvent->index < MAX_GAMEPADS)) CORE.Input.Gamepad.ready[gamepadEvent->index] = true;
    else CORE.Input.Gamepad.ready[gamepadEvent->index] = false;

    // TODO: Test gamepadEvent->index

    return 0;
}
#endif