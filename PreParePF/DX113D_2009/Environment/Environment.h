#pragma once

class Environment : public Singleton<Environment>
{
private:
	friend class Singleton;
		
	Matrix projection;	
	MatrixBuffer* projectionBuffer;

	LightBuffer* lightBuffer;

	Camera* mainCamera;

	D3D11_VIEWPORT viewport;

	SamplerState* samplerState;

	Environment();
	~Environment();
public:
	void PostRender();

	void Set();

	void SetViewProjection();
	void SetViewport(float width = WIN_WIDTH, float height = WIN_HEIGHT);

	Camera* GetMainCamera() { return mainCamera; }
	Matrix GetProjection() { return projection; }
private:
	void CreatePerspective();
};