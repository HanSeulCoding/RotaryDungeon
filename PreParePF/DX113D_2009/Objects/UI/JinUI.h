#pragma once
class Texture2D;

class JinUI : public Texture2D
{
private:
	bool isClick;
	EzrealUI* ezrealUI;
	Outline* outline;
public:
	JinUI();
	~JinUI();

	// Texture2D을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	bool GetIsClick() { return isClick; }
	void SetIsClick(bool value) { isClick = value; }
	void SetEzrealUI(EzrealUI* value) { ezrealUI = value; }


};