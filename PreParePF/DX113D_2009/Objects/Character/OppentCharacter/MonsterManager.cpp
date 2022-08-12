#include "Framework.h"

MonsterManager::MonsterManager(Terrain* terrain)
	:respawnTime(0.0f),breedingTime(10),time(0.0f), monsterRespawnNum(5)

{
	this->terrain = terrain;

	frustum = new Frustum();
}

MonsterManager::~MonsterManager()
{
	for (auto monster : monsters)
	{
		delete monster;
	}
	delete frustum;
}

void MonsterManager::Update()
{
	for (auto monster : monsters)
		monster->Update();
	if(stageNum!=2)
		Respawn();
	frustum->Update();
}

void MonsterManager::Render()
{
	//drawCount = 0;
	for (auto monster : monsters)
	{
		if (!monster->isActive)
			continue;
		SphereCollider* sphereCollider = (SphereCollider*)monster->GetCollider();
		if(frustum->ContainSphere(sphereCollider->GlobalPosition(), sphereCollider->Radius()+10))
			monster->Render();
	}
}

void MonsterManager::PostRender()
{
	for (auto monster : monsters)
		monster->PostRender();
}

void MonsterManager::PreRender()
{
	for (auto monster : monsters)
		monster->PreRender();
}

void MonsterManager::CreateChicken(int POOL_COUNT)
{
	for (int i = 0; i < POOL_COUNT; i++)
	{
		Monster* chicken = new Chicken(i);
		chicken->SetTerrain(terrain);
		chicken->SetObstalce(obstacles->obstacles);
		chicken->SetStageNum(stageNum);
		monsters.emplace_back(chicken);
	}
}

void MonsterManager::CreateIndexChicken(int index,Vector3 position)
{
	Monster* chicken = new Chicken(monsters.size()+1);
	chicken->SetTerrain(terrain);
	chicken->localPosition = position;
	chicken->SetObstalce(obstacles->obstacles);
	chicken->SetStageNum(2);
	monsters.emplace_back(chicken);
}

void MonsterManager::CreateFemailEnemy(int POOL_COUNT)
{
	for (int i = 0; i < POOL_COUNT; i++)
	{
		Monster* femailEnemy = new FemailEnemy(i+10);
		femailEnemy->SetTerrain(terrain);
		femailEnemy->SetObstalce(obstacles->obstacles);
		femailEnemy->SetStageNum(stageNum);
		monsters.emplace_back(femailEnemy);

	}
}

void MonsterManager::Respawn()
{
	int count = GMANAGER->killCount;
	if (GMANAGER->killCount > monsterRespawnNum)
	{
		respawnTime += DELTA;
		if (respawnTime > 5.0f)
		{
			for (auto monster : monsters)
			{
				if (!monster->isActive) //액티브 꺼져있는 몬스터 몬스터 초기화
				{
					monster->isActive = true; 
					monster->SetIsDie(false);
					monster->SetDieCount(0);
					monster->SetCurHp(monster->GetHp());
					monster->SettingItem();
					monster->localPosition.x = rand() / (float)RAND_MAX * 380; //랜덤으로 몬스터 소환
					monster->localPosition.z = rand() / (float)RAND_MAX * 380;
					GMANAGER->killCount = 0;
				}
			}
			respawnTime = 0.0f;
		}
	}
}

void MonsterManager::Breeding(bool isUpdate)
{
	for (auto monster : monsters)
	{
		if (monster->isActive)
		{
			chickenDieCount++;
		}
	}
	if (!isUpdate)
	{
		if (chickenDieCount < 2)
		{
			chickenDieCount = 0;
			return;
		}
		int aTime = (int)GMANAGER->playTime;
		int size = aTime / breedingTime;
		for (int i = 0; i < size; i++)
		{
			float x = rand() % 200 + 50;
			float z = rand() % 200 + 50;
			Vector3 position = Vector3(x, 0, z);
			obstacles->CreateEgg(terrain, this, position);
		}
	}
	if (isUpdate)
	{
		if (chickenDieCount < 2)
		{
			chickenDieCount = 0;
			return;
		}	
		time += DELTA;
		if (time > breedingTime)
		{
			float x = rand() % 200 + 50;
			float z = rand() % 200 + 50;
			Vector3 position = Vector3(x, 0, z);
			obstacles->CreateEgg(terrain, this, position);
			time = 0.0f;
		}
	}
	chickenDieCount = 0;
}

