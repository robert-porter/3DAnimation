#pragma once

#include "MD2.h"
#include "Matrix4x4.h"
#include "Vector3.h"


class Entity
{
public:
	Entity(void);
	~Entity(void);

	void SetModel(MorphAnimation *pModel) { pAnimatedModel->SetModel(pModel); }
	void SetWeaponModel(MorphAnimation *pModel) { pWeaponModel->SetModel(pModel); }

	void Draw();

	void Update(float dTime);

	void SetPosition(const Vector3f &p) { position = p; } 
	const Vector3f &GetPosition() const { return position; }
	void SetYaw(float y) { yaw = y; }
	float GetYaw() const { return yaw; }

	void WalkForward(float dTime);
	void WalkBackward(float dTime);
	void StrafeLeft(float dTime);
	void StrafeRight(float dTime);
	void TurnRight(float dTime);
	void TurnLeft(float dTime);
	void Attack();
	void Jump()
	{
		if(!isJumping)
			velocity = Vector3f(0, 2, 0);
	}

	void Hit()
	{
		pAnimatedModel->SetAnimation(PAIN_A);
	}
	Matrix4x4f GetRotation() { return Matrix4x4f::RotationY(yaw); }

protected:

	Vector3f position;
	float yaw;

	bool isJumping;
	bool isAttacking;
	bool isWalking;
	bool isStrafing;
	float walkDirection;	// with the range {-1,0,1}
	float strafeDirection;	// with the range {-1,0,1}

	float walkSpeed;
	float strafeSpeed;
	float yawSpeed;

	Vector3f velocity;
public:
	Md2AnimatedModel *pAnimatedModel;
	Md2AnimatedModel *pWeaponModel;
};
