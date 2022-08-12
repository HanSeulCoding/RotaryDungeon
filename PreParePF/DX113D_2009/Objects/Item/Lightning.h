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
	// Item을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Function(Players* player) override;
	void Move();

};