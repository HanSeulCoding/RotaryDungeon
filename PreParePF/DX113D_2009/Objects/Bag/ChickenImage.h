#pragma once
class ChickenImage : public Texture2D
{
public:
	ChickenImage();
	~ChickenImage();

	// Texture2D��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};