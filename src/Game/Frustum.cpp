#include "stdafx.h"
#include "Frustum.h"
#include "DebugNew.h"
//-----------------------------------------------------------------------------
inline void normalizePlane(Vector4& plane)
{
	const float magnitude = sqrtf(plane.x * plane.x + plane.y * plane.y + plane.z * plane.z);
	plane.x /= magnitude;
	plane.y /= magnitude;
	plane.z /= magnitude;
	plane.w /= magnitude;
}
//-----------------------------------------------------------------------------
Frustum::Frustum() noexcept
{
	Planes[static_cast<size_t>(FrustumPlanes::Right)]  = { 0.0f };
	Planes[static_cast<size_t>(FrustumPlanes::Left)]   = { 0.0f };
	Planes[static_cast<size_t>(FrustumPlanes::Top)]    = { 0.0f };
	Planes[static_cast<size_t>(FrustumPlanes::Bottom)] = { 0.0f };
	Planes[static_cast<size_t>(FrustumPlanes::Front)]  = { 0.0f };
	Planes[static_cast<size_t>(FrustumPlanes::Back)]   = { 0.0f };
}
//-----------------------------------------------------------------------------
void Frustum::Extract() noexcept
{
	// TODO: вместо копий матриц сделать ссылки (они берутся из коре движка)
	const Matrix projection = rlGetMatrixProjection();
	const Matrix modelview = rlGetMatrixModelview();

	Matrix planes = { 0 };

	planes.m0 = modelview.m0 * projection.m0 + modelview.m1 * projection.m4 + modelview.m2 * projection.m8 + modelview.m3 * projection.m12;
	planes.m1 = modelview.m0 * projection.m1 + modelview.m1 * projection.m5 + modelview.m2 * projection.m9 + modelview.m3 * projection.m13;
	planes.m2 = modelview.m0 * projection.m2 + modelview.m1 * projection.m6 + modelview.m2 * projection.m10 + modelview.m3 * projection.m14;
	planes.m3 = modelview.m0 * projection.m3 + modelview.m1 * projection.m7 + modelview.m2 * projection.m11 + modelview.m3 * projection.m15;
	planes.m4 = modelview.m4 * projection.m0 + modelview.m5 * projection.m4 + modelview.m6 * projection.m8 + modelview.m7 * projection.m12;
	planes.m5 = modelview.m4 * projection.m1 + modelview.m5 * projection.m5 + modelview.m6 * projection.m9 + modelview.m7 * projection.m13;
	planes.m6 = modelview.m4 * projection.m2 + modelview.m5 * projection.m6 + modelview.m6 * projection.m10 + modelview.m7 * projection.m14;
	planes.m7 = modelview.m4 * projection.m3 + modelview.m5 * projection.m7 + modelview.m6 * projection.m11 + modelview.m7 * projection.m15;
	planes.m8 = modelview.m8 * projection.m0 + modelview.m9 * projection.m4 + modelview.m10 * projection.m8 + modelview.m11 * projection.m12;
	planes.m9 = modelview.m8 * projection.m1 + modelview.m9 * projection.m5 + modelview.m10 * projection.m9 + modelview.m11 * projection.m13;
	planes.m10 = modelview.m8 * projection.m2 + modelview.m9 * projection.m6 + modelview.m10 * projection.m10 + modelview.m11 * projection.m14;
	planes.m11 = modelview.m8 * projection.m3 + modelview.m9 * projection.m7 + modelview.m10 * projection.m11 + modelview.m11 * projection.m15;
	planes.m12 = modelview.m12 * projection.m0 + modelview.m13 * projection.m4 + modelview.m14 * projection.m8 + modelview.m15 * projection.m12;
	planes.m13 = modelview.m12 * projection.m1 + modelview.m13 * projection.m5 + modelview.m14 * projection.m9 + modelview.m15 * projection.m13;
	planes.m14 = modelview.m12 * projection.m2 + modelview.m13 * projection.m6 + modelview.m14 * projection.m10 + modelview.m15 * projection.m14;
	planes.m15 = modelview.m12 * projection.m3 + modelview.m13 * projection.m7 + modelview.m14 * projection.m11 + modelview.m15 * projection.m15;

	Planes[static_cast<size_t>(FrustumPlanes::Right)] = { planes.m3 - planes.m0, planes.m7 - planes.m4, planes.m11 - planes.m8, planes.m15 - planes.m12 };
	normalizePlane(Planes[static_cast<size_t>(FrustumPlanes::Right)]);

	Planes[static_cast<size_t>(FrustumPlanes::Left)] = { planes.m3 + planes.m0, planes.m7 + planes.m4, planes.m11 + planes.m8, planes.m15 + planes.m12 };
	normalizePlane(Planes[static_cast<size_t>(FrustumPlanes::Left)]);

	Planes[static_cast<size_t>(FrustumPlanes::Top)] = { planes.m3 - planes.m1, planes.m7 - planes.m5, planes.m11 - planes.m9, planes.m15 - planes.m13 };
	normalizePlane(Planes[static_cast<size_t>(FrustumPlanes::Top)]);

	Planes[static_cast<size_t>(FrustumPlanes::Bottom)] = { planes.m3 + planes.m1, planes.m7 + planes.m5, planes.m11 + planes.m9, planes.m15 + planes.m13 };
	normalizePlane(Planes[static_cast<size_t>(FrustumPlanes::Bottom)]);

	Planes[static_cast<size_t>(FrustumPlanes::Back)] = { planes.m3 - planes.m2, planes.m7 - planes.m6, planes.m11 - planes.m10, planes.m15 - planes.m14 };
	normalizePlane(Planes[static_cast<size_t>(FrustumPlanes::Back)]);

	Planes[static_cast<size_t>(FrustumPlanes::Front)] = { planes.m3 + planes.m2, planes.m7 + planes.m6, planes.m11 + planes.m10, planes.m15 + planes.m14 };
	normalizePlane(Planes[static_cast<size_t>(FrustumPlanes::Front)]);
}
//-----------------------------------------------------------------------------
float DistanceToPlane(const Vector4& plane, const Vector3& position) noexcept
{
	return (plane.x * position.x + plane.y * position.y + plane.z * position.z + plane.w);
}
//-----------------------------------------------------------------------------
float DistanceToPlane(const Vector4& plane, float x, float y, float z) noexcept
{
	return (plane.x * x + plane.y * y + plane.z * z + plane.w);
}
//-----------------------------------------------------------------------------
bool Frustum::PointIn(const Vector3& position) const noexcept
{
	for (size_t i = 0; i < MaxFrustumPlanes; i++)
	{
		if (DistanceToPlane(Planes[i], position) <= 0) // point is behind plane
			return false;
	}

	return true;
}
//-----------------------------------------------------------------------------
bool Frustum::PointIn(float x, float y, float z) const noexcept
{
	for (size_t i = 0; i < MaxFrustumPlanes; i++)
	{
		if (DistanceToPlane(Planes[i], x, y, z) <= 0) // point is behind plane
			return false;
	}

	return true;
}
//-----------------------------------------------------------------------------
bool Frustum::SphereIn(const Vector3& position, float radius) const noexcept
{
	for (size_t i = 0; i < MaxFrustumPlanes; i++)
	{
		if (DistanceToPlane(Planes[i], position) < -radius) // center is behind plane by more than the radius
			return false;
	}

	return true;
}
//-----------------------------------------------------------------------------
bool Frustum::AABBoxIn(const Vector3& min, const Vector3& max) const noexcept
{
	// if any point is in and we are good
	if (PointIn(min.x, min.y, min.z))
		return true;

	if (PointIn(min.x, max.y, min.z))
		return true;

	if (PointIn(max.x, max.y, min.z))
		return true;

	if (PointIn(max.x, min.y, min.z))
		return true;

	if (PointIn(min.x, min.y, max.z))
		return true;

	if (PointIn(min.x, max.y, max.z))
		return true;

	if (PointIn(max.x, max.y, max.z))
		return true;

	if (PointIn(max.x, min.y, max.z))
		return true;

	// check to see if all points are outside of any one plane, if so the entire box is outside
	for (size_t i = 0; i < MaxFrustumPlanes; i++)
	{
		bool oneInside = false;

		if (DistanceToPlane(Planes[i], min.x, min.y, min.z) >= 0)
			oneInside = true;

		if (DistanceToPlane(Planes[i], max.x, min.y, min.z) >= 0)
			oneInside = true;

		if (DistanceToPlane(Planes[i], max.x, max.y, min.z) >= 0)
			oneInside = true;

		if (DistanceToPlane(Planes[i], min.x, max.y, min.z) >= 0)
			oneInside = true;

		if (DistanceToPlane(Planes[i], min.x, min.y, max.z) >= 0)
			oneInside = true;

		if (DistanceToPlane(Planes[i], max.x, min.y, max.z) >= 0)
			oneInside = true;

		if (DistanceToPlane(Planes[i], max.x, max.y, max.z) >= 0)
			oneInside = true;

		if (DistanceToPlane(Planes[i], min.x, max.y, max.z) >= 0)
			oneInside = true;

		if (!oneInside)
			return false;
	}

	// the box extends outside the frustum but crosses it
	return true;
}
//-----------------------------------------------------------------------------