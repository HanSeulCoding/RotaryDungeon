#include "Framework.h"

HpBar::HpBar(Vector3 position, Vector3 scale)
	:Texture2D(L"Textures/UI/HpBar.png",L"Bar")
{
	hpBuffer = new HPBuffer();
	localPosition = position; //Vector3(258, 645, 0);
	localScale = scale; //Vector3(322, 85, 0);
	
}

HpBar::~HpBar()
{
}

void HpBar::Update()
{
}

void HpBar::PreRender()
{
}

void HpBar::Render()
{
	hpBuffer->SetPSBuffer(11);
	Sprite::Render();
}

void HpBar::PostRender()
{
	ImGui::SliderFloat3("HPBarPositionX", (float*)&localPosition, 0, 1000);
	ImGui::SliderFloat3("HPBarScale", (float*)&localScale, 0, 1000);
}
