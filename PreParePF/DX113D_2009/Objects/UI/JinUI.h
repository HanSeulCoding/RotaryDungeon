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

	// Texture2D��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	bool GetIsClick() { return isClick; }
	void SetIsClick(bool value) { isClick = value; }
	void SetEzrealUI(EzrealUI* value) { ezrealUI = value; }


};