#include "Framework.h"
#include "SelectCharacterScene.h"

SelectCharacterScene::SelectCharacterScene()
{

	startButton = new StartButton();
	renderTarget = new RenderTarget();
	depthStencil = new DepthStencil();

	CAMERA->localPosition = Vector3(0, 0, 0);
	CAMERA->localRotation = Vector3(-12.50, 12.17, 0);
	jin = new Jin(true);
	jin->localPosition = { -150, -120, 400 };
	jin->localScale = { 1.1,1.1,1.1 };

	ezreal = new Ezreal(true);
	ezreal->localPosition = { -150,-120,400 };
	ezreal->localScale = { 2.2,2.2,2.2 };

	backGroundImage = new BackGround();
	
	jinUI = new JinUI();
	jinUI->localPosition = Vector3(560, 100, 0);
	

	ezrealUI = new EzrealUI();
	ezrealUI->localPosition = Vector3(680, 100, 0);

	jinUI->SetEzrealUI(ezrealUI);
	ezrealUI->SetJinUI(jinUI);

	sprite = new Sprite(L"Texture");
	sprite->SetSRV(renderTarget->GetSRV());
	sprite->localScale = { WIN_WIDTH , WIN_HEIGHT , 1 };
	sprite->localPosition = {630, 380, 0 };

	//CAMERA->SetTarget(outlineTest);
	FONT->Add("select", L"a Attack Graffiti", Float3(0, 0, 0), 60);
	AUDIO->Play("SelectBg");
}

SelectCharacterScene::~SelectCharacterScene()
{
	delete renderTarget;
	delete depthStencil;
	delete backGroundImage;
	delete sprite;
	delete ezrealUI;
	delete jinUI;
	delete startButton;
}

void SelectCharacterScene::Update()
{
	ezreal->Update();
	jin->Update();
	jinUI->Update();
	ezrealUI->Update();
	startButton->Update();
	if (startButton->GetIsClick())
	{
		if (ezrealUI->GetIsClick())
		{
			GMANAGER->playerTag = "ezreal";
			AUDIO->Play("EzrealSelect");
		}
		if (jinUI->GetIsClick())
		{
			GMANAGER->playerTag = "jin";
			AUDIO->Play("JinSelect");
		}
		SCENE->ChangeScene(1);
	}

}

void SelectCharacterScene::PreRender()
{
	
	renderTarget->Set(depthStencil);
	if (ezrealUI->GetIsClick())
	{
		ezreal->Render();
	}
	if (jinUI->GetIsClick())
	{
		jin->Render();
	}
	jinUI->PreRender();
	ezrealUI->PreRender();
	startButton->PreRender();
	
	
}

void SelectCharacterScene::Render()
{
	backGroundImage->Render();
	wstring s;
	s = L"Please select a character";
	Float2 pos;
	pos.x = WIN_WIDTH * 0.5f;
	pos.y = 40;

	Float2 size = { 880,40 };
	FONT->RenderText(s, "select", pos, size);
	
}


void SelectCharacterScene::PostRender()
{
	sprite->Render();
	jinUI->Render();
	ezrealUI->Render();
	startButton->Render();
	startButton->PostRender();
}
