#pragma once
class Fox;
class Monster;
class Jin : public Players
{
private:

	enum JinState
	{
		ROLL = 3
	};
	bool isSelect;

	//Weapon* meat;
	Item* weapon;
	Matrix rightHand;
	Matrix leftHand;

	
	Texture2D* jinUI;
	//Fox* fox;

	
public:
	Jin(bool isSelect = false);
	~Jin();

	void Update();
	void Render();
	virtual void PostRender() override;
	virtual void Skill() override;
	void IsControl();
	void IsNotControl();
	void NoControl();

	Vector3 SetVelocity() { return velocity; }

	void FontRender();

	//void SetFox(Fox* value) { fox = value; }

private:
	
	void SetWeapon();
	void EndESkill();


	// Players을(를) 통해 상속됨


};