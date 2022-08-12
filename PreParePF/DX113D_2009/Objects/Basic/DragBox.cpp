#include "Framework.h"

DragBox::DragBox()
{
	material = new Material(L"Collider");
	material->GetBuffer()->data.diffuse = { 0, 1, 0, 1 };

	CreateMesh();
	CreateViewProjection();
}

DragBox::~DragBox()
{
	delete material;
	delete mesh;

	delete viewBuffer;
	delete projectionBuffer;
}

void DragBox::Render()
{
	mesh->IASet(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	SetWorldBuffer();

	viewBuffer->SetVSBuffer(1);
	projectionBuffer->SetVSBuffer(2);	

	material->Set();

	DC->DrawIndexed(8, 0, 0);	
}

void DragBox::CreateMesh()
{
	Vertex vertices[4];
	vertices[0].position = { 0.0f, 0.0f, 0.0f };
	vertices[1].position = { 0.0f, 1.0f, 0.0f };
	vertices[2].position = { 1.0f, 1.0f, 0.0f };
	vertices[3].position = { 1.0f, 0.0f, 0.0f };

	UINT indices[] = {
		0, 1,
		1, 2,
		2, 3,
		3, 0
	};

	mesh = new Mesh(vertices, sizeof(Vertex), 4,
		indices, 8);
}

void DragBox::CreateViewProjection()
{
	view = XMMatrixIdentity();
	orthographic = XMMatrixOrthographicOffCenterLH(0, WIN_WIDTH, WIN_HEIGHT, 0, -1, 1);

	viewBuffer = new MatrixBuffer();
	viewBuffer->Set(view);

	projectionBuffer = new MatrixBuffer();
	projectionBuffer->Set(orthographic);
}
