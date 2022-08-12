#include "Framework.h"

Eve::Eve()
	:Obstacle("Eve")
{
	isActive = true;
	tag = "eve";
	localPosition = Vector3(258, 0, 375);
	localScale = Vector3(12.9, 12.9, 12.5);

	collider = new BoxCollider();
	collider->SetParent(&world);
	collider->localPosition = Vector3(0, 1.5, 0);
	collider->localScale = Vector3(1, 3, 1.5);
	SetDiffuseMap(L"ModelData/Materials/Obstacle/Dungeons_Texture_01_B.png");

}

Eve::~Eve()
{
}

void Eve::Update()
{
}

void Eve::Render()
{
	Obstacle::Render();
	collider->Render();
}

void Eve::PostRender()
{
	string str = tag + " Position";
	ImGui::InputFloat3(str.c_str(), (float*)&collider->localPosition);
	str = tag + " scale";
	ImGui::InputFloat3(str.c_str(), (float*)&collider->localScale);
	str = tag + " rotation";
	ImGui::InputFloat3(str.c_str(), (float*)&collider->localRotation);
}
