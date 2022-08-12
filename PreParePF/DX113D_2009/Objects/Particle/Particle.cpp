#include "Framework.h"

Particle::Particle(wstring diffuseFile)
	: particleCount(MAX_COUNT)
{
	isActive = false;

	material = new Material();
	material->SetDiffuseMap(diffuseFile);

	blendState[0] = new BlendState();
	blendState[1] = new BlendState();
	blendState[1]->Additive();

	depthState[0] = new DepthStencilState();
	depthState[1] = new DepthStencilState();
	depthState[1]->DepthWriteMask(D3D11_DEPTH_WRITE_MASK_ZERO);

	viewUpBuffer = new VectorBuffer();
}

Particle::~Particle()
{
	delete material;
	delete vertexBuffer;

	delete viewUpBuffer;

	delete blendState[0];
	delete blendState[1];

	delete depthState[0];
	delete depthState[1];	
}

void Particle::Render()
{
	if (!isActive) return;

	Vector3 up = CAMERA->Up();

	viewUpBuffer->data.vector.x = up.x;
	viewUpBuffer->data.vector.y = up.y;
	viewUpBuffer->data.vector.z = up.z;
	
	viewUpBuffer->SetGSBuffer(10);

	blendState[1]->SetState();
	depthState[1]->SetState();

	SetWorldBuffer();
	vertexBuffer->IASet();
	DC->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_POINTLIST);

	material->Set();
	geometryShader->Set();

	DC->Draw(particleCount, 0);

	DC->GSSetShader(nullptr, nullptr, 0);

	blendState[0]->SetState();
	depthState[0]->SetState();
}

void Particle::Play(Vector3 position)
{
	isActive = true;
	localPosition = position;
	localRotation = CAMERA->localRotation;
}

void Particle::Stop()
{
	isActive = false;
}
