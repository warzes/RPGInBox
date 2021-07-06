#include "stdafx.h"
#include "Platform.h"
#include "DebugNew.h"

//=============================================================================
// Window
//=============================================================================
//-----------------------------------------------------------------------------
// GLFW3: Error callback
static void ErrorCallback(int error, const char* description)
{
	puts(description);
}
//-----------------------------------------------------------------------------
struct Window::pimpl
{
	//GLFWwindow* window = nullptr;
};
//-----------------------------------------------------------------------------
Window::Window(Log& log) noexcept
	: m_log(log)
{
	m_impl = new Window::pimpl;
}
//-----------------------------------------------------------------------------
Window::~Window()
{
	delete m_impl;
#if SE_PLATFORM_DESKTOP || SE_PLATFORM_WEB
	//glfwDestroyWindow(m_impl->window);
	//glfwTerminate();
#endif
}
//-----------------------------------------------------------------------------
bool Window::init(const WindowConfig& config) noexcept
{
	m_width = config.width;
	m_height = config.height;
	m_fullscreen = config.fullscreen;

//#if SE_PLATFORM_DESKTOP || SE_PLATFORM_WEB
//	glfwSetErrorCallback(ErrorCallback);
//
//	if (!glfwInit())
//	{
//		m_log.Error("GLFW: Failed to initialize GLFW");
//		return false;
//	}
//
//	// NOTE: Getting video modes is not implemented in emscripten GLFW3 version
//#if SE_PLATFORM_DESKTOP
//	// Find monitor resolution
//	GLFWmonitor* monitor = glfwGetPrimaryMonitor();
//	if (!monitor)
//	{
//		m_log.Error("GLFW: Failed to get primary monitor");
//		return false;
//	}
//	const GLFWvidmode* mode = glfwGetVideoMode(monitor);
//
//	m_displayWidth = mode->width;
//	m_displayHeight = mode->height;
//
//	// Set screen width/height to the display width/height if they are 0
//	if (m_width == 0) m_width = m_displayWidth;
//	if (m_height == 0) m_height = m_displayHeight;
//#endif  // SE_PLATFORM_DESKTOP
//
//#if SE_PLATFORM_WEB
//	m_displayWidth = m_width;
//	m_displayHeight = m_height;
//#endif  // SE_PLATFORM_WEB
//
//	glfwDefaultWindowHints(); // Set default windows hints
//	glfwWindowHint(GLFW_RESIZABLE, config.resizable ? GLFW_TRUE : GLFW_FALSE);
//
//	glfwWindowHint(GLFW_SCALE_TO_MONITOR, GLFW_FALSE); // disable HighDPI
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
//	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
//	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
//	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
//	// OpenGL ES 2.0
////	{
////		glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
////		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
////		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
////#if SE_PLATFORM_DESKTOP
////		glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_EGL_CONTEXT_API);
////#else
////		glfwWindowHint(GLFW_CONTEXT_CREATION_API, GLFW_NATIVE_CONTEXT_API);
////#endif
////	}
//
//	if (m_fullscreen)
//	{
//		// remember center for switchinging from fullscreen to window
//		m_positionX = m_displayWidth / 2 - m_width / 2;
//		m_positionY = m_displayHeight / 2 - m_height / 2;
//
//		if (m_positionX < 0) m_positionX = 0;
//		if (m_positionY < 0) m_positionY = 0;
//
//		// Obtain recommended m_displayWidth/m_displayHeight from a valid videomode for the monitor
//		int count = 0;
//		const GLFWvidmode* modes = glfwGetVideoModes(glfwGetPrimaryMonitor(), &count);
//
//		// Get closest video mode to desired m_width/m_height
//		for (int i = 0; i < count; i++)
//		{
//			if ((unsigned int)modes[i].width >= m_width)
//			{
//				if ((unsigned int)modes[i].height >= m_height)
//				{
//					m_displayWidth = modes[i].width;
//					m_displayHeight = modes[i].height;
//					break;
//				}
//			}
//		}
//
//#if SE_PLATFORM_DESKTOP
//		// If we are windowed fullscreen, ensures that window does not minimize when focus is lost
//		if ((m_height == m_displayHeight) && (m_width == m_displayWidth))
//			glfwWindowHint(GLFW_AUTO_ICONIFY, 0);
//#endif
//		// NOTE: ISSUE: Closest videomode could not match monitor aspect-ratio, for example,
//		// for a desired screen size of 800x450 (16:9), closest supported videomode is 800x600 (4:3),
//		// framebuffer is rendered correctly but once displayed on a 16:9 monitor, it gets stretched
//		// by the sides to fit all monitor space...
//
//		// Try to setup the most appropiate fullscreen framebuffer for the requested screenWidth/screenHeight
//		// It considers device display resolution mode and setups a framebuffer with black bars if required (render size/offset)
//		// Modified global variables: m_width/m_height - CORE.Window.render.width/CORE.Window.render.height - CORE.Window.renderOffset.x/CORE.Window.renderOffset.y - CORE.Window.screenScale
//		// TODO: It is a quite cumbersome solution to display size vs requested size, it should be reviewed or removed...
//		// HighDPI monitors are properly considered in a following similar function: SetupViewport()
//		setupFramebuffer(m_displayWidth, m_displayHeight);
//
//		m_impl->window = glfwCreateWindow(m_displayWidth, m_displayHeight, "Game", glfwGetPrimaryMonitor(), nullptr);
//	}
//	else
//	{
//		// No-fullscreen window creation
//		m_impl->window = glfwCreateWindow(m_width, m_height, "Game", nullptr, nullptr);
//
//#if SE_PLATFORM_DESKTOP
//		// Center window on screen
//		int windowPosX = m_displayWidth / 2 - m_width / 2;
//		int windowPosY = m_displayHeight / 2 - m_height / 2;
//		if (windowPosX < 0) windowPosX = 0;
//		if (windowPosY < 0) windowPosY = 0;
//		if (!m_impl->window)
//			glfwSetWindowPos(m_impl->window, windowPosX, windowPosY);
//#endif
//	}
//	// Check for Valid Context
//	if (m_impl->window == nullptr)
//	{
//		m_log.Error("GLFW: Failed to initialize Window");
//		return false;
//	}
//
//	// Set window callback events
////	glfwSetWindowSizeCallback(m_impl->window, WindowSizeCallback);      // NOTE: Resizing not allowed by default!
////#if !SE_PLATFORM_WEB
////	glfwSetWindowMaximizeCallback(m_impl->window, WindowMaximizeCallback);
////#endif
////	glfwSetWindowIconifyCallback(m_impl->window, WindowIconifyCallback);
////	glfwSetWindowFocusCallback(m_impl->window, WindowFocusCallback);
////	glfwSetDropCallback(m_impl->window, WindowDropCallback);
////	// Set input callback events
////	glfwSetKeyCallback(m_impl->window, KeyCallback);
////	glfwSetCharCallback(m_impl->window, CharCallback);
////	glfwSetMouseButtonCallback(m_impl->window, MouseButtonCallback);
////	glfwSetCursorPosCallback(m_impl->window, MouseCursorPosCallback);   // Track mouse position changes
////	glfwSetScrollCallback(m_impl->window, MouseScrollCallback);
////	glfwSetCursorEnterCallback(m_impl->window, CursorEnterCallback);
//
//	// Create Context and Load OpenGL Functions
//	glfwMakeContextCurrent(m_impl->window);
//
//	glfwSwapInterval(config.vsync ? 1 : 0);
//
//#endif  // SE_PLATFORM_DESKTOP || SE_PLATFORM_WEB
//
//#if SE_PLATFORM_ANDROID
//	m_fullscreen = true;
//#error TODO
//#endif // SE_PLATFORM_ANDROID
//
//#if SE_PLATFORM_DESKTOP || SE_PLATFORM_WEB
//	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//#else
//	if (!gladLoadGLLoader((GLADloadproc)eglGetProcAddress))
//#endif
//	{
//		m_log.Error("GLAD: Cannot load OpenGL extensions");
//		return false;
//	}
//
//	gladLoadGL();
//
//	m_isClose = false;


	return true;
}
// Compute framebuffer size relative to screen size and display size
// NOTE: Global variables CORE.Window.render.width/CORE.Window.render.height and CORE.Window.renderOffset.x/CORE.Window.renderOffset.y can be modified
//-----------------------------------------------------------------------------
void Window::setupFramebuffer(int width, int height) noexcept
{
	// TODO: raylib SetupFramebuffer
}
//-----------------------------------------------------------------------------
void Window::update() noexcept
{
	//m_isClose = glfwWindowShouldClose(m_impl->window);
	//glfwSwapBuffers(m_impl->window);
	//glfwPollEvents();
}
//-----------------------------------------------------------------------------