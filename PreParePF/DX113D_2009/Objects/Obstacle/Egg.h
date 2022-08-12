#pragma once
class Monster;
class Egg : public Obstacle
{
private:
	float time;
	float hatchTime;

public:
	Egg();
	~Egg();

	// Obstacle을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	
};