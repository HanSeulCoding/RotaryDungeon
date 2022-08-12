#pragma once
class Fox;
class Monster;
class Ezreal : public Players
{
private:
	enum EzState
	{
		VISION1 = 3,VISION2
	};
	bool time;
	bool isCollision;
	bool isSelect;
	bool oneTime;
	bool isRender;
	bool isSkill2;

	float animationTime;
	//Weapon* meat;
	Vector3 lightPosition;
	Vector3 lightRotation;

	Vector3 rightHandPos;
	Matrix rightHand;
	Matrix leftHand;
	Collider* attackCollider;
	Texture2D* ezrealUI;
	Fox* fox;
	vector<Lightning*> lightnings;

public:
	Ezreal(bool isSelect = false);
	~Ezreal();
	void Update();
	void Render();
	virtual void PostRender() override;
	virtual void Skill() override;
	void IsControl();
	void IsNotControl();
	void NoControl();

	Vector3 SetVelocity() { return velocity; }

	void FontRender();

	void SetFox(Fox* value) { fox = value; }
	void CreateLightning();
	void EzrealAttack();
	void EzrealEndAttack();
	void CollisionLight(Lightning* light);
	void Vision1EndMotion();
private:

	void SetWeapon();

	// Players을(를) 통해 상속됨

};