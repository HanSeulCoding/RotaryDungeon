#pragma once
class Statue : public Obstacle
{
public:
	Statue();
	~Statue();

	// Obstacle을(를) 통해 상속됨
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

};