#pragma once
class Ez_E : public Texture2D
{
private:
	
public:
	Ez_E();
	~Ez_E();

	// Texture2D을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

};