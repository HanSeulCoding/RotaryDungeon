#include "Framework.h"

Egg::Egg()
	:Obstacle("egg"),hatchTime(10)
{
	isActive = true;
	collider = new BoxCollider();
	collider->SetParent(&world);
	collider->localPosition = Vector3(0, 1.5, 0);
	collider->localScale = Vector3(2, 3, 1.5);



	SetDiffuseMap(L"ModelData/Materials/egg.jpg");
	tag = "egg";
}

Egg::~Egg()
{
}

void Egg::Update()
{
	if (!isActive)
		return;
	time += DELTA;
	localPosition.y = terrain->GetHeight(localPosition)+3;
	if (time > hatchTime)
	{
		isActive = false;
		isDelete = true;
		float x = rand() % 200 + 50;
		float z = rand() % 200 + 50;
		Vector3 position = Vector3(x, 0, z);
		monsters->CreateIndexChicken(0, localPosition);
	}
}

void Egg::Render()
{
	if (!isActive)
		return;
	Obstacle::Render();
	collider->Render();
}

void Egg::PostRender()
{
	string str = tag + " Position";
	ImGui::InputFloat3(str.c_str(), (float*)&localPosition);
	str = tag + " scale";
	ImGui::InputFloat3(str.c_str(), (float*)&localScale);
	str = tag + " rotation";
	ImGui::InputFloat3(str.c_str(), (float*)&localRotation);
}
