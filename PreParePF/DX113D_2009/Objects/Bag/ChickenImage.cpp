#include "Framework.h"

ChickenImage::ChickenImage()
	:Texture2D(L"Textures/Bag/ChickenItem.png")
{
	//localScale = Vector3(96, 64, 0);
	
}

ChickenImage::~ChickenImage()
{
}

void ChickenImage::Update()
{
	CreateRec();
	IsOnMousePos();
}

void ChickenImage::PreRender()
{
}

void ChickenImage::Render()
{
	Sprite::Render();
}

void ChickenImage::PostRender()
{
	ImGui::SliderFloat3("position", (float*)&localPosition, 0, 1000);
	ImGui::SliderFloat3("scale", (float*)&localScale, 0, 1000);
}
