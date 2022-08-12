#pragma once
class ChickenItem : public Item
{
public:
	ChickenItem();
	~ChickenItem();

	// Item을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Function(Players* player) override;
};