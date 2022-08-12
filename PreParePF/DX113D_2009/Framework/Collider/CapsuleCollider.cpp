#include "Framework.h"

CapsuleCollider::CapsuleCollider(float radius, float height, UINT stackCount, UINT sliceCount)
    : radius(radius), height(height), stackCount(stackCount), sliceCount(sliceCount)
{
    type = CAPSULE;
    CreateMesh();
}

CapsuleCollider::~CapsuleCollider()
{
}

bool CapsuleCollider::RayCollision(IN Ray ray, OUT Contact* contact)
{
	UpdateWorld();

	Vector3 direction = Up();

	Vector3 pa = globalPosition - direction * Height() * 0.5f;
	Vector3 pb = globalPosition + direction * Height() * 0.5f;

	Vector3 ro = ray.pos;
	Vector3 rd = ray.dir;

	float r = Radius();

	Vector3 ba = pb - pa;
	Vector3 oa = ro - pa;

	float baba = Vector3::Dot(ba, ba);
	float bard = Vector3::Dot(ba, rd);
	float baoa = Vector3::Dot(ba, oa);
	float rdoa = Vector3::Dot(rd, oa);
	float oaoa = Vector3::Dot(oa, oa);

	float a = baba - bard * bard;
	float b = baba * rdoa - baoa * bard;
	float c = baba * oaoa - baoa * baoa - r * r * baba;
	float h = b * b - a * c;

	if (h >= 0.0f)
	{
		float t = (-b - sqrt(h)) / a;

		float y = baoa + t * bard;

		if (y > 0.0f && y < baba)
		{
			if (contact != nullptr)
			{
				contact->distance = t;
				contact->hitPoint = ray.pos + ray.dir * t;
			}
			return true;
		}

		Vector3 oc = (y <= 0.0f) ? oa : ro - pb;
		b = Vector3::Dot(rd, oc);
		c = Vector3::Dot(oc, oc) - r * r;
		h = b * b - c;
		if (h > 0.0f)
		{
			if (contact != nullptr)
			{
				contact->distance = t;
				contact->hitPoint = ray.pos + ray.dir * t;
			}
			return true;
		}
	}

    return false;
}

bool CapsuleCollider::BoxCollision(BoxCollider* collider)
{
	Vector3 direction = Up();
	Vector3 A = globalPosition - direction * Height() * 0.5f;
	Vector3 B = globalPosition + direction * Height() * 0.5f;

	Vector3 P = collider->GlobalPosition();

	Vector3 pointOnLine = ClosestPointOnLineSegment(A, B, P);

	return collider->SphereCollision(pointOnLine, Radius());
}

bool CapsuleCollider::SphereCollision(SphereCollider* collider)
{
	Vector3 direction = Up();
	Vector3 A = globalPosition - direction * Height() * 0.5f;
	Vector3 B = globalPosition + direction * Height() * 0.5f;

	Vector3 P = collider->GlobalPosition();

	Vector3 pointOnLine = ClosestPointOnLineSegment(A, B, P);

	float distance = Distance(P, pointOnLine);

	return distance <= (Radius() + collider->Radius());
}

bool CapsuleCollider::CapsuleCollision(CapsuleCollider* collider)
{
	Vector3 aDirection = Up();

	Vector3 aA = globalPosition - aDirection * Height() * 0.5f;
	Vector3 aB = globalPosition + aDirection * Height() * 0.5f;

	Vector3 bDirection = collider->Up();

	Vector3 bA = collider->globalPosition - bDirection * Height() * 0.5f;
	Vector3 bB = collider->globalPosition + bDirection * Height() * 0.5f;

	Vector3 v0 = bA - aA;
	Vector3 v1 = bB - aA;
	Vector3 v2 = bA - aB;
	Vector3 v3 = bB - aB;

	float d0 = Vector3::Dot(v0, v0);
	float d1 = Vector3::Dot(v1, v1);
	float d2 = Vector3::Dot(v2, v2);
	float d3 = Vector3::Dot(v3, v3);

	Vector3 bestA;
	if (d2 < d0 || d2 < d1 || d3 < d0 || d3 > d1)
		bestA = aB;
	else
		bestA = aA;

	Vector3 bestB = ClosestPointOnLineSegment(bA, bB, bestA);
	bestA = ClosestPointOnLineSegment(aA, aB, bestB);

	float distance = Distance(bestA, bestB);	

	return distance <= (Radius() + collider->Radius());
}

void CapsuleCollider::CreateMesh()
{
	float phiStep = XM_PI / stackCount;
	float thetaStep = XM_2PI / sliceCount;

	//Vertices
	for (UINT i = 0; i <= stackCount; i++)
	{
		float phi = i * phiStep;

		for (UINT j = 0; j <= sliceCount; j++)
		{
			float theta = j * thetaStep;

			Vertex vertex;

			vertex.position.x = sin(phi) * cos(theta) * radius;
			vertex.position.y = cos(phi) * radius;
			vertex.position.z = sin(phi) * sin(theta) * radius;

			if (vertex.position.y > 0)
				vertex.position.y += height * 0.5f;
			else
				vertex.position.y -= height * 0.5f;

			vertices.emplace_back(vertex);
		}
	}

	for (UINT i = 0; i < stackCount; i++)
	{
		for (UINT j = 0; j < sliceCount; j++)
		{
			indices.emplace_back((sliceCount + 1) * i + j);//0
			indices.emplace_back((sliceCount + 1) * i + j + 1);//1

			indices.emplace_back((sliceCount + 1) * i + j);//0
			indices.emplace_back((sliceCount + 1) * (i + 1) + j);//2			
		}
	}

	mesh = new Mesh(vertices.data(), sizeof(Vertex), vertices.size(),
		indices.data(), indices.size());
}
