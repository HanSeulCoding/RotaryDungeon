#pragma once
class StartSceneBgUI : public Texture2D
{
private:

public:
	StartSceneBgUI();
	~StartSceneBgUI();

	// Texture2D��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};