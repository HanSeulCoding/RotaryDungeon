#include "Framework.h"

Jin_E::Jin_E()
	:Texture2D(L"Textures/UI/jin_e.png")
{
	blackWhiteBuffer = new BlackWhiteBuffer();

	//material->SetDiffuseMap(L"Textures/UI/ezreal_e.png");
	localPosition = Vector3(1030, 160, 0);
	localScale = Vector3(60, 60, 60);
}

Jin_E::~Jin_E()
{
	delete blackWhiteBuffer;
}

void Jin_E::Update()
{
	if (GMANAGER->GetIsPlayerESkill())
		blackWhiteBuffer->data.hasBlack = 1;
	if (!GMANAGER->GetIsPlayerESkill())
		blackWhiteBuffer->data.hasBlack = 0;
}

void Jin_E::PreRender()
{
}

void Jin_E::Render()
{
	Sprite::Render();
}

void Jin_E::PostRender()
{
}
