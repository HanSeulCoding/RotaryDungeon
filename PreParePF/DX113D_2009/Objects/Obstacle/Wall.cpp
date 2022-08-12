#include "Framework.h"

Wall::Wall()
	:ModelReader("Wall")
{
	SetShader(L"Lighting");
	SetDiffuseMap(L"ModelData/Materials/Obstacle/Dungeons_Texture_WallTiles_02.png");
	SetNormalMap(L"ModelData/Materials/Obstacle/Dungeons_Texture_WallTiles_Normal.png");

}

Wall::~Wall()
{
}

void Wall::Render()
{
	SetWorldBuffer();
	MeshRender();
}

void Wall::Update()
{
}

void Wall::PostRender()
{
}
