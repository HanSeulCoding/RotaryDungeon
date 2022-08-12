#include "Framework.h"

BossUI::BossUI()
	:Texture2D(L"Textures/UI/BossUI.png")
{
	localScale = Vector3(64, 64, 1);
	localPosition = Vector3(354, 64, 0);
}

BossUI::~BossUI()
{
}

void BossUI::Update()
{
}

void BossUI::PreRender()
{
}

void BossUI::Render()
{
	Sprite::Render();
}

void BossUI::PostRender()
{
	ImGui::SliderFloat3("BossUIPositionX", (float*)&localPosition, 0, 1000);
	ImGui::SliderFloat3("BossUIScale", (float*)&localScale, 0, 1000);
}
