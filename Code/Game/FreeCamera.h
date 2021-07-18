#pragma once

#include "IGameCamera.h"

class FreeCamera final : public IGameCamera
{
public:
	void Setup(const float fovY, const Vector3& position, const float currentRotateY = 0.0f) noexcept;
	void Update(World& world) noexcept final;

	Vector3 MoveSpeed = { 1.0f, 1.0f, 1.0f };
	Vector2 TurnSpeed = { 90.0f, 90.0f };

	float MouseSensitivity = 600.0f;

	float MinimumViewY = -65.0f;
	float MaximumViewY = 89.0f;

	bool UseMouseX = true;
	bool UseMouseY = true;
	bool HideCursor = true;
	bool UseKeyboard = true;

private:
	float getSpeedForAxis(CameraControls axis, float speed) noexcept;

	bool m_windowFocused = true;
	Vector2 m_previousMousePosition = { 0.0f, 0.0f };
	Vector2 m_angle = { 0.0f, 0.0f };                // Camera angle in plane XZ
};