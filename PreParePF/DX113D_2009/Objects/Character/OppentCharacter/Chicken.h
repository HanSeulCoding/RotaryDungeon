#pragma once
class Chicken : public Monster
{

public:
	Chicken(int index);
	~Chicken();

	void Update();
	void Render();
};