#include "Framework.h"

StartButton::StartButton()
	:Texture2D(L"Textures/UI/startButton.png")
{
	localPosition = Vector3(1180, 50, 0);
	localScale = Vector3(100, 100, 0);
	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();

	outline = new Outline();
	outline->SetSRV(renderTarget->GetSRV());
	outline->localPosition = { WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, 0 };
	outline->localScale = { WIN_WIDTH, WIN_HEIGHT, 1 };
}

StartButton::~StartButton()
{
	delete renderTarget;
	delete depthStencil;
	delete outline;
}

void StartButton::Update()
{
	CreateRec();
	IsOnMousePos();

	if (isMouseOn)
	{
		if (KEY_DOWN(VK_LBUTTON))
		{
			isClick = true;
			AUDIO->Play("StartScene");
		}
	}
}

void StartButton::PreRender()
{
	renderTarget->Set(depthStencil);

	Render();
}

void StartButton::Render()
{
	Sprite::Render();
	if (isMouseOn)
		outline->Render();
}

void StartButton::PostRender()
{
	ImGui::InputFloat3("outlinePositionX", (float*)&localPosition);
	ImGui::InputFloat3("outlineScale", (float*)&localScale);
}
