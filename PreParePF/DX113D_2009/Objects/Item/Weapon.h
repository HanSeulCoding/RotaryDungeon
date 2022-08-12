#pragma once
class Players;
class Weapon : public Item
{
public:
	Weapon();
	~Weapon();


		// Item을(를) 통해 상속됨
	virtual void Function(Players* player) override;
	virtual void Update() override; 
	void Render();
};