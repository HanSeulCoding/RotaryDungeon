#include "Framework.h"

Hill::Hill(Vector3 pos, Vector3 scale, Vector3 rot)
	:ModelReader("hill")
{
	SetShader(L"Lighting");
	localPosition = pos;
	localScale = scale;
	localRotation = rot;
}

Hill::~Hill()
{
}

void Hill::Render()
{
	SetWorldBuffer();
	MeshRender();
}

void Hill::PostRender()
{
	ImGui::InputFloat3("pos", (float*)&localPosition);
	ImGui::InputFloat3("scale", (float*)&localScale);
	ImGui::InputFloat3("rotation", (float*)&localRotation);
}
