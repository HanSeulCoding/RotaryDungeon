#pragma once
class Statue : public Obstacle
{
public:
	Statue();
	~Statue();

	// Obstacle��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

};