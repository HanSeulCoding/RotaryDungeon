#include "Framework.h"

Cyborg::Cyborg()
	: ModelAnimator("Player")
{
	localScale *= 0.05f;
	localPosition = { 50, 0, 50 };

	SetShader(L"Lighting");
	SetDiffuseMap(L"Textures/Player/cyborgFemaleA.png");

	ReadClip(0,"Player/Idle", 1);
	ReadClip(1,"Player/Run", 1);
	ReadClip(2,"Player/Jump", 1);

	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();

	outline = new Outline();
	outline->SetSRV(renderTarget->GetSRV());
	outline->localPosition = { WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, 0 };
	outline->localScale = { WIN_WIDTH, WIN_HEIGHT, 1 };
}

Cyborg::~Cyborg()
{
	delete renderTarget;
	delete depthStencil;

	delete outline;
}

void Cyborg::Update()
{
	ModelAnimator::Update();
}

void Cyborg::PreRender()
{
	renderTarget->Set(depthStencil);

	Render();
}

void Cyborg::Render()
{
	SetWorldBuffer();
	ModelAnimator::Render();
}

void Cyborg::PostRender()
{
	outline->Render();
}
