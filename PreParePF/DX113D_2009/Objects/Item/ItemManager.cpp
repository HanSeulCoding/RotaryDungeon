#include "Framework.h"

ItemManager::ItemManager()
	:index(0)
{
}

ItemManager::~ItemManager()
{
}

void ItemManager::CreateMeatItem(string key,Vector3 position, Vector3 size)
{
	Meat* meat = new Meat();
	meat->localPosition = position;
	meat->localScale = size;
	meat->SetIsActive(false);
	items[key].emplace_back(meat);
}

void ItemManager::CreateMeatItem(string key, Matrix& world, int index, Vector3 localScale)
{
	Meat* meat = new Meat();
	meat->SetParent(&world);
	meat->SetIsActive(false);
	meat->localScale *= localScale;
	meat->SetIndex(index);
	items[key].emplace_back(meat);
}

void ItemManager::CreateHpPotionItem(string key, Matrix& world, int index, Vector3 localScale)
{
	HpPotion* hpPotion = new HpPotion();
	hpPotion->SetParent(&world);
	hpPotion->SetIsActive(false);
	hpPotion->SetIndex(index);
	hpPotion->localScale *= localScale;
	hpPotion->localRotation.x = 2.0f;
	hpPotion->localPosition.y += 30;
	items[key].emplace_back(hpPotion);
}

void ItemManager::CreateHpPotionItem(string key, Vector3 position, Vector3 size)
{
	HpPotion* hpPotion = new HpPotion();
	hpPotion->localPosition = position;
	hpPotion->localScale = size;
	hpPotion->SetIsActive(false);
	items[key].emplace_back(hpPotion);
}

void ItemManager::CreateItem(string key, Item* item)
{
	item->SetIsActive(false);
	items[key].emplace_back(item);
}

void ItemManager::CreateItem(string key, Item* item, bool isRender)
{
	item->SetIsActive(isRender);
	items[key].emplace_back(item);
}

void ItemManager::Render(string key)
{
	int size = items[key].size();
	if (items[key].size() <= 0)
		return;
	for (auto item : items[key])
	{
		if (item->GetIsActive())
		{
			item->Render();
		}
	}
}

void ItemManager::SaveData()
{
	BinaryWriter* bw = new BinaryWriter(L"GameData/ItemData.txt");
	bw->Int(ITEMMANAGER->GetItems()["player"].size());
	for (int i = 0; i < ITEMMANAGER->GetItems()["player"].size(); i++)
		bw->String(ITEMMANAGER->GetItems()["player"][i]->tag);
	
	delete bw;
}

void ItemManager::LoadData()
{
	BinaryReader* rw = new BinaryReader(L"GameData/ItemData.txt");

	int size = rw->Int();
	
	for (int i = 0; i < size; i++)
	{
		Item* item = nullptr;
		Texture2D* itemImage = nullptr;
		string tag = rw->String();
		if (tag == "meat") //태그에 맞는 아이템 저장
		{
			item = new Meat(); 
			itemImage = new MeatImage();
		}
		if (tag == "hpPotion")
		{
			item = new HpPotion();
			itemImage = new HpPotionImage();
		}
		if (tag == "chicken")
		{
			item = new ChickenItem();
			itemImage = new ChickenImage();
		}
		
		item->localScale *= 0.2;
		CreateItem("player", item,false);

		if(item != nullptr && itemImage != nullptr)
			inventory->CreateItem(tag, itemImage); //인벤토리에 아이템 사진 저장

	}
	delete rw;
}
