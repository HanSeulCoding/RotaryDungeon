#include "Framework.h"

Fox::Fox()
	:Players("fox"), differState(JUMP),
	count(0), isControl(false), isEatFinished(false)
{
	hp = 200;
	curHp = hp;
	moveSpeed = 20;
	localPosition = { 50, 0, 50 };

	SetEndEvent(ATTACK, bind(&Fox::EndAttack, this));
	SetDiffuseMap(L"ModelData/Materials/T_Fox_BC.png");
	SetNormalMap(L"ModelData/Materials/T_Fox_Normal.png");

	localScale *= 0.2f;
	ReadClip(0,"Fox/Idle", 0);
	ReadClip(1,"Fox/Run", 0);
	ReadClip(2, "Fox/Attack", 0);
	ReadClip(3,"Fox/Fox_Jump_InAir", 0);
	ReadClip(4,"Fox/Eat");

	collider = new CapsuleCollider();
	collider->SetParent(&world);
	collider->localScale *= 7;
	collider->localPosition.y = 10;

	ghostCollider = new CapsuleCollider();
	ghostCollider->UpdateWorld();

	beforeMoveSpeed = moveSpeed;
	FONT->Add("foxHp", L"Basic", Float3(1, 1, 1), 20);
	FONT->Add("foxExp", L"Basic", Float3(1, 1, 1), 20);

	hpBar = new HpBar(Vector3(258, 580, 0), Vector3(322, 85, 0));
	expBar = new ExpBar(Vector3(258, 550, 0), Vector3(415, 64, 1));
	foxUI = new FoxUI();
	
	LoadData(L"Fox");
	hpBar->GetHpBuffer()->data.value = hp;
	expBar->GetExpBuffer()->data.value = 0;

	tag = "animal";

}

Fox::~Fox()
{
}

void Fox::Update()
{
	if (KEY_DOWN(VK_F1))
	{
		CAMERA->SetTarget(this);
		isControl = true;
	}
	if (KEY_DOWN(VK_F2))
	{
		isControl = false;
	}
	if (KEY_DOWN('Y'))
	{
		ChangeHp(false, 30);
		isPress = true;
	}
	SetLevel();
	IsControl();
	IsNotControl();
	ColliderObstacle(); //장애물 충돌시
	GhostColliderMove();
	if (KEY_DOWN('J'))
	{
		ChangeHp(false, 20);
		float bar = (float)curHp / (float)hp;
		
		hpBar->GetHpBuffer()->data.value = bar;
	}
	hpBar->GetHpBuffer()->data.value = (float)curHp / (float)hp;
	expBar->GetExpBuffer()->data.value = (float)exp / (float)level.levelExp;
	ModelAnimator::Update();
}

void Fox::Render()
{
	SetWorldBuffer();
	ModelAnimator::Render();
	collider->Render();
	ghostCollider->Render();
	

	wstring s;
	s = to_wstring(curHp);
	Float2 pos;
	pos.x = 455;
	pos.y = 180;

	Float2 size = { 90,100 };
	FONT->RenderText(s, "foxHp", pos, size);

	wstring s2;
	s2 = to_wstring(exp);
	Float2 pos2;
	pos2.x = 450;
	pos2.y = 220;

	Float2 size2 = { 90,100 };
	FONT->RenderText(s2, "foxExp", pos2, size2);

	wstring s3;
	s3 = L"Level:  " + to_wstring(level.level);
	Float2 pos3;
	pos3.x = 75;
	pos3.y = 160;

	Float2 size3 = { 90,100 };
	FONT->RenderText(s3, "level", pos3, size3);
}

void Fox::PostRender()
{
	hpBar->Render();
	expBar->Render();
	foxUI->Render();
}

void Fox::Skill()
{
}

void Fox::IsControl()
{
	if (isControl)
	{
		Control();
		Move();
		GettingItem();
		SettingItem(this, player);
		DetectedPlayer();
		GetChicken();
		if (KEY_DOWN(VK_SPACE))
		{
			jumpPower = 80;
			isJump = true;
			SetAnimation(JUMP);

		}
		Jump();
		if (KEY_DOWN(VK_LBUTTON))
			Attack(collider);
		KillMonster(expBar);

	}
}

void Fox::IsNotControl()
{
	if (!isControl)
	{
		localPosition.y = terrain->GetHeight(localPosition);
		if (!player->IsDrop() && !isEatFinished)
			NoControl();
		if (player->IsDrop())
			EatMeat();
		if (isEatFinished)
			PlayerToMove();

	}
}

void Fox::Jump()
{
	float terrainY = terrain->GetHeight(localPosition);

	if (!isJump)
	{
		localPosition.y = terrainY;
		return;
	}
	
	jumpPower -= gravity * DELTA;

	localPosition.y += jumpPower * DELTA;

	if (localPosition.y <= terrainY)
	{
		SetAnimation(IDLE);
		isJump = false;
	}
}

void Fox::NoControl()
{
	
	TargetToMove(Vector3(player->localPosition.x - 10, 0, player->localPosition.z - 10), distance > 0.5f,15);
	
	if (distance> 0.5f)
	{
		SetAnimation(RUN);
	}
	else
	{
		if (!player->IsDrop())
			SetAnimation(IDLE);
	}
}

void Fox::EatMeat()
{
	for (int i =0 ;i<ITEMMANAGER->GetItems()["player"].size();i++)
	{
		string tag2 = ITEMMANAGER->GetItems()["player"][i]->tag;
		if (ITEMMANAGER->GetItems()["player"][i]->tag == "meat" && ITEMMANAGER->GetItems()["player"][i]->GetIsCheck())
		{
			item = ITEMMANAGER->GetItems()["player"][i];
			index = i;
			break;
		}//meat,portion구별해서 넣어준
		else
			item = nullptr;
	}
	if (ITEMMANAGER->GetItems()["player"].size() == 0)
		item = nullptr;

	if (item == nullptr)
	{
		player->GetBoolDrop(false); //남은 아이템이 없을시 플레이어한테 
		isEatFinished = true; // 쫓아간다. 
		SetAnimation(IDLE);
		return;
	}

	TargetToMove(item->localPosition, distance > 0.1f);

	if (distance > 0.1f)
	{
		SetAnimation(RUN);
	}
	else
	{
		PlayClip(EAT,1.5f);
		time += DELTA;
	
		if (time > 1.0f)
		{
			if (count == 0)
			{
				ITEMMANAGER->GetItems()["player"][index]->SetIsCheck(false);
				item->Function(this); //아이템 기능 한번 수행
				count++;
			}
			time = 0.0f;
			 //인덱스에 맞는 아이템 지워주기 
			ITEMMANAGER->GetItems()["player"].erase(ITEMMANAGER->GetItems()["player"].begin()+index);
			//int size = jin->SetMeats().size();
		
			SetAnimation(IDLE);//남은 아이템이 있을시 반복
		
			count = 0;
		}
	}
	if (player->IsDrop())
	{
		isEatFinished = false;
	}
	
}
void Fox::PlayerToMove()
{
	
	TargetToMove(Vector3(player->localPosition.x - 10, 0, player->localPosition.z - 10), distance > 0.5f);

	if (distance> 0.5f)
	{
		SetAnimation(RUN);
	}
	else
	{
		SetAnimation(IDLE);
		isEatFinished = false;
	}

}


