#pragma once

class Spark : public Particle
{
private:
	struct VertexParticle
	{
		Float3 position;
		Float2 size;
		Float3 velocity;

		VertexParticle() : position(0, 0, 0), size(0, 0), velocity(0, 0, 0)
		{}
	};
	
	SparkBuffer* sparkBuffer;
	VertexParticle* vertices;

	float minRadius;
	float maxRadius;

	float minSize;
	float maxSize;

	ColorBuffer* startColor;
	ColorBuffer* endColor;
public:
	Spark(wstring fileName);
	~Spark();

	virtual void Create() override;
	virtual void Update() override;
	virtual void Render() override;
	virtual void PostRender() override;

	virtual void Play(Vector3 position) override;
	void SetSparkBuffer(SparkBuffer* value) { sparkBuffer = value; }
	SparkBuffer* GetSparkBuffer() { return sparkBuffer; }
	void UpdateParticle();
};