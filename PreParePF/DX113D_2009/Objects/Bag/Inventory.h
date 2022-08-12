#pragma once
class Inventory : public Texture2D
{
private:
	float time;
	Vector3 mousePos;
	map<string, Texture2D*> items;
	string key;
	vector<string> keys;
	Texture2D* meatImage;
	int countX;
	int countY;
	bool change;

	string itemTag;
	bool isClick;
public:
	Inventory();
	~Inventory();

	// Texture2D을(를) 통해 상속됨
	virtual void Update() override;
	virtual void PreRender() override;
	virtual void Render() override;
	virtual void PostRender() override;
	
	void CreateItem(string key,Texture2D* texture);
	map<string, Texture2D*>GetItems() { return items; }

	void ClickItem();
	void ChangeClickCount(Texture2D* item);
	bool GetIsClick() { return isClick; }
	void SetIsClick(bool value) { isClick = value; }

	string GetItemTag() { return itemTag; }

};