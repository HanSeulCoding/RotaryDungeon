#include "Framework.h"
#include "FarmingScene.h"

FarmingScene::FarmingScene()
{
}
FarmingScene::~FarmingScene()
{
}
void FarmingScene::Start()
{
	AUDIO->Stop("SelectBg");
	AUDIO->Stop("BattleSceneBg");
	AUDIO->Play("FarmingSceneBg");

	GMANAGER->LoadData();
	CreateHill();
	terrain = new Terrain(L"Terrain",L"TextData/Terrain/ground3.data");
	bagImage = new BagImage();
	inventory = new Inventory();
	meatImage = new MeatImage();
	chickenImage = new ChickenImage();
	monsterManager = new MonsterManager(terrain);
	obstacleM = new ObstacleManager();
	skillImage = new SkillImage();

	if(GMANAGER->playerTag == "jin")
		player = new Jin();

	if (GMANAGER->playerTag == "ezreal")
		player = new Ezreal();

	fox = new Fox();
	sky = new SkyBox();
	auroras = new aurora(Vector3(260,3,150));
	auroras->SetStageNum(2);
	grass = new Grass(terrain);
	startSceneBg = new StartSceneBgUI();

	player->ghostCollider->localPosition = player->localPosition;
	fox->ghostCollider->localPosition = fox->localPosition;

	obstacleM->CreateStatue(Vector3(280,1,150),Vector3(0,1.5,0),Vector3(10,10,10));
	monsterManager->SetStageNum(2);
	monsterManager->SetObstacles(obstacleM);

	if(GMANAGER->GetFarmingChickenNum() != 0) //로드 한 치킨 숫자 만큼 만들어주기
		monsterManager->CreateChicken(GMANAGER->GetFarmingChickenNum());

	player->SetStageNum(2);
	fox->SetStageNum(2);
	player->SetTerrain(terrain);
	fox->SetTerrain(terrain);
	fox->SetPlayer(player);
	player->SetPlayer(fox);
	player->SetMonsterM(monsterManager);
	fox->SetMonsterM(monsterManager);
	player->SetInventory((Inventory*)inventory);
	fox->SetInventory((Inventory*)inventory);


	CAMERA->SetTarget(player);
	ITEMMANAGER->SetInventory((Inventory*)inventory);


	ParticleManager::Get();
	ITEMMANAGER->LoadData();
	CAMERA->SetRotDamping(6);
	monsterManager->Breeding(false);
}

void FarmingScene::End()
{
	delete terrain;
	for (int i = 0; i < hills.size(); i++)
	{
		hills.erase(hills.begin());
	}
	delete fox;
	delete player;
	delete sky;
	//delete monsterManager;
	delete bagImage;
	delete inventory;
	delete walls;
	delete meatImage;
	delete obstacleM;
	delete auroras;
	delete grass;
	int size = ITEMMANAGER->items["player"].size();
	for (int i = 0; i < size; i++)
	{
		ITEMMANAGER->items["player"].erase(ITEMMANAGER->items["player"].begin());
	}
}



void FarmingScene::Update()
{
	time += DELTA;
	if (time >= 1.0)
	{
		player->SetIsPress(true);
		fox->SetIsPress(true);
	}
	GMANAGER->playTime += DELTA;

	
	player->SetMonsterManager(monsterManager->GetMonsters());
	fox->SetMonsterManager(monsterManager->GetMonsters());
	player->SetObstalce(obstacleM->obstacles);
	fox->SetObstalce(obstacleM->obstacles);

	if (!GMANAGER->GetExcuteInventory())
	{
		fox->Update();
		monsterManager->Update();
		player->Update();
		bagImage->Update();
		ParticleManager::Get()->Update();
	}
	if (GMANAGER->GetExcuteInventory())
		inventory->Update();
	
	obstacleM->Update();
	PlayerAuroraCollision();
	auroras->Update();
	monsterManager->Breeding(true);
	skillImage->Update();
	ChangeScene();
}
void FarmingScene::Render()
{
	sky->Render();
	
	terrain->Render();
	fox->Render();
	player->Render();
	terrain->Render();
	MonsterItemRender();
	monsterManager->Render();
	
	for (auto hill : hills)
		hill->Render();

	obstacleM->Render();

	auroras->Render();
	chickenImage->Render();
	bagImage->Render();

	ParticleManager::Get()->Render();
	grass->Render();

	if (GMANAGER->GetExcuteInventory())
	{
		inventory->Render();
	}
	
}

void FarmingScene::PostRender()
{

	player->PostRender();
	fox->PostRender();
	skillImage->Render();
	skillImage->PostRender();
	bagImage->PostRender();
}

void FarmingScene::PreRender()
{
	bagImage->PreRender();
	meatImage->PreRender();
}

void FarmingScene::CreateHill()
{
	hill = new Hill(Vector3(200, 55, -60), Vector3(0.07, 0.1, 0.1), Vector3(1.5, 0, 0));
	hills.emplace_back(hill);
	hill = new Hill(Vector3(130, 65, 380), Vector3(0.07, 0.1, 0.1), Vector3(1.5, 0, 3.1));
	hills.emplace_back(hill);
	hill = new Hill(Vector3(-50, 55, 120), Vector3(0.07, 0.1, 0.1), Vector3(1.5, 0, -1.6));
	hills.emplace_back(hill);
	hill = new Hill(Vector3(380, 50, 200), Vector3(0.07, 0.1, 0.1), Vector3(1.5, 3.1, -1.6));
	hills.emplace_back(hill);
}

void FarmingScene::MonsterItemRender()
{
	if (ITEMMANAGER->GetItems()["monster"].size() <= 0)
		return;
	for (auto monster : monsterManager->GetMonsters())
	{
		for (auto item : ITEMMANAGER->GetItems()["monster"])
		{
			if (!monster->isActive)
			{
				if (monster->GetIndex() == item->GetIndex())
				{
					item->SetIsActive(true);
					break;
				}
			}
		}
	}
	ITEMMANAGER->Render("monster");
}

void FarmingScene::PlayerAuroraCollision()
{
	auroras->PlayerCollision(player->GetCollider());
}

void FarmingScene::ChangeScene()
{
	if (auroras->GetIsChangedScene())
	{
		string str = GMANAGER->playerTag;
		player->SaveData(ToWString(str));
		fox->SaveData(L"Fox");
		ITEMMANAGER->SaveData();
		int size = ITEMMANAGER->items["monster"].size();
		GMANAGER->SetFarmingChickenNum(monsterManager->GetMonsters().size());
		GMANAGER->SaveData();
		for (int i = 0; i < size; i++)
		{
			ITEMMANAGER->items["monster"].erase(ITEMMANAGER->items["monster"].begin());
		}

		SCENE->ChangeScene(0);
	}
}
