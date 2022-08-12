#pragma once

class Scene
{
public:
	virtual ~Scene() = default;

	virtual void Update() = 0;
	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;
	virtual void End() {};
	virtual void Start(Vector3 pos) {};
	virtual void Start() {};
};