#include "Framework.h"

Lightning::Lightning()
	:Item("lightSword")
{
	//SetDiffuseMap(L"ModelData/Materials/Lightning");
	
}

Lightning::~Lightning()
{
}

void Lightning::Render()
{
	if (!isActive) return;
	Item::Render();
}

void Lightning::Update()
{

		
}

void Lightning::Function(Players* player)
{
}

void Lightning::Move()
{
	localPosition -= direction * 30 * DELTA;
	
}
