#include "Framework.h"
#include "ObstacleManager.h"

ObstacleManager::ObstacleManager()
{
}

ObstacleManager::~ObstacleManager()
{
	for (auto obstacle : obstacles)
		delete obstacle;
}

void ObstacleManager::Update()
{
	for (auto obstacle : obstacles)
		obstacle->Update();
	for (int i = 0; i < obstacles.size(); i++) //egg가 많아졌을때 obstacles 벡터가 너무 커질거 같아 수시로 지워준다.
	{
		if (!obstacles[i]->GetIsActive())
		{
			obstacles.erase(obstacles.begin() + i);
		}
	}
	
	
}

void ObstacleManager::Render()
{
	for (auto obstacle : obstacles)
		obstacle->Render();
}

void ObstacleManager::PostRender()
{
	for (auto obstacle : obstacles)
		obstacle->PostRender();
}

void ObstacleManager::CreateEve(Vector3 pos)
{
	Obstacle* eve = new Eve();
	if (pos.x != 0)
	{
		eve->localPosition = pos;
		eve->localRotation.y -= XM_PI*0.5f;
	}
	obstacles.emplace_back(eve);
}

void ObstacleManager::CreateEgg(Terrain* terrain, MonsterManager* monster, Vector3 position)
{
	Obstacle* egg = new Egg();
	egg->localPosition = position;
	egg->SetMonsters(monster);
	egg->SetTerrain(terrain);
	obstacles.emplace_back(egg);
}

void ObstacleManager::CreateStatue(Vector3 position, Vector3 rotation, Vector3 scale)
{
	Obstacle* statue = new Statue();
	statue->localPosition = position;
	statue->localScale = scale;
	statue->localRotation = rotation;
	obstacles.emplace_back(statue);
}

