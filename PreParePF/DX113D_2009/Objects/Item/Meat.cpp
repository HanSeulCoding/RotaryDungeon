#include "Framework.h"
#include "Meat.h"

Meat::Meat()
	:Item("Meat"),incHp(30),incExp(20)
{
	isActive = false;
	SetDiffuseMap(L"ModelData/Materials/chicken leg piece_AlbedoTransparency.png");
	SetNormalMap(L"ModelData/Materials/chicken leg piece_Normal.png");
	tag = "meat";
}

Meat::~Meat()
{
}

void Meat::Update()
{
	if (isDrop)
	{
		localPosition.y -= 5;
		if (localPosition.y < height)
			localPosition.y = height;
	}
}

void Meat::Function(Players* player)
{
	player->ChangeHp(true, this->incHp);
	player->ChangeExp(incExp);
}

void Meat::Render()
{
	Item::Render();
}

