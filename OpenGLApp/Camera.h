#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"
#include "mymath.h"
#include <cassert>


class Camera
{
public:
	
	Camera();
	
	void SetPerspective();

	const Vector3f& GetPosition() const { return position; }
	const Vector3f& GetRight() const { return right; }
	const Vector3f& GetUp() const { return up; }
	const Vector3f& GetForward() const { return forward; }

	void SetPosition(const Vector3f &p) { position = p; }
	void SetAxes(const Vector3f& Right, const Vector3f& Up, const Vector3f &Forward);

	void SetFrustum(float fovy, float aspect, float znear, float zfar);
	void SetFrustum(float left, float right, float bottom, float top, float znear, float zfar);

	void LookUp(float angle);
	void LookDown(float angle);

	void SetYaw(float y);

	float GetYaw() const { return yaw; }
	float GetPitch() const { return pitch; }

protected:
	float fLeft, fRight, fBottom, fTop, fNear, fFar; // frustrum planes

	Vector3f position;
	Vector3f right, up, forward;

	float yaw, pitch;
};

