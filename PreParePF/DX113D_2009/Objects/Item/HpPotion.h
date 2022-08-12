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
	// Item을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Function(Players* player) override;
	void PostRender();
	void PreRender();
};