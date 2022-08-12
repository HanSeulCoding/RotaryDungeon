#pragma once
class ObstacleManager 
{
private:


public:
	vector<Obstacle*> obstacles;

	ObstacleManager();
	~ObstacleManager();

	void Update();
	void Render();
	void PostRender();
	void CreateEve(Vector3 pos = Vector3(0,0,0));
	void CreateEgg(Terrain* terrain, MonsterManager* monster, Vector3 position = { 0,0,0 });
	void CreateStatue(Vector3 position, Vector3 rotation, Vector3 scale = { 12.9,12.9,12.9 });
};