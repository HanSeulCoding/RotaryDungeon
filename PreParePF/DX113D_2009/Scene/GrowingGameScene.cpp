#include "Framework.h"
#include "GrowingGameScene.h"

GrowingGameScene::GrowingGameScene()
	:isLoading(true)
{
	
}
void GrowingGameScene::Start()
{
	AUDIO->Stop("FarmingSceneBg");

	AUDIO->Play("BattleSceneBg");
	terrain = new Terrain(L"Terrain", L"TextData/Terrain/Terrain.data");
	walls = new WallManager();
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
	auroras = new aurora(Vector3(260,0,360));

	obstacleM->CreateEve();
	monsterManager->SetObstacles(obstacleM);
	monsterManager->CreateChicken(10);
	monsterManager->CreateFemailEnemy(10);


	player->SetTerrain(terrain);
	fox->SetTerrain(terrain);
	fox->SetPlayer(player);
	player->SetPlayer(fox);
	player->SetMonsterM(monsterManager);
	fox->SetMonsterM(monsterManager);
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
void GrowingGameScene::End()
{
	delete terrain;
	delete fox;
	delete player;
	delete sky;
	delete bagImage;
	delete inventory;
	delete walls;
	delete meatImage;
	delete obstacleM;
	delete auroras;
	int size = ITEMMANAGER->items["player"].size();
	for (int i = 0; i < size; i++)
	{
		ITEMMANAGER->items["player"].erase(ITEMMANAGER->items["player"].begin());
	}
	delete startSceneBg;
}
	

GrowingGameScene::~GrowingGameScene()
{
}

void GrowingGameScene::Update()
{
	loadingTime += DELTA;
	if (loadingTime > 0.5f)
	{
		isLoading = false;
	}
	int hanseul = ITEMMANAGER->GetItems()["player"].size();
	GMANAGER->playTime += DELTA;
	player->SetMonsterManager(monsterManager->GetMonsters());
	fox->SetMonsterManager(monsterManager->GetMonsters());

	if (!GMANAGER->GetExcuteInventory())
	{
		fox->Update();
		//chicken->Update();
		monsterManager->Update();
		player->Update();
		bagImage->Update();
		ParticleManager::Get()->Update();
	}
	if (GMANAGER->GetExcuteInventory())
		inventory->Update();

	skillImage->Update();
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
}

void GrowingGameScene::PreRender()
{
	bagImage->PreRender();
	meatImage->PreRender();

}

void GrowingGameScene::Render()
{
	sky->Render();
	
	fox->Render();
	player->Render();
	terrain->Render();
	MonsterItemRender();
	monsterManager->Render();
	walls->Render();
//	OBSMANAGER->Render();
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

void GrowingGameScene::PostRender()
{
	auroras->PostRender();
	player->PostRender();
	skillImage->Render();
	skillImage->PostRender();
	fox->PostRender();
	chickenImage->PostRender();
	bagImage->PostRender();
	obstacleM->PostRender();
}



void GrowingGameScene::MonsterItemRender()
{
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

void GrowingGameScene::PlayerAuroraCollision()
{
	auroras->PlayerCollision(player->GetCollider());
	//auroras->PlayerCollision(fox->GetCollider());
}
