#pragma once
class HpBar : public Texture2D
{
	HPBuffer* hpBuffer;
public:
	HpBar(Vector3 position, Vector3 scale);
	~HpBar();

	// Texture2D��(��) ���� ��ӵ�
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;

	HPBuffer* GetHpBuffer() { return hpBuffer; }
	void SetHpBuffer(HPBuffer* value) { hpBuffer = value; }

};