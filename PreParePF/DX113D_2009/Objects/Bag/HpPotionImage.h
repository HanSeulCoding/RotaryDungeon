#pragma once
class HpPotionImage : public Texture2D
{
public:
	HpPotionImage();
	~HpPotionImage();

	void Update();
	void Render();
	void PreRender();
	void PostRender();

};