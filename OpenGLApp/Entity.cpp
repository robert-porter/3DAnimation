#include "Entity.h"
#include "Matrix4x4.h"
#include "mymath.h"

Entity::Entity(void)
{
	pAnimatedModel = new Md2AnimatedModel();
	pWeaponModel = new Md2AnimatedModel();
	position = Vector3f(1,1,1);
	yaw = 0;
	walkSpeed = 4.0f;
	strafeSpeed = 3.0f;
	yawSpeed = 3.2f;
	isWalking = false;
	isStrafing = false;
	isAttacking = false;
	walkDirection = 0;
	strafeDirection = 0;
}

Entity::~Entity(void)
{
}

void Entity::WalkForward(float dTime)
{
	isWalking = true;
	walkDirection = 1.0f;
}

void Entity::WalkBackward(float dTime)
{
	isWalking = true;
	walkDirection = -1.0f;
}

void Entity::StrafeLeft(float dTime)
{
	isStrafing = true;
	strafeDirection = -1.0f;
}

void Entity::StrafeRight(float dTime)
{
	isStrafing = true;
	strafeDirection = 1.0f;
}

void Entity::TurnRight(float dTime)
{
	yaw -= yawSpeed * dTime;
}

void Entity::TurnLeft(float dTime)
{
	yaw += yawSpeed * dTime;
}

void Entity::Draw()
{
	glEnable(GL_TEXTURE_2D);
	glPushMatrix();
	glTranslatef(position.x, position.y, position.z);
	glRotatef(yaw * RAD_TO_DEG, 0, 1, 0);


	pAnimatedModel->Draw();
	//pWeaponModel->Draw();

	glPopMatrix();
}

void Entity::Update(float dTime)
{	
	if(isAttacking)
	{
		pAnimatedModel->SetAnimation(ATTACK);
		pWeaponModel->SetAnimation(ATTACK);
	}
	if(isWalking)
	{
		float distance = walkSpeed * dTime * walkDirection;

		Vector3f forward(0, 0, -1);
		forward = Matrix4x4f::RotationY(yaw).TransformVector(forward);
		position += forward * distance;

		if(!isAttacking)
		{
			pAnimatedModel->SetAnimation(RUN);
			pWeaponModel->SetAnimation(RUN);
		}
		pAnimatedModel->SetWalking(true);
	}

	else if(isStrafing)
	{
		float distance = strafeSpeed * dTime * strafeDirection;

		Vector3f right(1, 0, 0);
		right = Matrix4x4f::RotationY(yaw).TransformVector(right);
		position += right * distance;

		pAnimatedModel->SetWalking(true);
	}
	else
		pAnimatedModel->SetWalking(false);

	isWalking = isStrafing = isAttacking = false;

	pAnimatedModel->Update(dTime);
	pWeaponModel->Update(dTime);
}

void Entity::Attack()
{
	isAttacking = true;
}