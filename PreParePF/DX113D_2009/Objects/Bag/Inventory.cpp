#include "Framework.h"

Inventory::Inventory()
	:Texture2D(L"Textures/Bag/Bag.png"),countX(-1),countY(0),change(false),
	isClick(false)
{
	localPosition = Vector3(709, 387, 0);
	localScale = Vector3(387, 516, 0);
	CreateRec();
	
	meatImage = new MeatImage();

	FONT->Add("meat", L"Basic", Float3(1, 1, 1), 20);
	FONT->Add("hpPotion", L"Basic", Float3(1, 1, 1), 20);
	FONT->Add("chicken", L"Basic", Float3(1, 1, 1), 20);
}

Inventory::~Inventory()
{
	delete meatImage;

}

void Inventory::Update()
{
	time += DELTA;
	if (time < 0.1f)
		return;
	mousePos = MOUSEPOS;
	if (MOUSEPOS.x < left || MOUSEPOS.x > right || MOUSEPOS.y > bottom || MOUSEPOS.y < top)
	{
		if(KEY_DOWN(VK_LBUTTON))
		{ 
			GMANAGER->SetExcuteInventory(false);
			time = 0.0f;
		}
		
	}
	for (auto item : items)
		item.second->Update();
	ClickItem();
}
void Inventory::CreateItem(string key,Texture2D* texture)
{
	
	if (items.size() > 0)
	{
		for (auto item : items) //���� Ű�� ������ �κ��丮â�� ��ĭ�� �÷����� �ʱ� ���� �ٷ� ����
		{
			if (item.second->key == key)
			{
				items[key]->SetItemNum(items[key]->GetItemNum() + 1);
				return;
			}
		}
	}
	
	countX++;
	if (countX > 4 && countX % 4 == 0)
	{
		countY++;
	}

	this->key = key;
	texture->key = key; 
	texture->localScale *= 80; //�κ��丮 �� �� ����
	texture->localPosition.x = 580 + (90*countX);
	texture->localPosition.y = 510 - (60 * countY);

	
	items[key] = texture;
	items[key]->SetItemNum(items[key]->GetItemNum() + 1);
	 //Item ���� �Է� // Inventory ȭ�鿡 Item �����ľ� ����

}
void Inventory::ClickItem() //�������� Ŭ���Ұ��
{
	for (auto item : items)
	{
		if (item.second->IsOnMousePos()) //������ ��ó�� ���콺�� �������
		{
			if (KEY_DOWN(VK_LBUTTON))
			{
				if (item.second->key == "meat")
				{
					itemTag = "meat";
					ChangeClickCount(item.second);
				}

				else if (item.second->key == "hpPotion")
				{
					ChangeClickCount(item.second);
					itemTag = "hpPotion";
				}
				if (item.second->key == "chicken")
				{
					ChangeClickCount(item.second);
					itemTag = "chicken";
				}
			}
		}
	}
	
}
void Inventory::ChangeClickCount(Texture2D* item)
{
	if (item->GetItemNum() < 1)
		return;

	isClick = true;
	item->SetItemNum(item->GetItemNum() - 1);
	item->SetClickCount(item->GetClickCount() + 1);
}
void Inventory::Render()
{
	Sprite::Render();
	for (auto item : items)
	{
		item.second->Render();
		wstring s;
		s = to_wstring(item.second->GetItemNum());
		Float2 pos;
		pos.x = item.second->localPosition.x +10;
		pos.y = WIN_HEIGHT - item.second->localPosition.y -10;

		Float2 size = { 90,100 };
		FONT->RenderText(s, item.second->key, pos, size);
	}
	
}

void Inventory::PreRender()
{
	for (auto item : items)
	{
		item.second->PreRender();
	}
}

void Inventory::PostRender()
{

	//for (auto item : items)
	//{
	//	item.second->PostRender();
	//}
	/*ImGui::SliderFloat3("x", (float*)&items[a]->localPosition,0,2000);
	ImGui::SliderFloat3("scale", (float*)&items[a]->localScale, 0, 2000);
	ImGui::SliderFloat("left",(float*)&left, 0, 2000);
	ImGui::SliderFloat("right", (float*)&right, 0, 2000);
	ImGui::SliderFloat("top", (float*)&top, 0, 2000);
	ImGui::SliderFloat("bottom", (float*)&bottom, 0, 2000);
	ImGui::InputFloat3("mousePos", (float*)&mousePos);*/

}


