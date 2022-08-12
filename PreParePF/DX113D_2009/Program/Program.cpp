#include "Framework.h"

#include "Scene/ModelExportScene.h"
#include "Scene/GrowingGameScene.h"
#include "Scene/FarmingScene.h"
#include "Scene/SelectCharacterScene.h"
#include "Scene/StartScene.h"
#include "Scene/BossScene.h"

Program::Program()
{
	Create();

	DirectWrite::Get()->Add("Basic", L"¹ÙÅÁÃ¼");
	CreateAudio();

	SCENE->Add(0, new GrowingGameScene());
	SCENE->Add(1, new FarmingScene());
	SCENE->Add(2, new SelectCharacterScene());
	SCENE->Add(3, new BossScene());
	//SCENE->Add(3, new StartScene());
	SCENE->ChangeScene(2);
}

Program::~Program()
{
	delete scene;

	Delete();
}

void Program::Update()
{
	if (KEY_DOWN(VK_F6))
		Collider::isVisible = !Collider::isVisible;
	Control::Get()->Update();
	Timer::Get()->Update();

	SCENE->Update();

	CAMERA->Update();
	
}

void Program::PreRener()
{
	Environment::Get()->Set();

	SCENE->PreRender();
}

void Program::Render()
{
	Device::Get()->SetRenderTarget();
	Environment::Get()->Set();
	FONT->GetDC()->BeginDraw();
	wstring fps = L"FPS : " + to_wstring(Timer::Get()->GetFPS());
	DirectWrite::Get()->RenderText(fps, "Basic", Float2(50, 50), Float2(100, 100));

	SCENE->Render();
	FONT->GetDC()->EndDraw();
}

void Program::PostRender()
{
	ImGui_ImplDX11_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();
	Environment::Get()->PostRender();
	SCENE->PostRender();

	//DirectWrite::Get()->GetDC()->EndDraw();

	ImGui::Render();
	ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());
}

void Program::Create()
{
	Device::Get();
	Control::Get();
	DirectWrite::Get();
	Timer::Get();
	Environment::Get();

	ImGui::CreateContext();
	ImGui::StyleColorsDark();

	ImGui_ImplWin32_Init(hWnd);
	ImGui_ImplDX11_Init(DEVICE, DC);
}

void Program::Delete()
{
	Device::Delete();
	Control::Delete();
	DirectWrite::Delete();
	Timer::Delete();
	Shader::Delete();
	Texture::Delete();
	Environment::Delete();
}

void Program::CreateAudio()
{
	AUDIO->Add("SelectBg", "Sound/SelectBackground.mp3", true);
	AUDIO->Add("EzrealSelect", "Sound/EzrealSelect.mp3", false);
	AUDIO->Add("JinSelect", "Sound/JinSelect.mp3", false);
	AUDIO->Add("SelectCharacter", "Sound/SelectCharacter.mp3", false);
	AUDIO->Add("FarmingSceneBg", "Sound/FarmingSceneBg.mp3", true);
	AUDIO->Add("BattleSceneBg", "Sound/BattleSceneBg.mp3", true);
	AUDIO->Add("damageSound", "Sound/DamageSound.mp3", false);
	AUDIO->Add("ezrealAttack", "Sound/EzrealAttack.mp3", false);
	AUDIO->Add("jinAttackSound", "Sound/JinAttackSound.mp3", false);
	AUDIO->Add("DamagedPlayer", "Sound/DamagedPlayer.mp3", false);
	AUDIO->Add("BossSceneBG", "Sound/BossSceneBG.mp3", true);
	AUDIO->Add("VisionAudio", "Sound/VisionAudio.mp3", false);
	AUDIO->Add("JumpAttack", "Sound/JumpAttack.mp3", false);
	AUDIO->Add("BossAttackSound", "Sound/BossAttackSound.mp3", false);
	AUDIO->Add("RollSound", "Sound/RollSound.mp3", false);
	AUDIO->Add("BossDieSound", "Sound/BossDieSound.mp3", false);
	AUDIO->Add("FallingBossSound", "Sound/FallingBossSound.mp3", false);
}
