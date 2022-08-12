#pragma once
class GrowingGameScene : public Scene
{
private:
	Terrain* terrain;
	Fox* fox;
	MonsterManager* monsterManager;
	Players* player;
	SkyBox* sky;
	Texture2D* bagImage;
	Texture2D* meatImage;
	Texture2D* inventory;
	WallManager* walls;
	Texture2D* chickenImage;
	ObstacleManager* obstacleM;
	aurora* auroras;
	StartSceneBgUI* startSceneBg;
	SkillImage* skillImage;
	float loadingTime;
	bool isLoading;

public:
	GrowingGameScene();
	~GrowingGameScene();

	// Scene��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	virtual void Start() override;
	virtual void End() override;
	void MonsterItemRender();
	void PlayerAuroraCollision();
};