#include "Framework.h"
#include "WallManager.h"

WallManager::WallManager()
{
	Wall* wall = new Wall();
	wall->localPosition = Vector3(0, 20, 0);
	wall->localScale = Vector3(80, 80, 40);
	wall->localRotation = Vector3(0, -1.56, 0);
	walls.emplace_back(wall);

	Wall* wall2 = new Wall();
	wall2->localPosition = Vector3(0, 20, 0);
	wall2->localScale = Vector3(80, 80, 40);
	wall2->localRotation = Vector3(0, 0, 0);
	walls.emplace_back(wall2);

	Wall* wall3 = new Wall();
	wall3->localPosition = Vector3(400, 20, 0);
	wall3->localScale = Vector3(80, 80, 40);
	wall3->localRotation = Vector3(0, -1.58, 0);
	walls.emplace_back(wall3);

	Wall* wall4 = new Wall();
	wall4->localPosition = Vector3(300, 20, 390);
	wall4->localScale = Vector3(20, 80, 40);
	wall4->localRotation = Vector3(0, 0, 0);
	walls.emplace_back(wall4);

	Wall* wall5 = new Wall();
	wall5->localPosition = Vector3(0, 20, 390);
	wall5->localScale = Vector3(45, 80, 40);
	wall5->localRotation = Vector3(0, 0, 0);
	walls.emplace_back(wall5);
}

WallManager::~WallManager()
{
	for (auto wall : walls)
		delete wall;
}

void WallManager::Update()
{
}

void WallManager::Render()
{
	for(auto wall : walls)
		wall->Render();
}
