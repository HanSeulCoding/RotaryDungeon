#include "Framework.h"

GameManager::GameManager()
	:isExcuteInventory(false),killCount(0),playTime(0.0f),isPlayerESkill(false),isBoss(false)
{
}

GameManager::~GameManager()
{
}

void GameManager::SetLevel(int exp)
{
	
}

void GameManager::SaveData()
{
	wstring path = L"GameData/gameData.txt";
	BinaryWriter* bw = new BinaryWriter(path);
	bw->Int(farmingChickenNum);
	
	delete bw;
}

void GameManager::LoadData()
{
	wstring path = L"GameData/gameData.txt";
	BinaryReader* rw = new BinaryReader(path);

	if (rw == nullptr)
		return;

	farmingChickenNum = rw->Int();
	delete rw;
}
