#pragma once
class BagImage : public Texture2D
{
private:
	
	Vector3 mousePos;
	bool isInventory;

public:
	BagImage();
	~BagImage();

	bool GetMouseOn() { return isMouseOn; }

	// Texture2D��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};