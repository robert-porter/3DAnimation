#include <windows.h>
#include <gl\gl.h>
#include "mymath.h"
#include "Camera.h"


Camera::Camera()
{
	right = Vector3f(1, 0, 0);
	up = Vector3f(0, 1, 0);
	forward = Vector3f(0, 0, -1);
	position = Vector3f(0, 20, 3);

	float aspectRatio = 4.0/3.0f;
	fLeft = -1.0 * aspectRatio;;
	fRight = 1.0 * aspectRatio;;
	fBottom = -1.0;
	fTop = 1.0;
	fNear = 0.2f;
	fFar = 100.0f;

	yaw = 0.0f;
	pitch = 0.0f;

	SetFrustum(PI/4.0f, aspectRatio, fNear, fFar);
}

void Camera::SetPerspective()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();

	glFrustum(fLeft, fRight, fBottom, fTop, fNear, fFar);
	//glOrtho(fLeft, fRight, fBottom, fTop, fNear, fFar);
}

void Camera::SetFrustum(float fovy, float aspectRatio, float znear, float zfar)
{
	fNear = znear;
	fFar = zfar;
	fTop = tan(fovy * 0.5f) * fNear;
	fRight = fTop * aspectRatio;
	fBottom = -fTop;
	fLeft = -fRight;
}

void Camera::SetFrustum(float left, float right, float bottom, float top, float znear, float zfar)
{		
	fNear = znear;
	fFar = zfar;
	fLeft = left; 
	fRight = right;
	fBottom = bottom; 
	fTop = top;
}

void Camera::SetAxes(const Vector3f& Right, const Vector3f& Up, const Vector3f &Forward)
{
	right = Right; up = Up; forward = Forward;
}

void Camera::LookUp(float angle)
{
	if(pitch - angle > PI/2.0f - 0.1)
	{
		pitch = PI/2.0f - 0.1;
	}
	else if(pitch - angle < -PI/2.0f)
	{
		pitch = -PI/2.0f + 0.1;
	}
	else
		pitch -= angle;

	Matrix4x4f Rx = Matrix4x4f::RotationX(pitch);
	Matrix4x4f Ry = Matrix4x4f::RotationY(yaw);
	Matrix4x4f R = Ry * Rx;
	forward = R.TransformVector(Vector3f(0, 0, -1));
	right = R.TransformVector(Vector3f(1, 0, 0));
	up = R.TransformVector(Vector3f(0, 1, 0));
}

void Camera::LookDown(float angle)
{
	if(pitch - angle > PI/2.0f - 0.1)
	{
		pitch = PI/2.0f - 0.1;
	}
	else if(pitch - angle < -PI/2.0f)
	{
		pitch = -PI/2.0f + 0.1;
	}
	else
		pitch -= angle;

	pitch -= angle;
	Matrix4x4f Rx = Matrix4x4f::RotationX(pitch);
	Matrix4x4f Ry = Matrix4x4f::RotationY(yaw);
	Matrix4x4f R = Ry * Rx;
	forward = R.TransformVector(Vector3f(0, 0, -1));
	right = R.TransformVector(Vector3f(1, 0, 0));
	up = R.TransformVector(Vector3f(0, 1, 0));
}


void Camera::SetYaw(float y)
{ 
	yaw = y;
	Matrix4x4f Rx = Matrix4x4f::RotationX(pitch);
	Matrix4x4f Ry = Matrix4x4f::RotationY(yaw);
	Matrix4x4f R = Ry * Rx;
	forward = R.TransformVector(Vector3f(0, 0, -1));
	right = R.TransformVector(Vector3f(1, 0, 0));
	up = R.TransformVector(Vector3f(0, 1, 0));
}