#include "Framework.h"

CharacterMovement::CharacterMovement()
	: maxMoveSpeed(30.0f), moveSpeed(10.0f), rotSpeed(2.0f),
	accelation(30.0f), deceleration(10.0f)
{
}

CharacterMovement::~CharacterMovement()
{
}

void CharacterMovement::Move()
{
	if (velocity.Length() > 0.0f)
	{
		Vector3 zero;
		velocity = LERP(velocity, zero, deceleration * DELTA);
	}
}
