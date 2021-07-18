#pragma once

class Frustum final
{
public:
	constexpr static size_t MaxFrustumPlanes = 6;
	enum class FrustumPlanes
	{
		Back   = 0,
		Front  = 1,
		Bottom = 2,
		Top    = 3,
		Right  = 4,
		Left   = 5
	};

	Frustum() noexcept;

	void Extract() noexcept;

	bool PointIn(const Vector3& position) const noexcept;
	bool PointIn(float x, float y, float z) const noexcept;

	bool SphereIn(const Vector3& position, float radius) const noexcept;
	bool AABBoxIn(const Vector3& min, const Vector3& max) const noexcept;

	Vector4 Planes[MaxFrustumPlanes];
};