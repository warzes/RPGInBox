#pragma once

// In MSVC debug mode, override the global new operator to include file and line logging. This file should be included last in compilation units. Will break using placement new.
#if SE_DEBUG && SE_COMPILER_MSVC && SE_PLATFORM_WINDOWS
#	define _CRTDBG_MAP_ALLOC
#	undef _malloca
#	include <crtdbg.h>
#	define ___DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#	define new ___DEBUG_NEW
#endif