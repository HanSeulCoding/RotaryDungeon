#pragma once
class BossUI : public Texture2D
{
public:
	BossUI();
	~BossUI();

	// Texture2D을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};