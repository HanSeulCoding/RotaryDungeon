#pragma once
class Players;
class Meat : public Item
{
private:
	int incHp;
	int incExp;
public:
	
	Meat();
	~Meat();

	virtual void Update() override;
	virtual void Function(Players* player) override;

	void Render();
};