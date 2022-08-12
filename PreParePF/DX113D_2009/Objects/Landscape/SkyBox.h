#pragma once

class SkyBox
{
private:
	Sphere* sphere;
	Texture* cubeMap;

	RasterizerState* rasterizerState[2];
	DepthStencilState* depthStencilState[2];
public:
	SkyBox(wstring path = L"Textures/Environment/Texture1.dds");
	~SkyBox();

	void Render();
};