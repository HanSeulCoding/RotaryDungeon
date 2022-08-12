#pragma once

class Transform
{
public:
	string tag;

	Vector3 localPosition;
	Vector3 localRotation;
	Vector3 localScale;

	bool isActive;

protected:
	Vector3 globalPosition;
	Vector3 globalRotation;
	Vector3 globalScale;

	Vector3 pivot;

	Matrix world;
	Matrix* parent;

	MatrixBuffer* worldBuffer;

public:
	Transform(string tag = "Untagged");
	~Transform();

	void UpdateWorld();

	void PostRender();

	void SetWorldBuffer(UINT slot = 0);

	Vector3 Forward();
	Vector3 Up();
	Vector3 Right();

	Vector3 GlobalPosition() { return globalPosition; }
	Vector3 GlobalRotation() { return globalRotation; }
	Vector3 GlobalScale() { return globalScale; }

	void SetParent(Matrix* matrix) { parent = matrix; }
	Matrix& GetWorld() { return world; }
};