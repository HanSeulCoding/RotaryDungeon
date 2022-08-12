#pragma once
class BossHpBar : public Texture2D
{
private:
	HPBuffer* hpBuffer;
public:
	BossHpBar();
	~BossHpBar();

	// Texture2D��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	HPBuffer* GetHpBuffer() { return hpBuffer; }
	void SetHpBuffer(HPBuffer* value) { hpBuffer = value; }
};