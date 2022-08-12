#include "Framework.h"

StartSceneBgUI::StartSceneBgUI()
	:Texture2D(L"Textures/UI/RotaryDungeonBg.png")
{
	localScale = Vector3(WIN_WIDTH, WIN_HEIGHT, 0);
	localPosition = Vector3(WIN_WIDTH * 0.5f, WIN_HEIGHT * 0.5f, 0);
}

StartSceneBgUI::~StartSceneBgUI()
{
}

void StartSceneBgUI::Update()
{
}

void StartSceneBgUI::PreRender()
{
}

void StartSceneBgUI::Render()
{
	Sprite::Render();
}

void StartSceneBgUI::PostRender()
{
}
