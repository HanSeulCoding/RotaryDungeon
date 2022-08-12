#include "Framework.h"

aurora::aurora(Vector3 position)
{
	collider = new SphereCollider();
	FONT->Add("changeScene", L"a Attack Graffiti", Float3(0,0, 0), 20);
	collider->localPosition = position; //Vector3(260, 0, 360);
	collider->localScale = Vector3(7, 3, 7);
	isChangedScene = false;
}

aurora::~aurora()
{
	delete collider;
}

void aurora::Update()
{
	if (isCollision)
	{
		if (KEY_DOWN('R'))
			isChangedScene = true;
	
	}
	ParticleManager::Get()->Play("aurora", Vector3(collider->localPosition.x,collider->localPosition.y+2,
		collider->localPosition.z));
}

void aurora::Render()
{
	collider->Render();
	if (stageNum != 2)
	{
		if (isCollision)
		{
			wstring s;
			s = L"If you want to go farm (PRESS R)";
			Float2 pos;
			Vector3 fontPosition = WorldToScreen(collider->localPosition);
			pos.x = fontPosition.x;
			pos.y = fontPosition.y + 5;

			Float2 size = { 200,100 };
			FONT->RenderText(s, "changeScene", pos, size);
		}
	}
	if (stageNum == 2)
	{
		if (isCollision)
		{
			wstring s;
			s = L"If you want to go hunting (PRESS R)";
			Float2 pos;
			Vector3 fontPosition = WorldToScreen(collider->localPosition);
			pos.x = fontPosition.x;
			pos.y = fontPosition.y + 5;

			Float2 size = { 200,100 };
			FONT->RenderText(s, "changeScene", pos, size);
		}
	}
}

void aurora::PostRender()
{
	ImGui::InputFloat3("auroraPosition", (float*)&collider->localPosition);
	ImGui::InputFloat3("auroraScale", (float*)&collider->localScale);

}

void aurora::PlayerCollision(Collider* targetCollider)
{
	if (collider->SphereCollision((SphereCollider*)targetCollider))
	{
		isCollision = true;
	}
	else
		isCollision = false;
}
