#include "Framework.h"
#include "StartScene.h"

StartScene::StartScene()
{
	startButton = new StartButton();
	startSceneBgUI = new StartSceneBgUI();
}

StartScene::~StartScene()
{
	delete startSceneBgUI;
	delete startButton;
}

void StartScene::Update()
{
	startButton->Update();
	if (startButton->GetIsClick())
		SCENE->ChangeScene(2);
}

void StartScene::PreRender()
{
	startButton->PreRender();
}

void StartScene::Render()
{
	startSceneBgUI->Render();
	startButton->Render();
}

void StartScene::PostRender()
{
	ImGui::InputFloat3("pos", (float*)&startButton->localPosition);
	//ImGui::InputFloat3("pos", (float*)&startButton->localPosition);
}
