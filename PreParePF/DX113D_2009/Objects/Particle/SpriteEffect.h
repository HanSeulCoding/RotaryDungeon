#pragma once

class SpriteEffect : public Particle
{
private:
	SpriteBuffer* spriteBuffer;
	VertexSize* vertices;

	float time;
	float speed;
	UINT curFrameCount;
	UINT maxFrameCount;

	Float2 size;

public:
	SpriteEffect(wstring diffuseFile, Float2 maxFrame, Float2 size);
	~SpriteEffect();
		
	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	virtual void Play(Vector3 position) override;
};