#pragma once

class Camera : public Transform
{
private:
	float moveSpeed;
	float rotSpeed;

	MatrixBuffer* viewBuffer;
	VectorBuffer* viewPosBuffer;
	Matrix view;

	Vector3 oldPos;

	float distance;
	float height;

	Vector3 offset;
	Vector3 originPosition;
	Vector3 destPos;
	float destRot;

	float moveDamping;
	float rotDamping;

	float magnitude;
	float duration;
	float reduceDamping;

	float rotY;

	Matrix rotMatrix;

	Transform* target;

public:
	Camera();
	~Camera();

	void Update();
	void PostRender();

	void SetView();
	void SetViewPos();

	void SetBuffer();

	Ray ScreenPointToRay(Vector3 screenPos);

	Matrix GetView() { return view; }	

	void SetTarget(Transform* value) { target = value; }
	void SetRotDamping(float value) { rotDamping = value; }

	void ShakeStart(float magnitude, float duration, float reduceDamping);
private:
	void FreeMode();
	void FollowMode();

	void FreeMove();
	void FreeRotation();

	void Save();
	void Load();
	void Shake();
};