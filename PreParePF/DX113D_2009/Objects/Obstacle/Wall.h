#pragma once
class Wall : public ModelReader, public Transform
{
private:
	Collider* collider;
public:
	Wall();
	~Wall();

	void Render();
	void Update();
	void PostRender();
};