#pragma once
class Inventory;
class ItemManager : public Singleton<ItemManager>
{
private:
	friend class Singleton;
	

	int index;
	Inventory* inventory;
	ItemManager();
	~ItemManager();
public:
	map<string, vector<Item*>> items;
	void CreateMeatItem(string key, Vector3 position, Vector3 size);
	void CreateMeatItem(string key, Matrix& world,int index,Vector3 localScale = Vector3(2,2,2));
	void CreateHpPotionItem(string key, Matrix& world,int index, Vector3 localScale = Vector3(5, 5, 5));
	void CreateHpPotionItem(string key, Vector3 position, Vector3 size);
	void CreateItem(string key, Item* item);
	void CreateItem(string key, Item* item,bool isRender);
	map<string, vector<Item*>>& GetItems() { return items; }
	void SetItems(map<string, vector<Item*>> value) { items = value; }
	
	void SetInventory(Inventory* value) { inventory = value; }
	void Render(string key);
	void SaveData();
	void LoadData();


};