#pragma once
class Jin_E : public Texture2D
{
public:
	Jin_E();
	~Jin_E();

	// Texture2D을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
};