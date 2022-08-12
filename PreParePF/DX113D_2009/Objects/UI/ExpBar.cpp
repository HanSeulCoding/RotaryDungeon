#include "Framework.h"

ExpBar::ExpBar(Vector3 position, Vector3 scale)
	:Texture2D(L"Textures/UI/ExpBar.png",L"ExpBar")
{
	expBuffer = new ExpBuffer();
	localPosition = position;// Vector3(258, 615, 0);
	localScale = scale;// Vector3(415, 64, 1);
}

ExpBar::~ExpBar()
{
	delete expBuffer;
}

void ExpBar::Update()
{
}

void ExpBar::PreRender()
{
}

void ExpBar::Render()
{
	expBuffer->SetPSBuffer(12);
	Sprite::Render();
	
}

void ExpBar::PostRender()
{
	ImGui::SliderFloat3("EXPBarPositionX", (float*)&localPosition, 0, 1000);
	ImGui::SliderFloat3("ExpBarScale", (float*)&localScale, 0, 1000);
}
