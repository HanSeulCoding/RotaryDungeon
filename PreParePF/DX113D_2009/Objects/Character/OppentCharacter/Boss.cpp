#include "Framework.h"

Boss::Boss()
	:Monster("Boss"),pattern1(false),time(5.0f), fallingSound(false)
{
	attribute = "person";
	moveSpeed = 20;
	rotSpeed = 2.0f;
	maxMoveSpeed = 10.0f;
	accelation = 15.0f;
	deceleration = 10.0f;
	attackDamage = 20.0f;
	tag = "boss";
	hp = 300;
	curHp = hp;
	this->index = index;
	localScale *= 0.15f;
	localPosition.x = 190;
	localPosition.z = 180;

	bossHpBar = new BossHpBar();
	bossUI = new BossUI();

	bossHpBar->GetHpBuffer()->data.value = hp;
	SetShader(L"ModelAnimation");

	//SetDiffuseMap(L"ModelData/Materials/Enemy/Dungeons_Texture_01.png");
	ReadClip(0, "Boss/Idle");
	ReadClip(1, "Boss/Walk");
	ReadClip(2, "Boss/Run");
	ReadClip(3, "Boss/RockAttack");
	ReadClip(4, "Boss/JumpAttack3");
	ReadClip(5, "Boss/Death");
	SetEndEvent(JUMPATTACK, bind(&Boss::Pattern1EndAttack, this));
	SetEndEvent(NORMALATTACK, bind(&Boss::Pattern2EndAttack, this));
	
	int count = rand() % 2 + 1;
	switch (count)
	{
	case 1:	ITEMMANAGER->CreateMeatItem("monster", world, index, Vector3(5, 5, 5));
		break;
	case 2:
		ITEMMANAGER->CreateHpPotionItem("monster", world, index, Vector3(10, 10, 10));
		break;
	default:
		break;
	}
	detectCollider->SetParent(&world);
	detectCollider->localScale *= 200;
	detectCollider->UpdateWorld();
	collider->localScale *= 70;
	collider->UpdateWorld();
}

Boss::~Boss()
{
	delete meat;
	delete collider;
	delete detectCollider;
}

void Boss::Update()
{
	if (!isActive)
		return;
	time += DELTA;
	if (!isDead)
	{
		if (time > 3.0f)
		{
			if (!pattern1 && !pattern2)
			{
				int randomPattern = Random(0, 2);

				if (randomPattern == 0)
					pattern1 = true;
				else if (randomPattern == 1)
					pattern2 = true;
			}
		}
		if (pattern1)
			AttackPattern1();

		if (pattern2)
			AttackPattern2();
	}
	if (curHp <= 0)
	{
		if (!isDead)
		{
			SetAnimation(DEATH);
			AUDIO->Play("BossDieSound");
			isDead = true;
			players->ChangeExp(200);
		}
		deadTime += DELTA;
		if (deadTime > 2.1f)
		{
			if (!fallingSound)
			{
				AUDIO->Play("FallingBossSound");
				fallingSound = true;
			}
		}
		if (deadTime > 3.0f)
		{
			isActive = false;
		}
	}
	
	float test = (float)curHp / (float)hp;
	bossHpBar->GetHpBuffer()->data.value = (float)curHp / (float)hp;
	ModelAnimator::Update();

	//AttackToMove();
}

void Boss::Render()
{
	if (!isActive)
		return;
	Monster::Render();
	detectCollider->Render();
	
}

void Boss::PostRender()
{
	if (!isActive)
		return;
	ImGui::InputFloat3("bossPos", (float*)&localPosition);
	ImGui::InputFloat3("bossScale", (float*)&localScale);
	ImGui::InputFloat("temp", (float*)&temp);
	bossUI->Render();
	bossHpBar->Render();
}

void Boss::PreRender()
{
}

void Boss::AttackPattern1()
{
	if(!isAttack)
		TargetToMove(players->localPosition, (distance > 40), 30);
	if (distance < 60)
	{
		isAttack = true;
		isChangeHp = false;
		pattern1Audio = true;

		
		if (!setJumpAttack)
		{
			SetAnimation(JUMPATTACK);
			setJumpAttack = true;
		}
		
	}
	/*if (pattern1Audio)
	{
		jumpAttackTime += DELTA;
		if (jumpAttackTime > 1.5f)
		{
			AUDIO->Play("JumpAttack");
			jumpAttackTime = 0.0f;
		
		}
	}*/
	//SetAnimation(JUMPATTACK);

}

void Boss::AttackPattern2()
{
	localRotation.y = atan2(players->localPosition.x - localPosition.x, players->localPosition.z - localPosition.z) + 3;

	if (!setNormalAttack)
	{
		SetAnimation(NORMALATTACK);
		AUDIO->Play("BossAttackSound");
		isChangeHp = false;
		setNormalAttack = true;

	}

}

void Boss::Pattern1EndAttack()
{
	SetAnimation(IDLE);
	if (pattern1Audio)
	{
		AUDIO->Play("JumpAttack");
		pattern1Audio = false;

	}
	localPosition -= Forward() * 4000 * DELTA;
	pattern1 = false;
	isAttack = false;
	setJumpAttack = false;

	if (!isChangeHp)
	{
		string tag = players->tag;
		players->ChangeHp(false, 30);
		isChangeHp = true;
	}
	time = 0.0f;
}

void Boss::Pattern2EndAttack()
{
	SetAnimation(IDLE);
	pattern2 = false;
	isAttack = false;
	setNormalAttack = false;
	if (!isChangeHp)
	{
		string tag = players->tag;
		players->ChangeHp(false, 30);
		isChangeHp = true;
	}
	time = 0.0f;
}


