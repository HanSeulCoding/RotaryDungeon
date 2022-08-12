#pragma once
class StartSceneBgUI : public Texture2D
{
private:

public:
	StartSceneBgUI();
	~StartSceneBgUI();

	// Texture2D을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};