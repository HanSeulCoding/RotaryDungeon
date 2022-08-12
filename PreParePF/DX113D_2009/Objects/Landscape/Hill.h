#pragma once
class Hill : public ModelReader, public Transform
{
public:
	Hill(Vector3 pos,Vector3 scale, Vector3 rot);
	~Hill();

	void Render();
	void PostRender();
};