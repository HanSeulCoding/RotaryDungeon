#pragma once
class ChickenItem : public Item
{
public:
	ChickenItem();
	~ChickenItem();

	// Item��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Function(Players* player) override;
};