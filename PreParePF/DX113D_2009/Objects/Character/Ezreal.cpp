#include "Framework.h"

Ezreal::Ezreal(bool isSelect)
	:Players("Ezreal"),isRender(true)
{
	SetDiffuseMap(L"ModelData/Materials/ezreal_base_tx_cm.png");
	this->isSelect = isSelect;
	hp = 100;
	curHp = hp;
	localScale *= 0.1f;
	ESkillTime = 2.0f;
	SetShader(L"Lighting");

	ReadClip(0, "Ezreal/Idle");
	ReadClip(1, "Ezreal/Run");
	ReadClip(2, "Ezreal/Attack");
	ReadClip(3, "Ezreal/VisionShift1");
	ReadClip(4, "Ezreal/VisionShift2");


	SetEndEvent(ATTACK, bind(&Ezreal::EzrealEndAttack, this));
	SetEndEvent(VISION1, bind(&Ezreal::Vision1EndMotion, this));

	CreateLightning();

	collider = new CapsuleCollider();
	collider->SetParent(&world);
	collider->localScale *= 50.0f;
	collider->localPosition.y = 100;
	collider->tag = "JinCollider";
	

	ghostCollider = new CapsuleCollider();
	ghostCollider->UpdateWorld();
	attackCollider = new CapsuleCollider();
	attackCollider->UpdateWorld();
	localPosition = Vector3(241, 0, 240);

	FONT->Add("hp", L"Basic", Float3(1, 1, 1), 20);
	FONT->Add("exp", L"Basic", Float3(1, 1, 1), 20);
	FONT->Add("level", L"Basic", Float3(1, 1, 1), 20);



	ezrealUI = new EzrealUI();
	ezrealUI->localPosition = Vector3(50, 645, 0);

	hpBar = new HpBar(Vector3(258, 645, 0), Vector3(322, 85, 0));
	expBar = new ExpBar(Vector3(258, 615, 0), Vector3(415, 64, 1));

	LoadData(L"Ezreal");

	hpBar->GetHpBuffer()->data.value = hp;
	expBar->GetExpBuffer()->data.value = 0;

	tag = "person";

}

Ezreal::~Ezreal()
{
	delete collider;
	delete hpBar;
	delete expBar;
	for (auto light : lightnings)
		delete light;
}

void Ezreal::Update()
{
	
	if (!isSelect)
	{
		ghostCollider->localScale = Vector3(1.5, 1.5, 1.5);
		ghostCollider->UpdateWorld();

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
		
		SetWeapon();
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

	//localPosition = Vector3(241, 0, 240);
}

void Ezreal::Render()
{
	if (!isActive || !isRender)
	{
		FontRender();
		return;
	}

	SetWorldBuffer();
	ModelAnimator::Render();
	if (!isSelect)
	{
		
		for(auto lightning : lightnings)
			lightning->Render();
		

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

void Ezreal::PostRender()
{
	hpBar->Render();
	expBar->Render();
	ezrealUI->Render();

	
	//ImGui::InputFloat3("Ezposition", (float*)&localPosition);
	//ParticleManager::Get()->PostRender();
	
}

void Ezreal::Skill()
{
	ESkillTime += DELTA;
	if (isSkill)
		isInvincible = true;
	if (ESkillTime > 2.0f)
	{
		GMANAGER->SetIsPlayerESkill(false);
		if (KEY_DOWN('E'))
		{
			isSkill = true;
			oneTime = true;
			SetAnimation(VISION1);
			AUDIO->Play("VisionAudio");
		}
		if (isSkill2)
		{
			animationTime += DELTA;
			if (animationTime > 0.5)
			{
				isRender = true; //Render 할지 말지
				isSkill = false; //첫번째 VisionAnimation 꺼주기 위함
				isSkill2 = false; //Vision Animation2 반복 실행 불가하게 하기 위함 
				isInvincible = false;
				ParticleManager::Get()->Play("visionEffect", Vector3(localPosition.x, localPosition.y + 8
					, localPosition.z));
				SetAnimation(VISION2);
				animationTime = 0.0f; // 0.5초 뒤에 나타나게 하기 위함.
				ESkillTime = 0.0f; //스킬 시간 초기화.
				GMANAGER->SetIsPlayerESkill(true);
			}
		}
	}
}

void Ezreal::IsControl()
{
	if (isControl)
	{
		Control();
		GettingItem();
		SettingItem(this, fox);
		Move();
		if (!GMANAGER->GetIsBoss())
		{
			KillMonster(expBar);
			DetectedPlayer();
			GetChicken();
		}
		EzrealAttack();
		Skill();
	
		if (isCollision)//맞고나서 몬스터 색깔 다시 원상복구 하기 위해
		{
			time += DELTA;
			if (time > 1.5)
			{
				time = 0.0f;
				isCollision = false;
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
				if (GMANAGER->GetIsBoss())
				{
					if (boss->GetIsDamage())
					{
						boss->SetColor(Float4(1, 1, 1, 1));
						boss->SetIsDamage(false);
						boss->GetAttackDamage(0);
					}
					
				}
			}
		}
	}
}

void Ezreal::IsNotControl()
{
	if (!isControl)
		NoControl();
}

void Ezreal::NoControl()
{
	TargetToMove(Vector3(player->localPosition.x - 10, 0, player->localPosition.z - 10), distance > 0.5f, 20);

	if (distance > 0.5f)
	{
		SetAnimation(RUN);
	}
	else
	{
		SetAnimation(IDLE);
	}
}

void Ezreal::FontRender()
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



void Ezreal::SetWeapon()
{
	int nodeIndex = GetNodeByName("mixamorig:RightHand");

	rightHand = GetTransformByNode(nodeIndex) * world;
}

void Ezreal::CreateLightning()
{
	for (int i = 0; i < 20; i++)
	{
		Lightning* lightning = new Lightning();
		lightning->tag = "light";
		//lightning->localRotation = Vector3(0, 0, 0);
		lightning->localScale= Vector3(0.4, 1.5, 1.5);
		lightning->isActive = false;

		lightnings.emplace_back(lightning);
	}
}

void Ezreal::EzrealAttack()
{
	if (isSkill)
		return;

	if (KEY_DOWN(VK_LBUTTON))
	{
		SetAnimation(ATTACK);
		isAttack = true;
		for (auto lightning : lightnings)
		{
			if (lightning->isActive)
				continue;
			lightning->isGo = true;
			lightning->direction = Forward();
			isLightningMove = false;
			break;
		}
		
		
	}
	
	for (auto lightning : lightnings) //앞으로 이동
	{
		if (lightning->isActive)
			lightning->localPosition -= lightning->direction * 80 * DELTA;
	}
	
	
	for (auto lightning : lightnings) //rightHand에 맞춰주기
	{
		if (!lightning->isActive)
		{
			Vector3 scale;
			XMMatrixDecompose(&scale.data, &lightRotation.data, &lightPosition.data, rightHand);
			rightHandPos = Vector3(lightPosition.x, lightPosition.y, lightPosition.z);
			lightning->localPosition = Vector3(lightPosition.x, lightPosition.y, lightPosition.z);
			lightning->localRotation.y = localRotation.y-XM_PI*0.5;
		}
	}
	
	for (auto lightning : lightnings) //충돌처리
	{
		if (!lightning->isActive)
			continue;

		if (lightning->isActive)
		{
			CollisionLight(lightning);
		}
	}

	for (auto lightning : lightnings)
	{
		if (lightning->localPosition.z > 400)
		{
			lightning->isActive = false;
			lightning->isGo = false;
			lightning->isCollision = false;
		}
		if (lightning->localPosition.z < 0)
		{
			lightning->isActive = false;
			lightning->isGo = false;
			lightning->isCollision = false;
		}
		if (lightning->localPosition.x < 0)
		{
			lightning->isActive = false;
			lightning->isGo = false;
			lightning->isCollision = false;
		}
		if (lightning->localPosition.x > 400)
		{
			lightning->isActive = false;
			lightning->isGo = false;
			lightning->isCollision = false;
		}
	}
}

void Ezreal::EzrealEndAttack()
{
	isAttack = false;

	for (auto lightning : lightnings)
	{
		if (lightning->isActive)
			continue;
		/*if (lightning->isActive && !lightning->isGo)
		{
			lightning->isGo = true;
			AUDIO->Play("ezrealAttack");
			break;
		}*/
		if (lightning->isGo)
		{
			lightning->isActive = true;
			AUDIO->Play("ezrealAttack");
			lightning->isGo = true;
			break;
		}
	}
	ParticleManager::Get()->Play("lightning", rightHandPos);
}

void Ezreal::CollisionLight(Lightning* light)
{
	if (!GMANAGER->GetIsBoss())
	{
		for (auto monster : monsterM->GetMonsters())
		{
			monster->UpdateWorld();
			monster->GetCollider()->UpdateWorld();
			if (light->GetCollider()->Collision((
				CapsuleCollider*)monster->GetCollider()))
			{
				if (!monster->isActive)
					continue;
				if (monster->attribute == tag) //태그(속성) 같으면 추가 데미지
					addDamage = 50;

				if (!light->isCollision)
				{
					monster->SetDamage(level.attackDamage + addDamage); //monster한테 데미지 준다.
					monster->SetColor(Float4(1, 0, 0, 1)); //몬스터 색깔 바꿔준다.
					monster->GetAttackDamage(level.attackDamage + addDamage);
					attackCount++;
					collider->UpdateWorld();
					light->isCollision = true;
					isCollision = true;
				}

				addDamage = 0;
			}
		}
	}
	if (GMANAGER->GetIsBoss())
	{
		if (light->GetCollider()->Collision((
			CapsuleCollider*)boss->GetCollider()))
		{
			if (boss->attribute == tag) //태그(속성) 같으면 추가 데미지
				addDamage = 50;

			if (!light->isCollision)
			{
				boss->SetDamage(level.attackDamage + addDamage); //boss한테 데미지 준다.
				boss->SetColor(Float4(1, 0, 0, 1)); //몬스터 색깔 바꿔준다.
				boss->GetAttackDamage(level.attackDamage + addDamage);
				attackCount++;
				collider->UpdateWorld();
				light->isCollision = true;
				isCollision = true;
			}

			addDamage = 0;
		}
	}
}

void Ezreal::Vision1EndMotion()
{
	if (!oneTime)
		return;
	ParticleManager::Get()->Play("visionEffect", Vector3(localPosition.x, localPosition.y + 8
	,localPosition.z));
	isRender = false;
	oneTime = false;
	isSkill2 = true;
	localPosition -= Forward()*30;
}
