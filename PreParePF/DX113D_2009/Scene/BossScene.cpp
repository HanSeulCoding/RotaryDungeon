#include "Framework.h"
#include "BossScene.h"

BossScene::BossScene()
{
}

BossScene::~BossScene()
{
}

void BossScene::Start()
{
	GMANAGER->SetIsBoss(true);
	AUDIO->Stop("BattleSceneBg");
	AUDIO->Stop("SelectBg");
	AUDIO->Play("BossSceneBG");
	GMANAGER->playerTag = "jin";

	terrain = new Terrain(L"Terrain", L"TextData/BossScene.data");
	bagImage = new BagImage();
	inventory = new Inventory();
	meatImage = new MeatImage();
	chickenImage = new ChickenImage();
	obstacleM = new ObstacleManager();
	boss = new Boss();
	skillImage = new SkillImage();
	if (GMANAGER->playerTag == "jin")
		player = new Jin();

	if (GMANAGER->playerTag == "ezreal")
		player = new Ezreal();

	fox = new Fox();
	sky = new SkyBox(L"Textures/Environment/BossSkyBox.dds");
	auroras = new aurora(Vector3(60, 8, 150));

	obstacleM->CreateEve(Vector3(40,0,150));

	player->localPosition = Vector3(60, 0, 150);

	boss->SetPlayer(player);
	boss->SetTerrain(terrain);
	boss->SetStageNum(3);

	player->SetBoss(boss);
	fox->SetBoss(boss);
	player->SetStageNum(3);
	fox->SetStageNum(3);
	
	player->SetTerrain(terrain);
	fox->SetTerrain(terrain);
	fox->SetPlayer(player);
	player->SetPlayer(fox);
	player->SetInventory((Inventory*)inventory);
	fox->SetInventory((Inventory*)inventory);
	player->SetObstalce(obstacleM->obstacles);
	fox->SetObstalce(obstacleM->obstacles);

	CAMERA->SetTarget(player);
	ITEMMANAGER->SetInventory((Inventory*)inventory);


	ParticleManager::Get();
	ITEMMANAGER->LoadData();
	CAMERA->SetRotDamping(6);
	startSceneBg = new StartSceneBgUI();
}

void BossScene::End()
{
	delete terrain;
	delete fox;
	delete player;
	delete boss;
	delete sky;
	delete bagImage;
	delete inventory;
	delete meatImage;
	delete obstacleM;
	delete auroras;
	int size = ITEMMANAGER->items["player"].size();
	for (int i = 0; i < size; i++)
	{
		ITEMMANAGER->items["player"].erase(ITEMMANAGER->items["player"].begin());
	}
	delete startSceneBg;
	delete skillImage;
}

void BossScene::Update()
{
	int hanseul = ITEMMANAGER->GetItems()["player"].size();
	GMANAGER->playTime += DELTA;

	if (!GMANAGER->GetExcuteInventory())
	{
		fox->Update();
		//chicken->Update();
		player->Update();
		bagImage->Update();
		boss->Update();
		ParticleManager::Get()->Update();

	}
	if (GMANAGER->GetExcuteInventory())
		inventory->Update();

	obstacleM->Update();
	PlayerAuroraCollision(); //다음 스테이지로 넘어가는 오오라랑 충돌하는지 판단.
	auroras->Update();
	if (auroras->GetIsChangedScene())
	{
		string str = GMANAGER->playerTag;
		player->SaveData(ToWString(str));
		fox->SaveData(L"Fox");
		ITEMMANAGER->SaveData();
		int size = ITEMMANAGER->items["monster"].size();
		for (int i = 0; i < size; i++)
		{
			ITEMMANAGER->items["monster"].erase(ITEMMANAGER->items["monster"].begin()); //몬스터가 가지고있는 아이템전부 지우기
			//int size = ITEMMANAGER->items["monster"].size();
		}
		SCENE->ChangeScene(1);
	}
	skillImage->Update();
}

void BossScene::PreRender()
{
	bagImage->PreRender();
	meatImage->PreRender();
}

void BossScene::Render()
{
	
	sky->Render();

	fox->Render();
	player->Render();
	boss->Render();
	terrain->Render();
	BossItemRender();
	obstacleM->Render();
	auroras->Render();
	chickenImage->Render();
	bagImage->Render();


	ParticleManager::Get()->Render();

	if (GMANAGER->GetExcuteInventory())
	{
		inventory->Render();
	}
}

void BossScene::PostRender()
{
	auroras->PostRender();
	player->PostRender();
	boss->PostRender();
	fox->PostRender();
	chickenImage->PostRender();
	bagImage->PostRender();
	obstacleM->PostRender();
	skillImage->Render();
}

void BossScene::BossItemRender()
{
}

void BossScene::PlayerAuroraCollision()
{
	auroras->PlayerCollision(player->GetCollider());
}
