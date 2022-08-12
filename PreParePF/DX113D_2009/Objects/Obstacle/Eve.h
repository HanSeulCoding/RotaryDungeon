#pragma once
class Eve : public Obstacle
{
public:

	Eve();
	~Eve();

	virtual void Update() override;
	virtual void Render() override;

	// Obstacle을(를) 통해 상속됨
	virtual void PostRender() override;
};