#pragma once
class Boss : public Monster
{
private:
	enum BossAction
	{
		NORMALATTACK=3, JUMPATTACK,DEATH
	};
	BossUI* bossUI;
	BossHpBar* bossHpBar;

	float temp;
	float deadTime;
	float jumpAttackTime;
	float time;
	bool fallingSound;
	bool pattern1;
	bool pattern1Audio;
	bool pattern2;
	bool setJumpAttack;
	bool setNormalAttack;
	bool isAttack;
	bool isChangeHp;
	bool isDead;
public:
	Boss();
	~Boss();

	void Update();
	void Render();
	void PostRender();
	void PreRender();

	void AttackPattern1();
	void AttackPattern2();

	void Pattern1EndAttack();
	void Pattern2EndAttack();
};