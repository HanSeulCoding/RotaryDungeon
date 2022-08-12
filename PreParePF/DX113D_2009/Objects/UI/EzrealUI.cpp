#include "Framework.h"

EzrealUI::EzrealUI()
	:Texture2D(L"Textures/UI/Ezreal.png")
{
	localPosition = Vector3(50, 645, 0);//Vector3 ( 50, 645, 0 );
	localScale = Vector3(64, 64, 0);

	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();

	outline = new Outline();
	outline->SetSRV(renderTarget->GetSRV());
	outline->localPosition = { WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, 0 };
	outline->localScale = { WIN_WIDTH, WIN_HEIGHT, 1 };
}

EzrealUI::~EzrealUI()
{
}

void EzrealUI::Update()
{
	CreateRec();
	IsOnMousePos();

	if (isMouseOn)
	{
		if (KEY_DOWN(VK_LBUTTON))
		{
			jinUI->SetIsClick(false);
			isClick = true;
			AUDIO->Play("SelectCharacter");
		}
	}
}

void EzrealUI::PreRender()
{
	renderTarget->Set(depthStencil);

	Render();
}

void EzrealUI::Render()
{
	Sprite::Render();
	if (isMouseOn)
		outline->Render();
}

void EzrealUI::PostRender()
{
}
