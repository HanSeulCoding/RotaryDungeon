#pragma once
class BossScene : public Scene
{
private:
	Terrain* terrain;
	Fox* fox;
	
	Players* player;
	Boss* boss;
	SkyBox* sky;
	Texture2D* bagImage;
	Texture2D* meatImage;
	Texture2D* inventory;
	Texture2D* chickenImage;
	ObstacleManager* obstacleM;
	aurora* auroras;
	StartSceneBgUI* startSceneBg;
	SkillImage* skillImage;
public:
	BossScene();
	~BossScene();

	// Scene을(를) 통해 상속됨
	virtual void Start() override;
	virtual void End() override;
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	void BossItemRender();
	void PlayerAuroraCollision();
	
};