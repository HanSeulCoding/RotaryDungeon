#pragma once
class MonsterManager
{
private:
	vector<Monster*> monsters;
	ObstacleManager* obstacles;
	Terrain* terrain;
	Frustum* frustum;
	Obstacle* egg;
	int monsterRespawnNum;
	float respawnTime;
	float chickenDieCount;
	int stageNum;
	int count;
	
	float time;
	int breedingTime;

public:
	MonsterManager(Terrain* terrain);
	~MonsterManager();

	void Update();
	void Render();
	void PostRender();
	void PreRender();

	void CreateChicken(int POOL_COUNT);
	void CreateIndexChicken(int index,Vector3 position);
	void CreateFemailEnemy(int POOL_COUNT);
	vector<Monster*> GetMonsters() { return monsters; }

	void SetObstacles(ObstacleManager* value) { obstacles = value; }
	void SetStageNum(int value) { stageNum = value; }
	void Respawn();
	void Breeding(bool isUpdate);
};