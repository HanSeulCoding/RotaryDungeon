#pragma once
#pragma once
class Monster;
class MonsterManager;
class Boss;
class Players : public ModelAnimator, public Transform
{
protected:
	
	struct Level
	{
		int levelExp = 20;
		int level = 1;
		int attackDamage = 10;
		int hp;
		int exp;
	}level;

	enum CommonState
	{
		IDLE, RUN, ATTACK
	}commonState;

	int key;
	float maxMoveSpeed;
	float beforeMoveSpeed;
	float moveSpeed;
	float rotSpeed;

	Vector3 velocity;
	Vector3 direction;
	float accelation;
	float deceleration;

	bool isAttack;
	bool isControl;
	bool isCollision;
	bool isPress;
	bool isSkill;
	bool isInvincible;

	float jumpPower;
	float gravity;
	float distance;
	bool isJump;
	float angle;
	float ESkillTime;

	Terrain* terrain;
	vector<Monster*> monsters;
	vector<Obstacle*> obstacles;
	Vector3 lightDirection;
	Collider* collider;
	
	Meat* meat;
	ExpBar* expBar;
	HpBar* hpBar;
	MeatImage* meatImage;
	Inventory* inventory;
	MonsterManager* monsterM;
	Vector3 capturedChickenP;
	Boss* boss;
	int count;
	int attackCount;
	int chickenCount;
	bool isDrop;
	bool isLightningMove;

	int hp;
	int curHp;
	int exp;
	int i;
	int addDamage;
	int stageNum;
	bool isGetChicken;
	Players* player;

public:
	Players(string file);
	~Players();

	Collider* ghostCollider;
	string tag;
	//virtual void Update() = 0;
	virtual void PostRender() = 0;
	virtual void Skill() = 0;
	//virtual void test();
	void Control();
	void Move();

	bool ColliderCharacter(Collider* collider);
	void Attack(Collider* collider);
	void EndAttack();
	
	void TargetToMove(Vector3 targetPosition, bool judge,int moveSpeed = 10);


	Collider* GetCollider() { return collider; }
	void SetTerrain(Terrain* value) { terrain = value; }

	int GetCount() { return count; }
	void SetCount(int value) { count = value; }

	bool IsDrop() { return isDrop; }
	void GetBoolDrop(bool value) { isDrop = value; }

	void SetInventory(Inventory* value) { inventory = value; }

	void SetObstalce(vector<Obstacle*> value) { obstacles = value; }

	void SetAnimation(int value);
	void SetMonsterManager(vector<Monster*> value) { monsters = value; }

	void SetMonsterM(MonsterManager* value) { monsterM = value; }
	void SetStageNum(int value) { stageNum = value; }
	void SetIsPress(bool value) { isPress = value; }

	void ChangeHp(bool inc,int curHp);
	void ChangeExp(int exp) {
		this->exp += exp;
	}

	void SetLevel();
	void KillMonster(OUT ExpBar* expBar);
	void GettingItem();
	void SettingItem(Players* player, Players* subPlayer);
	void FeedMeat();

	void DetectedPlayer();
	void SaveData(wstring tag);
	void LoadData(wstring tag);
	void GetChicken();
	void GhostColliderMove();

	void ColliderObstacle();
	void SetPlayer(Players* value) { player = value; }

	void SetBoss(Boss* value) { boss = value; }


};