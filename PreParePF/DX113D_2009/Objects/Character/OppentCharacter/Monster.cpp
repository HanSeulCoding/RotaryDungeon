#include "Framework.h"

Monster::Monster(string fileName)
	:ModelAnimator(fileName), randomNum(100), isCollision(false),isHit(false)
{
	

	detectCollider = new CapsuleCollider();

	collider = new CapsuleCollider();
	collider->SetParent(&world);

	collider->UpdateWorld();
	UpdateWorld();
	SetEndEvent(ATTACK, bind(&Monster::EndAttack, this));
	FONT->Add("damage", L"a Attack Graffiti", Float3(1, 1, 1), 20);
	
	
}

Monster::~Monster()
{
	delete collider;
}

void Monster::Control()
{
	if (dieCount != 0)
		return;

	time += DELTA;
	if (time > 2.0f)
	{
		time = 0.0f;
		randomNum = rand() % 3 + 0;
	}
	if (randomNum == 0)
	{
		if (velocity.Length() < maxMoveSpeed)
			velocity -= Forward() * accelation * DELTA;
	}

	/*if (randomNum == 1)
	{
		if (velocity.Length() < maxMoveSpeed)
			velocity += Forward() * accelation * DELTA;
	}*/

	if (randomNum == 1)
	{
		localRotation.y += rotSpeed * DELTA;
	}

	if (randomNum == 2)
	{
		localRotation.y -= rotSpeed * DELTA;
	}
}

void Monster::Move()
{
	if (dieCount != 0)
		return;
	localPosition.y = terrain->GetHeight(localPosition);
	if (velocity.Length() > 0.0f)
	{
		Vector3 zero;
		velocity = LERP(velocity, zero, deceleration * DELTA);
	}
	if (isDamage)
	{
		backUpMoveSpeed = moveSpeed;
		moveSpeed = 50;
	}
	else if (!isDamage)
		moveSpeed = backUpMoveSpeed;

	localPosition += velocity * 20 * DELTA;

	if (velocity.Length() > 0.5f)
	{
		SetAnimation(WALK);
	}
	else
	{
		SetAnimation(IDLE);
	}
	if (stageNum != 2)
	{
		if (localPosition.z > 380)
		{
			localPosition.z = 380;
		}
		if (localPosition.z < 10)
		{
			localPosition.z = 10;
		}
		if (localPosition.x < 40)
			localPosition.x = 40;
		if (localPosition.x > 370)
			localPosition.x = 370;
	}
	if (stageNum == 2)
	{
		if (localPosition.z > 281)
		{
			localPosition.z = 281;
		}
		if (localPosition.z < 45)
		{
			localPosition.z = 45;
		}
		if (localPosition.x < 94)
			localPosition.x = 94;
		if (localPosition.x > 295)
			localPosition.x = 295;
	}
	if (stageNum == 3)
	{
		if (localPosition.z > 291)
		{
			localPosition.z = 291;
		}
		if (localPosition.z < 0)
		{
			localPosition.z = 0;
		}
		if (localPosition.x < 0)
			localPosition.x = 0;
		if (localPosition.x > 295)
			localPosition.x = 295;

	}
}

void Monster::Update()
{
	if (!isActive)
		return;
	if(!isAttack)
	{
		Control();
		Move();
	}
	IsDie();
	ModelAnimator::Update();

}

void Monster::Render()
{
	collider->Render();

	if (!isActive)
	{
		return;
	}
	SetWorldBuffer();
	ModelAnimator::Render();

	if (attackedDamage != 0)
	{
		wstring s;
		s = to_wstring(attackedDamage);
		Float2 pos;
	//	WorldToScreen(damagedPosition);
		pos.x = localPosition.x;
		pos.y = localPosition.y+5;

		Float2 size = { 90,100 };
		FONT->RenderText(s, "damage", pos, size);
	}
}

void Monster::PreRender()
{
}

void Monster::SetDamage(int damage)
{
	if(GMANAGER->playerTag == "ezreal")
		AUDIO->Play("damageSound");

	curHp -= damage;
	isDamage = true;

	localPosition.z += 5;
	ParticleManager::Get()->Play("blood", Vector3(localPosition.x, localPosition.y + 10, localPosition.z));
	ParticleManager::Get()->Play("attack", Vector3(localPosition.x + 3, localPosition.y+8, localPosition.z));
}

void Monster::SetAnimation(int state)
{
	if (key != state)
	{
		key = state;
		PlayClip(key);
	}
}

void Monster::AttackToMove()
{
	if (isAttack)
	{
		if (collider->Collision((CapsuleCollider*)players->GetCollider()))
		{
			Attack();
		}
		else
		{
			TargetToMove(players->localPosition, distance > 0.2f, 20);
		}
	}
}

void Monster::Attack()
{
	attackTime += DELTA;
	if (attackTime > 2.2f)
	{
		attackTime = 0.0f;
		isHit = true;
		
	} 
	if (dieCount == 0)
	{
		SetAnimation(ATTACK);
	}
}

void Monster::EndAttack()
{
	if(isHit)
		players->ChangeHp(false, attackDamage);
	isHit = false;
}



void Monster::TargetToMove(Vector3 targetPosition, bool judge, int moveSpeed)
{
	if(dieCount == 0)
		SetAnimation(RUN);

	Vector3 targetToMoveP;

	targetToMoveP.x = targetPosition.x;
	targetToMoveP.z = targetPosition.z;


	direction = targetToMoveP - localPosition;
	direction.y = 0;

	distance = direction.Length();
	velocity = direction.Normalized();
	

	localRotation.y = atan2(targetToMoveP.x - localPosition.x, targetToMoveP.z - localPosition.z) + 3;
	
	if (judge)
	{
		localPosition.x += velocity.x * moveSpeed * DELTA;
		localPosition.z += velocity.z * moveSpeed * DELTA;
		localPosition.y = terrain->GetHeight(localPosition);
	}

}

void Monster::IsDie()
{
	if (curHp <= 0)
	{
		deadTime += DELTA;
		if (dieCount == 0)
		{
			SetAnimation(DEAD);
			dieCount++;
		}
			
		
		if (deadTime > 1.5f)
		{
			isDie = true;
			isActive = false;
			int count = GMANAGER->killCount;
			GMANAGER->killCount += 1;
			deadTime = 0.0f;
		}
	}
}

void Monster::SettingItem()
{
	for (int i = 0; i < ITEMMANAGER->GetItems()["monster"].size(); i++)
	{
		int size = ITEMMANAGER->GetItems().size();
		int hoho = ITEMMANAGER->GetItems()["monster"][i]->GetIndex();
		if (index == ITEMMANAGER->GetItems()["monster"][i]->GetIndex())
		{
			ITEMMANAGER->GetItems()["monster"].erase(ITEMMANAGER->GetItems()["monster"].begin() + i);
		}
	}

	int count = rand() % 2 + 1;
	switch (count)
	{
	case 1:	ITEMMANAGER->CreateMeatItem("monster", world, index);
		break;
	case 2:
		ITEMMANAGER->CreateHpPotionItem("monster", world, index, Vector3(10, 10, 10));
		break;
	default:
		break;
	}
}

