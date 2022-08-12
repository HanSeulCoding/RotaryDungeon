#include "Framework.h"

ParticleManager::ParticleManager()
{
	CreateSpark("blood",L"Textures/Particle/Blood.png");
	CreateSpriteEffect("attack", L"Textures/UI/AttackEffect.png",
		Float2(1, 5), 30);
	CreateSpriteEffect("aurora", L"Textures/UI/aurora.png", Float2(5, 4),30);
	CreateSpriteEffect("lightning", L"Textures/Particle/lightning.png", Float2(2, 3), 30,Float2(8,8));
	CreateSpriteEffect("visionEffect", L"Textures/Particle/VisionEffect.png", Float2(1, 6), 30, Float2(15, 15));
	
}

ParticleManager::~ParticleManager()
{
	for (auto particles : totalParticle)
	{
		for (Particle* particle : particles.second)
		{
			delete particle;
		}
	}
}

void ParticleManager::Update()
{
	for (auto particles : totalParticle)
	{
		for (Particle* particle : particles.second)
		{
			particle->Update();
		}
	}
}

void ParticleManager::Render()
{
	for (auto particles : totalParticle)
	{
		for (Particle* particle : particles.second)
		{
			particle->Render();
		}
	}
}

void ParticleManager::PostRender()
{
	for (auto particles : totalParticle)
	{
		for (Particle* particle : particles.second)
		{
			particle->PostRender();
		}
	}
}

void ParticleManager::Play(string key, Vector3 position)
{
	if (totalParticle.count(key) == 0)
		return;

	for (Particle* particle : totalParticle[key])
	{
		if (!particle->isActive)
		{
			particle->Play(position);
			return;
		}
	}
}

void ParticleManager::CreateSpark(string key, wstring fileName)
{
	UINT poolCount = 50;

	vector<Particle*> particles;

	for (UINT i = 0; i < poolCount; i++)
	{
		particles.emplace_back(new Spark(fileName));
	}

	totalParticle[key] = particles;
}

void ParticleManager::CreateSpriteEffect(string key, wstring diffuseFile, Float2 maxFrame, UINT poolCount, Float2 size)
{	
	vector<Particle*> particles;

	for (UINT i = 0; i < poolCount; i++)
	{
		particles.emplace_back(new SpriteEffect(diffuseFile, maxFrame,size));
	}

	totalParticle[key] = particles;
}
