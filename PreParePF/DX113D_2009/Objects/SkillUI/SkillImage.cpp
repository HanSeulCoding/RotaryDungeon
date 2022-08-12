#include "Framework.h"

SkillImage::SkillImage()
{
	ezreal_E = new Ez_E();
	jin_E = new Jin_E();
}

SkillImage::~SkillImage()
{
	delete ezreal_E;
	delete jin_E;
}

void SkillImage::Render()
{
	if(GMANAGER->playerTag == "ezreal")
		ezreal_E->Render();
	if (GMANAGER->playerTag == "jin")
		jin_E->Render();
}

void SkillImage::Update()
{
	if (GMANAGER->playerTag == "ezreal")
		ezreal_E->Update();
	if (GMANAGER->playerTag == "jin")
		jin_E->Update();
}

void SkillImage::PostRender()
{

}
