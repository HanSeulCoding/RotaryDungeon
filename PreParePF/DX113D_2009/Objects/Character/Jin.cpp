#include "Framework.h"

Jin::Jin(bool isSelect)
	: Players("Jin")
{
	this->isSelect = isSelect;
	hp = 100;
	curHp = hp;
	localScale *= 0.05f;
	ESkillTime = 2.0f;
	SetShader(L"Lighting");

	ReadClip(0,"Jin/Idle");
	ReadClip(1,"Jin/Run");
	ReadClip(2,"Jin/Attack");
	ReadClip(3, "Jin/Roll");

	SetEndEvent(ATTACK, bind(&Jin::EndAttack, this));
	SetEndEvent(ROLL, bind(&Jin::EndESkill, this));

	weapon = new Weapon();
	weapon->tag = "Axe";
	weapon->SetDiffuseMap(L"ModelData/Materials/Aex.png");

	weapon->localScale *= 100;
	weapon->SetParent(&rightHand);

	collider = new CapsuleCollider();
	collider->SetParent(&world);
	collider->localScale *= 50.0f;
	collider->localPosition.y = 100;
	collider->tag = "JinCollider";


	ghostCollider = new CapsuleCollider();

	localPosition = Vector3(241, 0, 240);

	FONT->Add("hp", L"Basic", Float3(1, 1, 1), 20);
	FONT->Add("exp", L"Basic", Float3(1, 1, 1), 20);
	FONT->Add("level", L"Basic", Float3(1, 1, 1), 20);
	

	
	jinUI = new JinUI();

	hpBar = new HpBar(Vector3(258, 645, 0), Vector3(322, 85, 0));
	expBar = new ExpBar(Vector3(258, 615, 0), Vector3(415, 64, 1));

	LoadData(L"Jin");

	hpBar->GetHpBuffer()->data.value = hp;
	expBar->GetExpBuffer()->data.value = 0;

	tag = "person";

}

Jin::~Jin()
{
	delete weapon;
	delete collider;
	delete hpBar;
	delete expBar;
}

void Jin::Update()
{
	if (!isSelect)
	{
		ghostCollider->localScale = Vector3(1.5, 1.5, 1.5);


		if (KEY_DOWN('Y'))
			isPress = true;
		if (KEY_DOWN(VK_F2))
		{
			isControl = true;
			CAMERA->SetTarget(this);
		}
		else if (KEY_DOWN(VK_F1))
			isControl = false;
		if (KEY_DOWN(VK_F3))
			CAMERA->SetTarget(nullptr);

		localPosition.y = terrain->GetHeight(localPosition); //터레인 높이값에 해당하는 Y값 설정
		SetWeapon(); //무기 웨폰 설정
		IsControl(); //컨트롤시 
		IsNotControl(); //컨트롤 안할시 
		SetLevel(); //레벨 설정
		ColliderObstacle(); //장애물 충돌시
		GhostColliderMove();

		if (ITEMMANAGER->GetItems()["player"].size() != 0) //player key에 해당하는 아이템 Update
		{
			for (auto item : ITEMMANAGER->GetItems()["player"])
				item->Update();
		}
		if (KEY_DOWN('H')) //데미지 깎게 실험
		{
			ChangeHp(false, 20);
			float bar = (float)curHp / (float)hp;

			hpBar->GetHpBuffer()->data.value = bar;
		}


		hpBar->GetHpBuffer()->data.value = (float)curHp / (float)hp;
		expBar->GetExpBuffer()->data.value = (float)exp / (float)level.levelExp;
	}
	ModelAnimator::Update();
}

void Jin::Render()
{
	
	SetWorldBuffer();
	ModelAnimator::Render();
	if (!isSelect)
	{
		weapon->Render();
		collider->Render();
		ghostCollider->Render();

		if (ITEMMANAGER->GetItems()["player"].size() != 0)
		{
			vector<Item*> hanseul = ITEMMANAGER->GetItems()["player"];
			for (auto item : ITEMMANAGER->GetItems()["player"])
			{
				if (item->tag == "meat" && item->GetIsActive())
				{
					Vector3 position = item->localPosition;
					Vector3 scale = item->localScale;
					item->Render();
				}
			}
		}

		FontRender();
	}
}

void Jin::PostRender()
{

	hpBar->Render();
	expBar->Render();
	jinUI->Render();
}

void Jin::Skill()
{
	if (isAttack)
		return;
	ESkillTime += DELTA;
	if (ESkillTime > 2.0f)
	{
		//GMANAGER->SetIsPlayerESkill(false);
		if (KEY_DOWN('E'))
		{
			ESkillTime = 0.0f;
			isSkill = true;
			isInvincible = true;

			SetAnimation(ROLL);
			AUDIO->Play("RollSound");
			GMANAGER->SetIsPlayerESkill(true);
		}
	}
	if (isSkill)
	{
		localPosition -= Forward() * 20 * DELTA;
	}
}



void Jin::IsControl()
{
	if (isControl)
	{
		Control();
		GettingItem();
		SettingItem(this,player);
		Move();
		KillMonster(expBar);
		DetectedPlayer();
		GetChicken();
		Skill();
		if (KEY_DOWN(VK_LBUTTON))
			Attack(weapon->GetCollider());

	}
}


void Jin::IsNotControl()
{
	if (!isControl)
		NoControl();
}
void Jin::NoControl()
{
	TargetToMove(Vector3(player->localPosition.x - 10, 0, player->localPosition.z - 10), distance > 0.5f,20);

	if (distance > 0.5f)
	{
		SetAnimation(RUN);
	}
	else
	{
		SetAnimation(IDLE);
	}
}
void Jin::FontRender()
{
	wstring s;
	s = to_wstring(curHp);
	Float2 pos;
	pos.x = 455;
	pos.y = 105;

	Float2 size = { 90,100 };
	FONT->RenderText(s, "hp", pos, size);

	wstring s2;
	s2 = to_wstring(exp);
	Float2 pos2;
	pos2.x = 455;
	pos2.y = 145;

	Float2 size2 = { 90,100 };
	FONT->RenderText(s2, "exp", pos2, size2);

	wstring s3;
	s3 = L"Level:  " + to_wstring(level.level);
	Float2 pos3;
	pos3.x = 75;
	pos3.y = 68;

	Float2 size3 = { 90,100 };
	FONT->RenderText(s3, "level", pos3, size3);

	if (GMANAGER->GetIsPlayerESkill())
	{
		wstring s;
		int EskillTime = (int)(3.0 - ESkillTime);
		s = to_wstring(EskillTime);
		Float2 pos;
		pos.x = 1060;
		pos.y = 550;

		Float2 size = { 90,100 };
		FONT->RenderText(s, "playerE", pos, size);
	}

	
}
void Jin::SetWeapon()
{
	int nodeIndex = GetNodeByName("RightHand");
	
	rightHand = GetTransformByNode(nodeIndex) * world;	
}

void Jin::EndESkill()
{
	isSkill = false;
	isInvincible = false;
	SetAnimation(IDLE);
	GMANAGER->SetIsPlayerESkill(false);
}


