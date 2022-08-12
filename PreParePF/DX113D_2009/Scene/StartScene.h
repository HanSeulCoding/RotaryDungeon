#pragma once
class StartScene : public Scene
{
private:
	StartSceneBgUI* startSceneBgUI;
	StartButton* startButton;
public:
	StartScene();
	~StartScene();



	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};