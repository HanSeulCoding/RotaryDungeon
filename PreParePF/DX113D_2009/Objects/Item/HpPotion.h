#pragma once
class Players;
class HpPotion : public Item
{
private:
	int incHp;
public:
	HpPotion();
	~HpPotion();

	void Render();
	// Item��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Function(Players* player) override;
	void PostRender();
	void PreRender();
};