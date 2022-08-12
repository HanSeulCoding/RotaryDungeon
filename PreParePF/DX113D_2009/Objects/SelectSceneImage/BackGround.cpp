#include "Framework.h"

BackGround::BackGround()
	:Texture2D(L"Textures/UI/SelectBackGround.png")
{
	localScale = Vector3(WIN_WIDTH, WIN_HEIGHT, 0);
	localPosition = { WIN_WIDTH * 0.5,WIN_HEIGHT * 0.5,0 };
}

BackGround::~BackGround()
{
}

void BackGround::Update()
{
}

void BackGround::PreRender()
{
}

void BackGround::Render()
{
	Sprite::Render();
}

void BackGround::PostRender()
{
}
