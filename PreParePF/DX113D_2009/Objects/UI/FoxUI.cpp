#include "Framework.h"

FoxUI::FoxUI()
	:Texture2D(L"Textures/UI/fox.png")
{
	localPosition = Vector3(50, 548, 0);
	localScale = Vector3(64, 64, 0);
}

FoxUI::~FoxUI()
{
}

void FoxUI::Update()
{
}

void FoxUI::PreRender()
{
}

void FoxUI::Render()
{
	Sprite::Render();
}

void FoxUI::PostRender()
{
	ImGui::SliderFloat3("foxUIPositionX", (float*)&localPosition, 0, 1000);
	ImGui::SliderFloat3("foxScale", (float*)&localScale, 0, 1000);
}
