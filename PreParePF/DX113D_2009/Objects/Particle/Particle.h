#pragma once

class Particle : public Transform
{
protected:
	const UINT MAX_COUNT = 200;

	Material* material;
	GeometryShader* geometryShader;

	VertexBuffer* vertexBuffer;
	VectorBuffer* viewUpBuffer;

	UINT particleCount;

	BlendState* blendState[2];
	DepthStencilState* depthState[2];
public:
	Particle(wstring diffuseFile);
	virtual ~Particle();

	virtual void Create() = 0;
	virtual void Update() = 0;

	virtual void Render();

	virtual void PostRender() = 0;

	virtual void Play(Vector3 position);
	virtual void Stop();
};