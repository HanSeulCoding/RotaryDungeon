#include "Framework.h"

BossHpBar::BossHpBar()
	:Texture2D(L"Textures/UI/BossHpBar.png",L"Bar")
{
	hpBuffer = new HPBuffer();
	localPosition = Vector3(741, 58, 0);
	localScale = Vector3(806, 64, 1);
}

BossHpBar::~BossHpBar()
{
}

void BossHpBar::Update()
{
}

void BossHpBar::PreRender()
{
}

void BossHpBar::Render()
{
	hpBuffer->SetPSBuffer(11);
	Sprite::Render();
}

void BossHpBar::PostRender()
{
	ImGui::SliderFloat3("BossHpPositionX", (float*)&localPosition, 0, 1000);
	ImGui::SliderFloat3("BossHpScale", (float*)&localScale, 0, 1000);
}
