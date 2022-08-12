#pragma once
class JinUI;
class EzrealUI : public Texture2D
{
	JinUI* jinUI;
	bool isClick;
	Outline* outline;
public:
	EzrealUI();
	~EzrealUI();

	// Texture2D을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	bool GetIsClick() { return isClick; }
	void SetIsClick(bool value) { isClick = value; }

	void SetJinUI(JinUI* value) { jinUI = value; }
};