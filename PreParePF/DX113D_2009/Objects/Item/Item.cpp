#include "Framework.h"

Item::Item(string file)
	: ModelReader(file),isActive(true),isDrop(false),isRender(false),isEat(false),
	isCheck(false)
{
	SetShader(L"Lighting");

	Vector3 min, max;
	SetBox(&min, &max);

	collider = new BoxCollider(min, max);
	collider->tag = "WeaponCollider";	

	collider->SetParent(&world);
	FONT->Add("item", L"a Attack Graffiti", Float3(0, 0, 0), 20);
}

Item::~Item()
{
}

void Item::Render()
{
	if (!isActive)
		return;
	SetWorldBuffer();
	MeshRender();

	collider->Render();

	wstring s;
	s = L" ащ╠Б (Press F) ";
	Float2 pos;
	Vector3 fontPosition = collider->localPosition;
	pos.x = fontPosition.x;
	pos.y = fontPosition.y + 5;

	Float2 size = { 200,100 };
	FONT->RenderText(s, "changeScene", pos, size);
}



void Item::DropItem(bool isDrop, float height)
{
	this->isDrop = isDrop;
	this->height = height;
}



