#include "stdafx.h"
#include "GameInput.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
template<typename Func> 
inline bool keysFuncs(Func func, GameKey key) noexcept
{
	switch (key)
	{
	case GameKey::Up:          if (func(KEY_W) || func(KEY_UP)) return true; break;
	case GameKey::Down:        if (func(KEY_S) || func(KEY_DOWN)) return true; break;
	case GameKey::Left:        if (func(KEY_A) || func(KEY_LEFT)) return true; break;
	case GameKey::Right:       if (func(KEY_D) || func(KEY_RIGHT)) return true; break;
	case GameKey::StrafeLeft:  if (func(KEY_Q) || func(KEY_RIGHT_CONTROL)) return true; break;
	case GameKey::StrafeRight: if (func(KEY_E) || func(KEY_KP_0)) return true; break;

	case GameKey::Ok:          if (func(KEY_Z) || func(KEY_SPACE) || func(KEY_ENTER)) return true; break;
	case GameKey::Cancel:      if (func(KEY_X) || func(KEY_ESCAPE)) return true; break;
	case GameKey::Menu:        if (func(KEY_C) || func(KEY_TAB) || func(KEY_I)) return true; break;

	case GameKey::K1:          if (func(KEY_ONE)) return true; break;
	case GameKey::K2:          if (func(KEY_TWO)) return true; break;
	case GameKey::K3:          if (func(KEY_THREE)) return true; break;
	case GameKey::K4:          if (func(KEY_FOUR)) return true; break;
	case GameKey::K5:          if (func(KEY_FIVE)) return true; break;
	case GameKey::K6:          if (func(KEY_SIX)) return true; break;

	default: break;
	}

	return false;
}
//-----------------------------------------------------------------------------
bool Input::IsDown(GameKey key) noexcept
{
	return keysFuncs(IsKeyDown, key);
}
//-----------------------------------------------------------------------------
bool Input::IsUp(GameKey key) noexcept
{
	return keysFuncs(IsKeyUp, key);
}
//-----------------------------------------------------------------------------
bool Input::IsPressed(GameKey key) noexcept
{
	return keysFuncs(IsKeyPressed, key);
}
//-----------------------------------------------------------------------------
bool Input::IsReleased(GameKey key) noexcept
{
	return keysFuncs(IsKeyReleased, key);
}
//-----------------------------------------------------------------------------