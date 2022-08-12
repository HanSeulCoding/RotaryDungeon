#pragma once
class ItemImage : public Texture2D
{
private:
	MeatImage* meatImage;

	// Texture2D을(를) 통해 상속됨
	

	// Texture2D을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};