#pragma once
class StartScene : public Scene
{
private:
	StartSceneBgUI* startSceneBgUI;
	StartButton* startButton;
public:
	StartScene();
	~StartScene();



	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};