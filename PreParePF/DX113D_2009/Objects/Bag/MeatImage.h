#pragma once
class MeatImage : public Texture2D
{
private:

public:
	MeatImage();
	~MeatImage();

	// Texture2D을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};