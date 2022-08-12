#pragma once
class ChickenImage : public Texture2D
{
public:
	ChickenImage();
	~ChickenImage();

	// Texture2D을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};