#pragma once
class Lightning : public Item
{
public:
	bool isActive;
	bool isCollision;
	bool isGo;
	Vector3 direction;
	Lightning();
	~Lightning();
	

	void Render();
	// Item��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Function(Players* player) override;
	void Move();

};