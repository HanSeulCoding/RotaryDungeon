#include "Framework.h"

Cube::Cube(Vector3 size) : size(size)
{
	material = new Material(L"Diffuse");

	Create();
}

Cube::~Cube()
{
	delete material;
	delete mesh;
}

void Cube::Render()
{
	mesh->IASet();

	SetWorldBuffer();

	material->Set();

	DC->DrawIndexed((UINT)indices.size(), 0, 0);
}

void Cube::Create()
{
	Vector3 halfSize = size * 0.5f;

	vertices.resize(8);
	vertices[0].position = { -halfSize.x, -halfSize.y, -halfSize.z };
	vertices[1].position = { -halfSize.x, +halfSize.y, -halfSize.z };
	vertices[2].position = { +halfSize.x, +halfSize.y, -halfSize.z };
	vertices[3].position = { +halfSize.x, -halfSize.y, -halfSize.z };

	vertices[4].position = { -halfSize.x, -halfSize.y, +halfSize.z };
	vertices[5].position = { -halfSize.x, +halfSize.y, +halfSize.z };
	vertices[6].position = { +halfSize.x, +halfSize.y, +halfSize.z };
	vertices[7].position = { +halfSize.x, -halfSize.y, +halfSize.z };

	vertices[0].uv = { 0, 1 };
	vertices[1].uv = { 0, 0 };
	vertices[2].uv = { 1, 0 };
	vertices[3].uv = { 1, 1 };
	vertices[4].uv = { 0, 0 };
	vertices[5].uv = { 0, 1 };
	vertices[6].uv = { 1, 1 };
	vertices[7].uv = { 1, 0 };

	indices.resize(36);
	indices = {
		//Front
		0, 1, 2, 0, 2, 3,
		//Right
		3, 2, 6, 3, 6, 7,
		//Up
		1, 5, 6, 1, 6, 2,
		//Back
		4, 6, 5, 4, 7, 6,
		//Left
		0, 5, 1, 0, 4, 5,
		//Down
		0, 7, 4, 0, 3, 7
	};

	CreateNormal();

	mesh = new Mesh(vertices.data(), sizeof(VertexType), (UINT)vertices.size(),
		indices.data(), (UINT)indices.size());
}

void Cube::CreateNormal()
{
	for (UINT i = 0; i < indices.size() / 3; i++)
	{
		UINT index0 = indices[i * 3 + 0];
		UINT index1 = indices[i * 3 + 1];
		UINT index2 = indices[i * 3 + 2];

		Vector3 v0 = vertices[index0].position;
		Vector3 v1 = vertices[index1].position;
		Vector3 v2 = vertices[index2].position;

		Vector3 A = v1 - v0;
		Vector3 B = v2 - v0;

		Vector3 normal = Vector3::Cross(A, B).Normalized();

		vertices[index0].normal = normal + vertices[index0].normal;
		vertices[index1].normal = normal + vertices[index1].normal;
		vertices[index2].normal = normal + vertices[index2].normal;
	}
}
