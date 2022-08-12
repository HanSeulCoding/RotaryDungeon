#include "Framework.h"

MeatImage::MeatImage()
	:Texture2D(L"Textures/Bag/MeatImage.png")
{
	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();

	outline = new Outline();
	outline->SetSRV(renderTarget->GetSRV());
	outline->localPosition = { WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, 0 };
	outline->localScale = { WIN_WIDTH, WIN_HEIGHT, 1 };

	
}

MeatImage::~MeatImage()
{
	delete renderTarget;
	delete depthStencil;

	delete outline;
}

void MeatImage::Update()
{
	CreateRec();
	IsOnMousePos();
}

void MeatImage::PreRender()
{
	renderTarget->Set(depthStencil);
	Render();
}

void MeatImage::Render()
{
	Sprite::Render();
}

void MeatImage::PostRender()
{
	if (isMouseOn)
		outline->Render();
	
}
