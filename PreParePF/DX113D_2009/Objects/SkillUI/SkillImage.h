#pragma once
class SkillImage
{
	Ez_E* ezreal_E;
	Jin_E* jin_E;

public:
	SkillImage();
	~SkillImage();

	void Render();
	void Update();
	void PostRender();


};