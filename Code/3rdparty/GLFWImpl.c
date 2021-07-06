#define _CRT_SECURE_NO_WARNINGS

#if defined(_MSC_VER)
#	pragma warning(push, 0)
#	pragma warning(disable:4820)
#	pragma warning(disable:4255)
#	pragma warning(disable:5105)
#	pragma warning(disable:5045)
#endif

//=============================================================================
// GLFW Implementation
//=============================================================================

//http://www.glfw.org/docs/latest/compile.html#compile_manual

#if defined(_WIN32)
#define _GLFW_WIN32
#endif
#if defined(__linux__)
#if !defined(_GLFW_WAYLAND)     // Required for Wayland windowing
#define _GLFW_X11
#endif
#endif
#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined(__NetBSD__) || defined(__DragonFly__)
#define _GLFW_X11
#endif
#if defined(__APPLE__)
#define _GLFW_COCOA
#define _GLFW_USE_MENUBAR       // To create and populate the menu bar when the first window is created
#define _GLFW_USE_RETINA        // To have windows use the full resolution of Retina displays
#endif
#if defined(__TINYC__)
#define _WIN32_WINNT_WINXP      0x0501
#endif

// NOTE: _GLFW_MIR experimental platform not supported at this moment

#include "glfw/context.c"
#include "glfw/init.c"
#include "glfw/input.c"
#include "glfw/monitor.c"
#include "glfw/vulkan.c"
#include "glfw/window.c"

#if defined(_WIN32)
#include "glfw/win32_init.c"
#include "glfw/win32_joystick.c"
#include "glfw/win32_monitor.c"
#include "glfw/win32_time.c"
#include "glfw/win32_thread.c"
#include "glfw/win32_window.c"
#include "glfw/wgl_context.c"
#include "glfw/egl_context.c"
#include "glfw/osmesa_context.c"
#endif

#if defined(__linux__)
#if defined(_GLFW_WAYLAND)
#include "glfw/wl_init.c"
#include "glfw/wl_monitor.c"
#include "glfw/wl_window.c"
#include "glfw/wayland-pointer-constraints-unstable-v1-client-protocol.c"
#include "glfw/wayland-relative-pointer-unstable-v1-client-protocol.c"
#endif
#if defined(_GLFW_X11)
#include "glfw/x11_init.c"
#include "glfw/x11_monitor.c"
#include "glfw/x11_window.c"
#include "glfw/glx_context.c"
#endif

#include "glfw/linux_joystick.c"
#include "glfw/posix_thread.c"
#include "glfw/posix_time.c"
#include "glfw/xkb_unicode.c"
#include "glfw/egl_context.c"
#include "glfw/osmesa_context.c"
#endif

#if defined(__FreeBSD__) || defined(__OpenBSD__) || defined( __NetBSD__) || defined(__DragonFly__)
#include "glfw/x11_init.c"
#include "glfw/x11_monitor.c"
#include "glfw/x11_window.c"
#include "glfw/xkb_unicode.c"
// TODO: Joystick implementation
#include "glfw/null_joystick.c"
#include "glfw/posix_time.c"
#include "glfw/posix_thread.c"
#include "glfw/glx_context.c"
#include "glfw/egl_context.c"
#include "glfw/osmesa_context.c"
#endif

#if defined(__APPLE__)
#include "glfw/cocoa_init.m"
#include "glfw/cocoa_joystick.m"
#include "glfw/cocoa_monitor.m"
#include "glfw/cocoa_window.m"
#include "glfw/cocoa_time.c"
#include "glfw/posix_thread.c"
#include "glfw/nsgl_context.m"
#include "glfw/egl_context.c"
#include "glfw/osmesa_context.c"
#endif

#if defined(_MSC_VER)
#	pragma warning(pop)
#endif