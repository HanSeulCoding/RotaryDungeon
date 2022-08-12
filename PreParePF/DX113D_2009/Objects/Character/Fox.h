#pragma once
class Fox : public Players
{
private:
	enum DfferState
	{
		JUMP=3, EAT
	}differState;

	float time;
	int count;
	int index;
	bool isControl;
	bool isEatFinished;

	vector<int> indexes;

	FoxUI* foxUI;
	
	
public:
	Fox();
	~Fox();

	Item* item;

	void Update();
	void Render();
	virtual void PostRender() override;
	virtual void Skill() override;
	Vector3 GetVelocity() { return velocity; }
	void SetPlayer(Players* value) { player = value; }
	Collider* GetCollider() { return collider; }
	

private:
	void IsControl();
	void IsNotControl();
	void Jump();
	void NoControl();
	void EatMeat();
	void PlayerToMove();

	// Players을(를) 통해 상속됨

};