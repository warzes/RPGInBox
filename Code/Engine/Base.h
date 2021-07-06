#pragma once

//=============================================================================
// Architecture
//=============================================================================
#define SE_ARCH_32BIT 0
#define SE_ARCH_64BIT 0
#if defined(__x86_64__)    || \
	defined(_M_X64)        || \
	defined(__aarch64__)   || \
	defined(__64BIT__)     || \
	defined(__mips64)      || \
	defined(__powerpc64__) || \
	defined(__ppc64__)     || \
	defined(__LP64__)
#	undef  SE_ARCH_64BIT
#	define SE_ARCH_64BIT 64
#else
#	undef  SE_ARCH_32BIT
#	define SE_ARCH_32BIT 32
#endif


//=============================================================================
// DEBUG MODE
//=============================================================================
#define SE_DEBUG 0
#if (defined(_DEBUG) || defined(DEBUG)) && !defined(NDEBUG)
#	undef  SE_DEBUG
#	define SE_DEBUG 1
#endif

//=============================================================================
// Compiler
//=============================================================================
#define SE_COMPILER_MSVC 0
#if defined(_MSC_VER)
#	undef  SE_COMPILER_MSVC
#	define SE_COMPILER_MSVC _MSC_VER
#else
#	error "Unknown compiler."
#endif

//=============================================================================
// Platform OS
//=============================================================================
#define SE_PLATFORM_ANDROID 0
#define SE_PLATFORM_EMSCRIPTEN 0
#define SE_PLATFORM_LINUX 0
#define SE_PLATFORM_WINDOWS 0
#if defined(_WIN32) || defined(_WIN64)
#	undef  SE_PLATFORM_WINDOWS
#	define SE_PLATFORM_WINDOWS 1
#elif defined(__ANDROID__)
#	undef  SE_PLATFORM_ANDROID
#	define SE_PLATFORM_ANDROID __ANDROID_API__
#elif  defined(__linux__)
#	undef  SE_PLATFORM_LINUX
#	define SE_PLATFORM_LINUX 1
#elif defined(__EMSCRIPTEN__)
#	undef  SE_PLATFORM_EMSCRIPTEN
#	define SE_PLATFORM_EMSCRIPTEN 1
#else
#	error Unknown platform.
#endif

//=============================================================================
// Macros Utility
//=============================================================================
#define SE_STRINGIZE( _n ) SE_STRINGIZE_2( _n )
#define SE_STRINGIZE_2( _n ) #_n

//=============================================================================
// Macros TODO
//=============================================================================
#if SE_COMPILER_MSVC
#	define TODO( _msg ) __pragma(message("" __FILE__ "(" SE_STRINGIZE(__LINE__) "): TODO: " _msg))
#else
#	define TODO( _msg )
#endif

//=============================================================================
// Pragma Warning
//=============================================================================
#if SE_COMPILER_MSVC
#	define SE_PRAGMA_WARNING_PUSH             __pragma(warning(push))
#	define SE_PRAGMA_WARNING_POP              __pragma(warning(pop))
#	define SE_PRAGMA_WARNING_DISABLE_MSVC(id) __pragma(warning(disable: id))
#	define SE_PRAGMA_WARNING_DISABLE_CLANG(id)
#	define SE_PRAGMA_WARNING_DISABLE_GCC(id)
#elif SE_COMPILER_CLANG
#	define SE_PRAGMA_WARNING_PUSH             _Pragma("clang diagnostic push")
#	define SE_PRAGMA_WARNING_POP              _Pragma("clang diagnostic pop")
#	define SE_PRAGMA_WARNING_DISABLE_MSVC(id)
#	define SE_PRAGMA_WARNING_DISABLE_CLANG(id)_Pragma(SE_STRINGIZE_2(clang diagnostic ignored id) )
#	define SE_PRAGMA_WARNING_DISABLE_GCC(id)
#elif SE_COMPILER_GNUC
#	define SE_PRAGMA_WARNING_PUSH             _Pragma("GCC diagnostic push")
#	define SE_PRAGMA_WARNING_POP              _Pragma("GCC diagnostic pop")
#	define SE_PRAGMA_WARNING_DISABLE_MSVC(id)
#	define SE_PRAGMA_WARNING_DISABLE_CLANG(id)
#	define SE_PRAGMA_WARNING_DISABLE_GCC(id)  _Pragma(SE_STRINGIZE_2(GCC diagnostic ignored id) )
#endif

//=============================================================================
// Platform macros
//=============================================================================

#if SE_PLATFORM_WINDOWS
// Set Windows version to Windows 7 (0x0601)
#	ifdef _WIN32_WINNT
#		undef _WIN32_WINNT
#	endif
#	define _WIN32_WINNT 0x0601
#	ifdef WINVER
#		undef WINVER
#	endif
#	define WINVER _WIN32_WINNT

#	define WIN32_LEAN_AND_MEAN      // 
#	define WIN32_EXTRA_LEAN         // 
#	define VC_EXTRALEAN             // 
#	define NOIME                    // 
#	define NOWINRES                 // 
#	define NOGDICAPMASKS            // CC_*, LC_*, PC_*, CP_*, TC_*, RC_
#	define NOVIRTUALKEYCODES        // VK_*
#	define NOWINMESSAGES            // WM_*, EM_*, LB_*, CB_*
#	define NOWINSTYLES              // WS_*, CS_*, ES_*, LBS_*, SBS_*, CBS_*
#	define NOSYSMETRICS             // SM_*
#	define NOMENUS                  // MF_*
#	define NOICONS                  // IDI_*
#	define NOKEYSTATES              // MK_*
#	define NOSYSCOMMANDS            // SC_*
#	define NORASTEROPS              // Binary and Tertiary raster ops
#	define NOSHOWWINDOW             // SW_*
#	define OEMRESOURCE              // OEM Resource values
#	define NOATOM                   // Atom Manager routines
#	define NOCLIPBOARD              // Clipboard routines
#	define NOCOLOR                  // Screen colors
#	define NOCTLMGR                 // Control and Dialog routines
#	define NODRAWTEXT               // DrawText() and DT_*
#	define NOGDI                    // All GDI defines and routines
#	define NOBITMAP                 // 
#	define NOUSER                   // All USER defines and routines
#	define NOMB                     // MB_* and MessageBox()
#	define NOMEMMGR                 // GMEM_*, LMEM_*, GHND, LHND, associated routines
#	define NOMETAFILE               // typedef METAFILEPICT
#	define NOMINMAX                 // Macros min(a, b) and max(a, b)
#	define NOMSG                    // typedef MSG and associated routines
#	define NOOPENFILE               // OpenFile(), OemToAnsi, AnsiToOem, and OF_*
#	define NOSCROLL                 // SB_* and scrolling routines
#	define NOSERVICE                // All Service Controller routines, SERVICE_ equates, etc.
#	define NOSOUND                  // Sound driver routines
#	define NOTEXTMETRIC             // typedef TEXTMETRIC and associated routines
#	define NOWH                     // SetWindowsHook and WH_*
#	define NOWINOFFSETS             // GWL_*, GCL_*, associated routines
#	define NOCOMM                   // COMM driver routines
#	define NOKANJI                  // Kanji support stuff.
#	define NOHELP                   // Help engine interface. (WinHelp is deprecated)
#	define NOPROFILER               // Profiler interface.
#	define NODEFERWINDOWPOS         // DeferWindowPos routines
#	define NOMCX                    // Modem Configuration Extensions
#	define NOPROXYSTUB              // 
#	define NOIMAGE                  // 
#	define NO                       // 
#	define NOTAPE                   // 
#	define ANSI_ONLY                // 
#	define NOCRYPT                  // 
#	define NOKERNEL                 // All KERNEL defines and routines
#	define NONLS                    // All NLS defines and routines
#endif

//=============================================================================
// inline global func
//=============================================================================

// Implementation from "08/02/2015 Better array 'countof' implementation with C++ 11 (updated)" - https://www.g-truc.net/post-0708.html
template<typename T, size_t N>
[[nodiscard]] constexpr size_t Countof(T const (&)[N])
{
	return N;
}

