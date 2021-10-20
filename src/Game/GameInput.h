#pragma once

enum class GameKey
{
	Up,
	Down,
	Left,
	Right,
	StrafeLeft,
	StrafeRight,

	Ok,
	Cancel,
	Menu,

	K1,
	K2,
	K3,
	K4,
	K5,
	K6,
};

class Input final
{
public:
	static bool IsDown(GameKey key) noexcept;
	static bool IsUp(GameKey key) noexcept;
	static bool IsPressed(GameKey key) noexcept;
	static bool IsReleased(GameKey key) noexcept;
};