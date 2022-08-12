#include "Framework.h"

FemailEnemy::FemailEnemy(int index)
	:Monster("FemailEnemy")
{
	attribute = "person";
	moveSpeed = 20;
	rotSpeed = 2.0f;
	maxMoveSpeed = 10.0f;
	accelation = 15.0f;
	deceleration = 10.0f;
	attackDamage = 20.0f;
	tag = "femailEnemy";
	hp = 30;
	curHp = hp;
	this->index = index;
	localScale *= 0.08f;

	localPosition.x = rand() / (float)RAND_MAX * 380;
	localPosition.z = rand() / (float)RAND_MAX * 380;
	SetShader(L"ModelAnimation");

	SetDiffuseMap(L"ModelData/Materials/Enemy/Dungeons_Texture_01.png");
	ReadClip(0, "FemailEnemy/Idle");
	ReadClip(1, "FemailEnemy/Walking");
	ReadClip(2, "FemailEnemy/Run");
	ReadClip(3, "FemailEnemy/Attack");
	ReadClip(4, "FemailEnemy/Dead");
	//ReadClip(3,"Chicken/Eat");

	//collider->localPosition.y = 100;
	int count = rand() % 2 + 1;
	switch (count)
	{
	case 1:	ITEMMANAGER->CreateMeatItem("monster", world,index,Vector3(5,5,5));
		break;
	case 2:
		ITEMMANAGER->CreateHpPotionItem("monster", world,index,Vector3(10,10,10));
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

FemailEnemy::~FemailEnemy()
{
	delete meat;
	delete collider;
	delete detectCollider;
}

void FemailEnemy::Update()
{
	Monster::Update();
	AttackToMove();
	if (hp < 0)
	{
		
	}
}

void FemailEnemy::Render()
{
	Monster::Render();
	detectCollider->Render();
}

void FemailEnemy::PostRender()
{
}

void FemailEnemy::PreRender()
{
}
