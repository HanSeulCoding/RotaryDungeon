#include "Framework.h"

BagImage::BagImage()
	:Texture2D(L"Textures/Bag/bagImage.png")
	,isInventory(false)
{
	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();

	localPosition = Vector3(WIN_WIDTH - 100, 50, 0);
	localScale *= 100;
	outline = new Outline();
	outline->SetSRV(renderTarget->GetSRV());
	outline->localPosition = { WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, 0 };
	outline->localScale = { WIN_WIDTH, WIN_HEIGHT, 1 };

	CreateRec();
}

BagImage::~BagImage()
{
	delete renderTarget;
	delete depthStencil;

	delete outline;
}


void BagImage::Update()
{
	IsOnMousePos();

	if (isMouseOn)
	{
		if (KEY_DOWN(VK_LBUTTON))
		{
			GMANAGER->SetExcuteInventory(true); //인벤토리창 띄우기
		}
	}
}

void BagImage::PreRender()
{
	renderTarget->Set(depthStencil);
	Render();
}

void BagImage::Render()
{
	Sprite::Render();
}

void BagImage::PostRender()
{
	if (isMouseOn)
		outline->Render();
	
}
