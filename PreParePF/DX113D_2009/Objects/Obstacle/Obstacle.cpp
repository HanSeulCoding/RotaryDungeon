#include "Framework.h"

Obstacle::Obstacle(string file)
	:ModelReader(file)
{
	SetShader(L"Lighting");


}

Obstacle::~Obstacle()
{
}

void Obstacle::Update()
{
}

void Obstacle::Render()
{
	if (!isActive)
		return;
	SetWorldBuffer();
	MeshRender();

	//	collider->Render();
}

bool Obstacle::ColliderObstacle(Collider* targetCollider)
{
	if (targetCollider->BoxCollision((BoxCollider*)collider))
	{
		return true;
	}

	return false;
}
