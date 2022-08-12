#pragma once
class FemailEnemy : public Monster
{
private:
	int deadTime;
public:
	FemailEnemy(int index);
	~FemailEnemy();

	void Update();
	void Render();
	void PostRender();
	void PreRender();
};