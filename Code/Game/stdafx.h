#pragma once

#include <Engine/Base.h>

SE_PRAGMA_WARNING_DISABLE_MSVC(4514)
SE_PRAGMA_WARNING_DISABLE_MSVC(4625)
SE_PRAGMA_WARNING_DISABLE_MSVC(4626)
SE_PRAGMA_WARNING_DISABLE_MSVC(4820)
SE_PRAGMA_WARNING_DISABLE_MSVC(5026)
SE_PRAGMA_WARNING_DISABLE_MSVC(5027)
SE_PRAGMA_WARNING_PUSH_LEVEL(0)

#include <raylib/raylib.h>
#include <raylib/rayutils.h>
#include <raylib/rlgl.h>
#include <raylib/raymath.h>

#include <terminal.h>

#include <cstdint>
#include <fstream>
#include <chrono>
#include <string>
#include <functional>

SE_PRAGMA_WARNING_POP

#include <Engine/Engine.h>
#include <Engine/Frustum.h>

#include "ConfigDefine.h"