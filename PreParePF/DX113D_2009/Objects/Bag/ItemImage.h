#pragma once
class ItemImage : public Texture2D
{
private:
	MeatImage* meatImage;

	// Texture2D��(��) ���� ��ӵ�
	

	// Texture2D��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};