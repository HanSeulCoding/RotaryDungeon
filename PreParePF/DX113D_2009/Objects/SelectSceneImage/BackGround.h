#pragma once
class BackGround : public Texture2D
{
public:
	BackGround();
	~BackGround();

	// Texture2D��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};