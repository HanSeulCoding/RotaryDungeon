#pragma once

class CharacterMovement
{
private:
	float maxMoveSpeed;
	float moveSpeed;
	float rotSpeed;

	Vector3 velocity;
	float accelation;
	float deceleration;

public:
	CharacterMovement();
	~CharacterMovement();

	void Move();

	void SetMaxMoveSpeed(float value) { maxMoveSpeed = value; }
	void SetMoveSpeed(float value) { moveSpeed = value; }
	void SetRotSpeed(float value) { rotSpeed = value; }

	void SetAccelation(float value) { accelation = value; }
	void SetDeceleration(float value) { deceleration = value; }

	void SetVelocity(Vector3 value) { velocity = value; }

	float Speed() { return velocity.Length(); }
	Vector3 Velocity() { return velocity; }
};