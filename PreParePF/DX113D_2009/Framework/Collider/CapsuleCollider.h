#pragma once

class CapsuleCollider : public Collider
{
private:
	float radius;
	float height;

	float stackCount;
	float sliceCount;
public:
	CapsuleCollider(float radius = 1.0f, float height = 2.0f,
		UINT stackCount = 15, UINT sliceCount = 30);
	~CapsuleCollider();

	virtual bool RayCollision(IN Ray ray, OUT Contact* contact = nullptr) override;
	virtual bool BoxCollision(BoxCollider* collider) override;
	virtual bool SphereCollision(SphereCollider* collider) override;
	virtual bool CapsuleCollision(CapsuleCollider* collider) override;

	float Radius() { return radius * max(globalScale.x, max(globalScale.y, globalScale.z)); }
	float Height() { return height * globalScale.y; }

private:
	virtual void CreateMesh() override;
};