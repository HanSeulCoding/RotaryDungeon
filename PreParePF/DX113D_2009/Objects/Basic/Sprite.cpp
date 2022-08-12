#include "Framework.h"

Sprite::Sprite(wstring shaderFile)
{
	material = new Material(shaderFile);

	CreateMesh();
	CreateViewProjection();

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Alpha(true);
//	blendState[1]->AlphaToCoverage(true);

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthEnable(false);

	blackWhiteBuffer = new BlackWhiteBuffer();
}

Sprite::~Sprite()
{
	delete material;
	delete mesh;

	delete viewBuffer;
	delete projectionBuffer;

	delete blendState[0];
	delete blendState[1];

	delete depthState[0];
	delete depthState[1];
}

void Sprite::Update()
{
}

void Sprite::Render()
{

	blackWhiteBuffer->SetPSBuffer(13);
	mesh->IASet();

	SetWorldBuffer();

	viewBuffer->SetVSBuffer(1);
	projectionBuffer->SetVSBuffer(2);
	
	DC->PSSetShaderResources(0, 1, &srv);

	material->Set();

	blendState[1]->SetState();
	depthState[1]->SetState();

	DC->DrawIndexed(6, 0, 0);

	blendState[0]->SetState();
	depthState[0]->SetState();
}

void Sprite::CreateMesh()
{
	VertexUV vertices[4];
	vertices[0].position = { -0.5f, -0.5f, 0.0f };
	vertices[1].position = { -0.5f, +0.5f, 0.0f };
	vertices[2].position = { +0.5f, -0.5f, 0.0f };
	vertices[3].position = { +0.5f, +0.5f, 0.0f };

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

void Sprite::CreateViewProjection()
{
	view = XMMatrixIdentity();
	orthographic = XMMatrixOrthographicOffCenterLH(0, WIN_WIDTH, 0, WIN_HEIGHT, -1, 1);

	viewBuffer = new MatrixBuffer();
	viewBuffer->Set(view);

	projectionBuffer = new MatrixBuffer();
	projectionBuffer->Set(orthographic);
}
