#include "Framework.h"

JinUI::JinUI()
	:Texture2D(L"Textures/UI/Jin.png"),isClick(true)
{
	localPosition = Vector3 ( 50, 645, 0 );// Vector3(560, 100, 0);
	localScale = Vector3( 64, 64, 0 );

	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();

	outline = new Outline();
	outline->SetSRV(renderTarget->GetSRV());
	outline->localPosition = { WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, 0 };
	outline->localScale = { WIN_WIDTH, WIN_HEIGHT, 1 };
	
}

JinUI::~JinUI()
{
	delete renderTarget;
	delete depthStencil;
	delete outline;
}


void JinUI::Update()
{
	CreateRec();
	IsOnMousePos();

	if (isMouseOn)
	{
		if (KEY_DOWN(VK_LBUTTON))
		{
			ezrealUI->SetIsClick(false);
			isClick = true;
			AUDIO->Play("SelectCharacter");
		}
	}

}

void JinUI::PreRender()
{
	renderTarget->Set(depthStencil);

	Render();
}

void JinUI::Render()
{

	Sprite::Render();
	if(isMouseOn)
		outline->Render();
}

void JinUI::PostRender()
{
	
	/*ImGui::SliderFloat3("outlinePositionX", (float*)&outline->localPosition, -1000, 1000);
	ImGui::SliderFloat3("outlineScale", (float*)&outline->localScale, 0, 1000);*/
}


