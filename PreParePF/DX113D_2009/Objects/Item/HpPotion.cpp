#include "Framework.h"

HpPotion::HpPotion()
	:Item("Bottle_red"),incHp(50)
{
	isActive = false;
	SetDiffuseMap(L"ModelData/Materials/Bottle_red_diff.png");
	SetNormalMap(L"ModelData/Materials/Bottles_normal.png");
	SetSpecularMap(L"ModelData/Materials/Bottles_spec.png");
	tag = "hpPotion";
}

HpPotion::~HpPotion()
{
}

void HpPotion::Render()
{
	Item::Render();
}

void HpPotion::Update()
{
	
}

void HpPotion::Function(Players* player)
{
	player->ChangeHp(true, incHp);
}

void HpPotion::PostRender()
{
}

void HpPotion::PreRender()
{
}
