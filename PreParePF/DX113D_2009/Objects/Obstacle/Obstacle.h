#pragma once
class MonsterManager;
class Obstacle : public ModelReader, public Transform
{
protected:
	Collider* collider;
	bool isRenderText;
	bool isActive;
	bool isDelete;
	MonsterManager* monsters;
	Terrain* terrain;
public:
	string tag;
	Obstacle(string file);
	~Obstacle();

	virtual void Update() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;

	bool ColliderObstacle(Collider* targetCollider);
	void PushPlayer();

	bool GetIsActive() { return isActive; }
	void SetIsActive(bool value) { isActive = value; }
	void SetMonsters(MonsterManager* value) { monsters = value; }
	void SetTerrain(Terrain* value) { terrain = value; }
};