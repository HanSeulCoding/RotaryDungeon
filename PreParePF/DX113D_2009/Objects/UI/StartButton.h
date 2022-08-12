#pragma once
class StartButton : public Texture2D
{
	RenderTarget* renderTarget;
	DepthStencil* depthStencil;
	Outline* outline;

	bool isClick;
public:
	StartButton();
	~StartButton();

	// Texture2D��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	bool GetIsClick() { return isClick; }
	void SetIsClick(bool value) { isClick = value; }

};