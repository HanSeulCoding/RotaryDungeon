#pragma once
class Weapon;
class Meat;
class GameManager : public Singleton<GameManager>
{
private:
	friend class Singleton;

	int meatCount;
	bool isExcuteInventory;
	bool isGetItem;
	bool isPlayerESkill;
	bool isBoss;
	int meatsCount;

	int farmingChickenNum;


	GameManager();
	~GameManager();
	
public:
	string playerTag;
	float playTime;
	int killCount;
	struct Level
	{
		int attackDamage;
		int hp;
		int exp;
	};
	void SetExcuteInventory(bool value) { isExcuteInventory = value; }
	bool GetExcuteInventory() { return isExcuteInventory; }

	void SetMeats(int value) { meatsCount = value; }
	int GetMeats() { return meatsCount; }

	void SetItem(bool value) { isGetItem = value; }
	bool GetItem() { return isGetItem; }

	void SetLevel(int exp);

	void SetKillCount(int value) { killCount = value; }
	int GetKillCount() { return killCount; }

	void SetPlayTime(float value) { playTime = value; }
	float GetPlayTime() { return playTime; }

	void SetFarmingChickenNum(int value) { farmingChickenNum = value; }
	int GetFarmingChickenNum() { return farmingChickenNum; }

	void SetIsPlayerESkill(bool value) { isPlayerESkill = value; }
	bool GetIsPlayerESkill() { return isPlayerESkill; }

	void SetIsBoss(bool value) { isBoss = value; }
	bool GetIsBoss() { return isBoss; }

	void SaveData();
	void LoadData();
	
};