#pragma once
class Eve : public Obstacle
{
public:

	Eve();
	~Eve();

	virtual void Update() override;
	virtual void Render() override;

	// Obstacle��(��) ���� ��ӵ�
	virtual void PostRender() override;
};