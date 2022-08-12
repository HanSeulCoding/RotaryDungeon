#include "Framework.h"

HpPotionImage::HpPotionImage()
	:Texture2D(L"Textures/Bag/HpPotion.png")
{
}

HpPotionImage::~HpPotionImage()
{
}

void HpPotionImage::Update()
{
	CreateRec();
	IsOnMousePos();
}

void HpPotionImage::Render()
{
	Sprite::Render();
}

void HpPotionImage::PreRender()
{
}

void HpPotionImage::PostRender()
{
}
