#include "Framework.h"

Ez_E::Ez_E()
	:Texture2D(L"Textures/UI/ezreal_e.png")
{
	blackWhiteBuffer = new BlackWhiteBuffer();

	material->SetDiffuseMap(L"Textures/UI/ezreal_e.png");
	localPosition = Vector3(1030, 160, 0);
	localScale = Vector3(60, 60, 60);
}

Ez_E::~Ez_E()
{
	delete blackWhiteBuffer;
}

void Ez_E::Update()
{
	if(GMANAGER->GetIsPlayerESkill())
		blackWhiteBuffer->data.hasBlack = 1;
	if(!GMANAGER->GetIsPlayerESkill())
		blackWhiteBuffer->data.hasBlack = 0;
}

void Ez_E::PreRender()
{
}

void Ez_E::Render()
{
	Sprite::Render();
}

void Ez_E::PostRender()
{
}
