#pragma once
class ExpBar : public Texture2D
{
private:
	ExpBuffer* expBuffer;
public :
	ExpBar(Vector3 position, Vector3 scale);
	~ExpBar();

	// Texture2D을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	ExpBuffer* GetExpBuffer() { return expBuffer; }
	void SetExpBuffer(ExpBuffer* value) { expBuffer = value; }
};