#pragma once
class Players;
class Weapon : public Item
{
public:
	Weapon();
	~Weapon();


		// Item��(��) ���� ��ӵ�
	virtual void Function(Players* player) override;
	virtual void Update() override; 
	void Render();
};