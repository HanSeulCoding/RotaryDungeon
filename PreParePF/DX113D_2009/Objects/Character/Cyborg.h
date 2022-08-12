#pragma once

class Cyborg : public ModelAnimator, public Transform
{
private:
	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	Outline* outline;
	
public:
	Cyborg();
	~Cyborg();

	void Update();

	void PreRender();
	void Render();
	void PostRender();
};