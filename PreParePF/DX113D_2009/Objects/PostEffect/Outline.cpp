#include "Framework.h"

Outline::Outline()
	: Sprite(L"Outline")
{
	buffer = new OutlineBuffer();
}

Outline::~Outline()
{
	delete buffer;
}

void Outline::Render()
{
	buffer->data.size.x = localScale.x;
	buffer->data.size.y = localScale.y;

	buffer->SetPSBuffer(10);
	Sprite::Render();

	/*ImGui::SliderFloat("Value", &buffer->data.value, 0, 8);
	ImGui::SliderFloat("Range", &buffer->data.value, 0, 8);
	ImGui::ColorEdit4("Color", (float*)&material->GetBuffer()->data.diffuse);*/
}
