#include "Framework.h"

Statue::Statue()
	:Obstacle("statue")
{
	isActive = true;
	localScale = Vector3(12.9, 12.9, 12.5);
	SetDiffuseMap(L"ModelData/Materials/Obstacle/Dungeons_Texture_01_B.png");
	collider = new BoxCollider();
	collider->SetParent(&world);
	collider->localPosition = Vector3(0, 1.5, 0);
	collider->localScale = Vector3(1, 3, 1.5);

	tag = "statue";
}

Statue::~Statue()
{
	delete collider;
}

void Statue::Update()
{
}

void Statue::Render()
{
	Obstacle::Render();
	collider->Render();
}

void Statue::PostRender()
{
	string str = tag + " Position";
	ImGui::InputFloat3(str.c_str(), (float*)&localPosition);
	str = tag + " scale";
	ImGui::InputFloat3(str.c_str(), (float*)&localScale);
	str = tag + " rotation";
	ImGui::InputFloat3(str.c_str(), (float*)&localRotation);

	str = tag + " ColliderPosition";
	ImGui::InputFloat3(str.c_str(), (float*)&collider->localPosition);
	str = tag + " Colliderscale";
	ImGui::InputFloat3(str.c_str(), (float*)&collider->localScale);
	str = tag + " Colliderrotation";
	ImGui::InputFloat3(str.c_str(), (float*)&collider->localRotation);
}
