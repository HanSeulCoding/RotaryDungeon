#pragma once
class FarmingScene : public Scene
{
	Grass* grass;
	Hill* hill;
	vector<Hill*> hills;
	Terrain* terrain;
	Players* player;
	Fox* fox;
	MonsterManager* monsterManager;
	Jin* jin;
	SkyBox* sky;
	Texture2D* bagImage;
	Texture2D* meatImage;
	Texture2D* inventory;
	WallManager* walls;
	Texture2D* chickenImage;
	ObstacleManager* obstacleM;
	aurora* auroras;
	SkillImage* skillImage;

	StartSceneBgUI* startSceneBg;

	float time;
	float chickenSize;
public:
	FarmingScene();
	~FarmingScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void Start() override;
	virtual void End() override;

	void CreateHill();
	void MonsterItemRender();
	void PlayerAuroraCollision();
	void ChangeScene();
	
};