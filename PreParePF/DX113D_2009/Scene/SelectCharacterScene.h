#pragma once
class SelectCharacterScene : public Scene
{
	Jin* jin;
	Ezreal* ezreal;
	RenderTarget* renderTarget;
	DepthStencil* depthStencil;
	Texture2D* backGroundImage;
	JinUI* jinUI;
	EzrealUI* ezrealUI;
	StartButton* startButton;
	Sprite* sprite;
//	FarmingScene* farmingScene;

public:
	SelectCharacterScene();
	~SelectCharacterScene();

	// Scene을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};