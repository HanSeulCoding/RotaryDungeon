#pragma once
class FoxUI : public Texture2D
{
public:
	FoxUI();
	~FoxUI();

	// Texture2D��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};