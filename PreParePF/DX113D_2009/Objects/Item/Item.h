#pragma once
class Players;
class Item : public ModelReader, public Transform
{
private:
	Collider* collider;
	

protected:
	int index;
	bool isDrop;
	bool isRender;
	float height;
	bool isActive; 
	bool isEat;
	bool isCheck;

	//string tag;
public:
	Item(string file);
	~Item();

	void Render();
	virtual void Update()=0;
	virtual void Function(Players* player) = 0;

	Collider* GetCollider() { return collider; }
	void SetIsActive(bool value) { isActive = value; }
	bool GetIsActive() { return isActive; }
	void SetIsRender(bool value) { isRender = value; }
	bool GetIsRender() { return isRender; }

	void DropItem(bool isDrop, float height);

	void SetIndex(int value) { index = value; }
	int GetIndex() { return index; }

	bool GetIsEat() { return isEat; }
	void SetIsEat(bool value) { isEat = value; }

	bool GetIsCheck() { return isCheck; }
	void SetIsCheck(bool value) { isCheck = value; }


	//string GetTag() { return tag; }
};