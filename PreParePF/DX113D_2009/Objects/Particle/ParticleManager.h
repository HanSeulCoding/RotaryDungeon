#pragma once

class ParticleManager : public Singleton<ParticleManager>
{
private:
	friend class Singleton;

	map<string, vector<Particle*>> totalParticle;

	ParticleManager();
	~ParticleManager();

public:
	void Update();
	void Render();
	void PostRender();

	void Play(string key, Vector3 position);

private:
	void CreateSpark(string key,wstring fileName);
	void CreateSpriteEffect(string key, wstring diffuseFile,
		Float2 maxFrame, UINT poolCount,Float2 size = Float2(20,20));
};