#pragma once

#include "Base.h"

SE_PRAGMA_WARNING_DISABLE_MSVC(4514)
SE_PRAGMA_WARNING_DISABLE_MSVC(4625)
SE_PRAGMA_WARNING_DISABLE_MSVC(4626)
SE_PRAGMA_WARNING_DISABLE_MSVC(4820)
SE_PRAGMA_WARNING_DISABLE_MSVC(5026)
SE_PRAGMA_WARNING_DISABLE_MSVC(5027)
SE_PRAGMA_WARNING_DISABLE_MSVC(5045)
SE_PRAGMA_WARNING_PUSH_LEVEL(0)

#include <raylib/raylib.h>
#include <raylib/utils.h>
#include <raylib/rlgl.h>
#include <raylib/raymath.h>

#include <cassert>
#include <cstdint>
#include <fstream>
#include <chrono>
#include <string>
#include <algorithm>
#include <functional>
#include <array>
#include <vector>
#include <map>
#include <unordered_map>

SE_PRAGMA_WARNING_POP

#include "ConfigDefine.h"
#include "NonCopyable.h"
#include "STLUtils.h"
#include "EngineMath.h"