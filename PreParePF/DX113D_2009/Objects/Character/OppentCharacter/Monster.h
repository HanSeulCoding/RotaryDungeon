#pragma once
class Monster  : public ModelAnimator, public Transform
{
protected:

	enum CommonState {
		IDLE, WALK, RUN,ATTACK , DEAD
	};
	int key;
	float moveSpeed;
	float backUpMoveSpeed;
	float rotSpeed;
	float maxMoveSpeed;
	float accelation;
	float deceleration;
	float damageTime;
	float time;
	float attackTime;
	float distance;
	float angle;
	float attackDamage;
	float deadTime;

	Vector3 velocity;
	Vector3 direction;
	Vector3 damagedPosition;

	vector<Obstacle*> obstacles;
	Terrain* terrain;
	Item* meat;
	Collider* collider;
	Collider* detectCollider;
	Players* players;

	bool isDamage;
	bool isCollision;
	bool isDie;
	bool isAttack;
	bool isHit;
	bool isGetChicken;

	int hp;
	int curHp;
	int exp;
	int attackedDamage;
	int count;
	int dieCount;
	int index;
	int randomNum;
	int stageNum;

public:
	string tag;
	string attribute;
	Monster(string fileName);
	~Monster();
	void Update();
	void Render();
	void PreRender();

	void AttackToMove();
	void Attack();
	void EndAttack();
	void TargetToMove(Vector3 targetPosition, bool judge, int moveSpeed = 10);
	void IsDie();
	void SettingItem();
	void Control();
	void Move();


	void SetTerrain(Terrain* value) { terrain = value; }
	void SetDamage(int damage);
	void SetIsCollision(bool value) { isCollision = value; }
	void SetAnimation(int state);
	void SetIsDie(bool value) { isDie = value; }
	void SetObstalce(vector<Obstacle*> value) { obstacles = value; }
	void SetIsDamage(bool value) { isDamage = value; }
	void SetIsGetChicken(bool value) { isGetChicken = value; }
	void SetDetectCollider(Collider* value) { detectCollider = value; }
	void SetIsAttack(bool value) { isAttack = value; }
	void SetHp(int value) { hp = value; }
	void SetCurHp(int value) { curHp = value; }
	void SetDieCount(int value) { dieCount = value; }
	void SetPlayer(Players* value) { players = value; }
	void SetStageNum(int value) { stageNum = value; }

	void GetAttackDamage(int value) { attackedDamage = value; }
	bool GetIsDamage() { return isDamage; }
	bool GetIsActive() { return isActive; }
	bool GetIsDie() { return isDie; }
	bool GetIsCollision() { return isCollision; }
	Collider* GetCollider() { return collider; }
	Item* GetMeat() { return meat; }
	Collider* GetDetectCollider() { return detectCollider; }
	int GetHp() { return hp; }
	int GetCurHp() { return curHp; }
	int GetIndex() { return index; }


};