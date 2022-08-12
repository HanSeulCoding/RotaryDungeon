
#include "Framework.h"

Players::Players(string file)
	:ModelAnimator(file),maxMoveSpeed(10.0f), moveSpeed(10.0f), rotSpeed(2.0f),
	commonState(IDLE), accelation(30.0f), deceleration(10.0f), isAttack(false), isControl(true)
	, jumpPower(0), gravity(98.0f), isJump(false), isSkill(false),isInvincible(false)
{
	SetShader(L"Lighting");
	beforeMoveSpeed = moveSpeed;
	
	FONT->Add("playerE", L"Basic", Float3(1, 1, 1), 20);
}

Players::~Players()
{
}



void Players::Control()
{
	if (isAttack)
		return;

	if (KEY_PRESS('W'))
	{
		if (velocity.Length() < maxMoveSpeed)
			velocity -= Forward() * accelation * DELTA;
	}

	if (KEY_PRESS('S'))
	{
		if (velocity.Length() < maxMoveSpeed)
			velocity += Forward() * accelation * DELTA;
	}

	if (KEY_PRESS('D'))
	{
		localRotation.y += rotSpeed * DELTA;
	}

	if (KEY_PRESS('A'))
	{
		localRotation.y -= rotSpeed * DELTA;
	}


}




void Players::Move()
{
	if (isAttack || isSkill)
		return;

	if (velocity.Length() > 0.0f)
	{
		Vector3 zero;
		velocity = LERP(velocity, zero, deceleration * DELTA);
	}

	localPosition += velocity * moveSpeed * DELTA;

	if (velocity.Length() > 0.5f)
	{
		SetAnimation(RUN);
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
		if (localPosition.x < 40)
			localPosition.x = 40;
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

bool Players::ColliderCharacter(Collider* collider)
{
	if (this->collider->Collision((BoxCollider*)collider))
	{
		return true;
	}
	return false;
}

void Players::Attack(Collider* collider)
{
	if (isSkill)
		return;

	isAttack = true;
	SetAnimation(ATTACK);
	AUDIO->Play("jinAttackSound");
	if (!GMANAGER->GetIsBoss())
	{
		for (auto monster : monsters)
		{
			if (collider->Collision((CapsuleCollider*)monster->GetCollider()))
			{
				if (!monster->isActive)
					continue;
				if (monster->attribute == tag) //태그(속성) 같으면 추가 데미지
					addDamage = 50;
				monster->SetDamage(level.attackDamage + addDamage); //monster한테 데미지 준다.
				monster->SetColor(Float4(1, 0, 0, 1)); //몬스터 색깔 바꿔준다.
				monster->GetAttackDamage(level.attackDamage + addDamage);

				addDamage = 0;
			}
		}
	}
	if (GMANAGER->GetIsBoss())
	{
		if (collider->Collision((CapsuleCollider*)boss->GetCollider()))
		{
			if (boss->attribute == tag) //태그(속성) 같으면 추가 데미지
				addDamage = 50;
			boss->SetDamage(level.attackDamage + addDamage); //monster한테 데미지 준다.
			boss->SetColor(Float4(1, 0, 0, 1)); //몬스터 색깔 바꿔준다.
			boss->GetAttackDamage(level.attackDamage + addDamage);
			addDamage = 0;
		}
	}
}




void Players::EndAttack()
{
	isAttack = false;
	SetAnimation(IDLE);
	attackCount = 0;
	if (!GMANAGER->GetIsBoss())
	{
		for (auto monster : monsters)
		{
			if (monster->GetIsDamage())
			{
				if (!monster->isActive)
					continue;
				monster->SetColor(Float4(1, 1, 1, 1));
				monster->SetIsDamage(false);
				monster->GetAttackDamage(0);
			}
		}
	}
	if (GMANAGER->GetIsBoss())
	{
		boss->SetColor(Float4(1, 1, 1, 1));
		boss->SetIsDamage(false);
		boss->GetAttackDamage(0);
	}
}


void Players::TargetToMove(Vector3 targetPosition, bool judge, int moveSpeed)
{
	Vector3 targetToMoveP;

	targetToMoveP.x = targetPosition.x - 10;
	targetToMoveP.z = targetPosition.z;


	direction = targetToMoveP - localPosition; //방향 값
	direction.y = 0;

	distance = direction.Length();
	velocity = direction.Normalized();
	angle = atan2(-direction.z, direction.x); //각도 값


	if (judge)
	{
		localRotation.y = angle - XM_PI * 0.5f;
		localPosition.x += velocity.x * moveSpeed * DELTA;
		localPosition.z += velocity.z * moveSpeed * DELTA;
	}
}

void Players::SetAnimation(int value)
{
	if (key != value)
	{
		key = value;
		PlayClip(key);
	}
}

void Players::ChangeHp(bool inc, int curHp)
{
	if (isInvincible)
	{
		return;
	}

	if (inc)
	{
		this->curHp += curHp;
	}
	else
	{
		ParticleManager::Get()->Play("blood", Vector3(localPosition.x, localPosition.y + 10, localPosition.z));
		AUDIO->Play("DamagedPlayer");
		this->curHp -= curHp;
	}

	if (this->curHp > hp)
		this->curHp = hp;
	else if (this->curHp < 0)
		this->curHp = 0;

	if (!inc)
	{
		CAMERA->ShakeStart(3, 0.5, 3);
	}

}

void Players::SetLevel()
{
	
	if (exp >= level.levelExp)
	{
		i++;
		level.levelExp = 20 + i * 30;
		level.level = i + 1;
		level.attackDamage = 20+3*i;
		level.hp = 50 + 3 * i;
		hp += level.hp;
		curHp = hp;
		exp = 0;
		hpBar->GetHpBuffer()->data.value = (float)curHp / hp;
		expBar->GetExpBuffer()->data.value = (float)exp / level.levelExp;
			
	}
	
	

}

void Players::KillMonster(OUT ExpBar* expBar)
{
	for (auto monster : monsters)
	{
		if (monster->GetIsDie())
		{
			exp += 5;
			expBar->GetExpBuffer()->data.value = (float)exp / level.levelExp;
			monster->SetIsDie(false);
			break;
		}
	}
}

void Players::GettingItem()
{
	int hanseul = ITEMMANAGER->GetItems()["player"].size();
	if (ITEMMANAGER->GetItems()["monster"].size() <= 0)
		return;
	for (int i = 0; i < ITEMMANAGER->GetItems()["monster"].size(); i++)
	{
		//Item* item = *iter;
		Item* item = ITEMMANAGER->GetItems()["monster"][i];
		if (ColliderCharacter(item->GetCollider()))
		{
			if (KEY_DOWN('R')) // R 키 누르면 고기 얻기 
			{
				if (item->tag == "meat")
				{
					meat = new Meat;
					meat->localScale *= 0.3;
					ITEMMANAGER->CreateItem("player", meat);
					ITEMMANAGER->GetItems()["monster"].erase(ITEMMANAGER->GetItems()["monster"].begin() + (i));
					meatImage = new MeatImage();
					inventory->CreateItem("meat", meatImage);
				}
				if (item->tag == "hpPotion")
				{
					ITEMMANAGER->CreateItem("player", item);
					ITEMMANAGER->GetItems()["monster"].erase(ITEMMANAGER->GetItems()["monster"].begin() + (i));
					HpPotionImage* hpPotionImage = new HpPotionImage();
					inventory->CreateItem("hpPotion", hpPotionImage);
				}

			}
		}
	}
}

void Players::SettingItem(Players* player,Players* subPlayer)
{
	int hanseul = ITEMMANAGER->GetItems()["player"].size();
	if (inventory->GetIsClick()) //Inventory에서 아이템을 클릭했을경우
	{

			if (inventory->GetItemTag() == "meat")
			{
				int a = inventory->GetItems()["meat"]->GetClickCount();
				
				if (player->tag != "animal")
					FeedMeat();
				
				inventory->GetItems()["meat"]->SetClickCount(0);
			}
			
			if (inventory->GetItemTag() == "hpPotion")
			{
				for (int i = 0; i < inventory->GetItems()["hpPotion"]->GetClickCount(); i++) //인벤토리 누른 횟수만큼 
				{
					for (int i = 0; i < ITEMMANAGER->GetItems()["player"].size();i++)   //이미지아이템이랑 진짜 아이템이랑
					{
						if (ITEMMANAGER->GetItems()["player"][i]->tag == inventory->GetItems()["hpPotion"]->key)         //키값 태그값 비교해서 아이템 기능 
						{
							ITEMMANAGER->GetItems()["player"][i]->Function(player);
							ITEMMANAGER->GetItems()["player"][i]->Function(this);						// 실행
							ITEMMANAGER->GetItems()["player"].erase(ITEMMANAGER->GetItems()["player"].begin() + i);
							hpBar->GetHpBuffer()->data.value = curHp / hp;
							break;

						}

					}
				}
				inventory->GetItems()["hpPotion"]->SetClickCount(0);
			}
			if (inventory->GetItemTag() == "chicken")
			{
				for (int i = 0; i < inventory->GetItems()["chicken"]->GetClickCount(); i++) //인벤토리 누른 횟수만큼 
				{
					monsterM->CreateIndexChicken(i, Vector3(localPosition.x + 10, localPosition.y, localPosition.z + 10));
				}
			}
			inventory->GetItems()["chicken"]->SetClickCount(0);
		
		}
		inventory->SetIsClick(false);
}

void Players::FeedMeat()
{
	int size = ITEMMANAGER->GetItems()["player"].size();
	for (int i = 0; i < inventory->GetItems()["meat"]->GetClickCount(); i++)
	{
		int hanseul = inventory->GetItems()["meat"]->GetClickCount();
		vector<Item*> item = ITEMMANAGER->GetItems()["player"];
		for (int j = 0; j < ITEMMANAGER->GetItems()["player"].size(); j++)
		{
			Item* item = ITEMMANAGER->GetItems()["player"][j];
			if (item->tag == "meat" && !item->GetIsCheck())
			{
				if (count == inventory->GetItems()["meat"]->GetClickCount())
				{
					count = 0;
					isDrop = true;
					return;
				}
				ITEMMANAGER->GetItems()["player"][j]->localPosition.x = localPosition.x + (count + 1) * 5;
				ITEMMANAGER->GetItems()["player"][j]->localPosition.z = localPosition.z + (count + 1) * 5;
				ITEMMANAGER->GetItems()["player"][j]->localPosition.y = terrain->GetHeight(ITEMMANAGER->GetItems()["player"][j]->localPosition);
				Vector3 position = ITEMMANAGER->GetItems()["player"][j]->localPosition;
				ITEMMANAGER->GetItems()["player"][j]->SetIsActive(true);
				ITEMMANAGER->GetItems()["player"][j]->SetIsCheck(true);
				count++;
			}
		}
	}

	count = 0;
	isDrop = true;
}

void Players::DetectedPlayer()
{
	for (auto monster : monsters)
	{
		if (monster->GetDetectCollider()->CapsuleCollision((CapsuleCollider*)collider))
		{
			monster->SetIsAttack(true);
			monster->SetPlayer(this);
		}
		else
			monster->SetIsAttack(false);
	}
}

void Players::SaveData(wstring tag)
{
	wstring path = L"GameData/" + tag + L"Data.txt";
	BinaryWriter* bw = new BinaryWriter(path);
	bw->Int(curHp); //현재 hp
	bw->Int(hp); //hp
	bw->Int(level.level); // level
	bw->Int(exp); //경험치
	bw->Int(level.levelExp);
	bw->Int(i);
	bw->Int(level.attackDamage);

	delete bw;
}

void Players::LoadData(wstring tag)
{
	
	wstring path = L"GameData/" + tag + L"Data.txt";
	BinaryReader* rw = new BinaryReader(path);

	if (rw == nullptr)
		return;

	curHp = rw->Int();
	if (curHp < 0)
	{
		curHp = hp;
		return;
	}
	hp = rw->Int();
	level.level = rw->Int();
	exp = rw->Int();
	level.levelExp = rw->Int();
	i = rw->Int();
	level.attackDamage = rw->Int();

	delete rw;
}

void Players::GetChicken()
{
	for (auto monster : monsters)
	{
		if (monster->tag == "chicken")
		{
			if (monster->GetCollider()->CapsuleCollision((CapsuleCollider*)collider))
			{
				if (monster->GetCurHp() <= monster->GetHp() / 3 && KEY_DOWN('G'))
				{
					monster->isActive = false;
					Item* chickenItem = new ChickenItem();
					ITEMMANAGER->CreateItem("player", chickenItem);
					ChickenImage* chickenImage = new ChickenImage();
					inventory->CreateItem("chicken", chickenImage);

				}
				if (monster->GetCurHp() <= monster->GetHp() / 3)
				{
					monster->SetIsGetChicken(true);
				}
			}
			else
				monster->SetIsGetChicken(false);
			
		}
	}
}

void Players::GhostColliderMove()
{
	ghostCollider->localPosition += velocity * beforeMoveSpeed * DELTA;
}

void Players::ColliderObstacle()
{
	if (isPress)
	{
		for (auto obstacle : obstacles)
		{
			if (obstacle->ColliderObstacle(collider))
			{
				if (obstacle->localPosition.x > localPosition.x)
				{
					moveSpeed = 0;
					if (!obstacle->ColliderObstacle(ghostCollider))
					{
						moveSpeed = beforeMoveSpeed;
						ghostCollider->localPosition = localPosition;
						break;
					}
				}

				if (obstacle->localPosition.x < localPosition.x)
				{
					moveSpeed = 0;
					if (!obstacle->ColliderObstacle(ghostCollider))
					{
						moveSpeed = beforeMoveSpeed;
						ghostCollider->localPosition = localPosition;
						break;
					}
				}

				if (obstacle->localPosition.z > localPosition.z)
				{
					moveSpeed = 0;
					if (!obstacle->ColliderObstacle(ghostCollider))
					{
						moveSpeed = beforeMoveSpeed;
						ghostCollider->localPosition = localPosition;
						break;
					}
				
				}

				if (obstacle->localPosition.z < localPosition.z)
				{
					moveSpeed = 0;
					if (!obstacle->ColliderObstacle(ghostCollider))
					{
						moveSpeed = beforeMoveSpeed;
						ghostCollider->localPosition = localPosition;
						break;
					}
				}
			}
		}
	}
}



