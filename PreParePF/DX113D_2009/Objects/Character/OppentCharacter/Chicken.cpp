#include "Framework.h"

Chicken::Chicken(int index)
	:Monster("chicken")
{
	attribute = "animal";
	moveSpeed = 20;
	rotSpeed = 2.0f;
	maxMoveSpeed = 10.0f;
	accelation = 15.0f;
	deceleration = 10.0f;
	tag = "chicken";
	hp = 30;
	curHp = hp;
	this->index = index;
	localScale *= 0.2f;

	localPosition.x = rand() / (float)RAND_MAX * 380;
	localPosition.z = rand() / (float)RAND_MAX * 380;
	SetShader(L"ModelAnimation");

	ReadClip(0,"Chicken/Idle");
	ReadClip(1,"Chicken/Run");
	ReadClip(2,"Chicken/Walk");
	ReadClip(3, "Chicken/Eat");
	ReadClip(4,"Chicken/Dead");
	
	//collider->localPosition.y = 100;
	int count = rand() % 2 + 1;
	switch (count)
	{
	case 1 :	ITEMMANAGER->CreateMeatItem("monster", world,index);
		break;
	case 2:
		ITEMMANAGER->CreateHpPotionItem("monster", world,index);
		break;
	default:
		break;
	}
	FONT->Add("capture", L"a Attack Graffiti", Float3(0, 1, 1), 20);
	collider->localScale *= 30;
	collider->UpdateWorld();
	
}

Chicken::~Chicken()
{
	delete meat;
	delete collider;
}

void Chicken::Update()
{
	
	Monster::Update();
	if (!isActive)
		return;

}

void Chicken::Render()
{
	
	Monster::Render();
	if (isGetChicken)
	{
		wstring s4;
		s4 = L"If you catch Chicken (PRESS G)";
		Float2 pos4;
		Vector3 capturedChickenP = WorldToScreen(localPosition);
		pos4.x = capturedChickenP.x;
		pos4.y = capturedChickenP.y-5;

		Float2 size4 = { 180,100 };
		FONT->RenderText(s4, "capture", pos4, size4);
	}
}



