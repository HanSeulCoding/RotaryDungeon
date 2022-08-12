#include "Framework.h"

Quad::Quad(wstring textureFile)
{
	material = new Material(L"Texture");
	material->SetTexture(textureFile);

	CreateMesh();	
}

Quad::Quad(Float2 size)
	: size(size)
{
	material = new Material(L"Texture");	

	CreateMesh();
}


Quad::Quad(wstring textureFile, Float2 size)
	: size(size)
{
	material = new Material(L"Texture");
	material->SetTexture(textureFile);	

	CreateMesh();
}

Quad::~Quad()
{
	delete material;
	delete mesh;
}

void Quad::Render()
{
	SetWorldBuffer();

	mesh->IASet();
	material->Set();

	DC->DrawIndexed(indices.size(), 0, 0);
}

void Quad::CreateMesh()
{
	/*Float2 halfSize;
	halfSize.x = size.x * 0.5f;
	halfSize.y = size.y * 0.5f;
	vertices.emplace_back(-halfSize.x, +halfSize.y, 0, 0);
	vertices.emplace_back(+halfSize.x, +halfSize.y, 1, 0);
	vertices.emplace_back(-halfSize.x, -halfSize.y, 0, 1);
	vertices.emplace_back(+halfSize.x, -halfSize.y, 1, 1);

	indices = { 0, 1, 2, 2, 1, 3 };

	mesh = new Mesh(vertices.data(), sizeof(VertexUV), vertices.size(),
		indices.data(), indices.size());*/
	VertexUV vertices[4];
	Float2 halfSize;
	halfSize.x = size.x * 0.5f;
	halfSize.y = size.y * 0.5f;
	vertices[0].position = { -halfSize.x, -halfSize.y, 0.0f };
	vertices[1].position = { -halfSize.x, +halfSize.y, 0.0f };
	vertices[2].position = { +halfSize.x, -halfSize.y, 0.0f };
	vertices[3].position = { +halfSize.x, +halfSize.y, 0.0f };

	vertices[0].uv = { 0, 1 };
	vertices[1].uv = { 0, 0 };
	vertices[2].uv = { 1, 1 };
	vertices[3].uv = { 1, 0 };

	UINT indices[] = {
		0, 1, 2,
		2, 1, 3
	};

	mesh = new Mesh(vertices, sizeof(VertexUV), 4,
		indices, 6);
}
