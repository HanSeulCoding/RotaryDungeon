#pragma once
class Texture2D : public Sprite
{
protected:
	float top;
	float bottom;
	float right;
	float left;

	
	int itemNum;
	bool isMouseOn;
	RenderTarget* renderTarget;
	DepthStencil* depthStencil;

	Outline* outline;
	int clickCount;

	

public:
	Texture2D(wstring textureFile, wstring shaderFile = L"Texture");
	~Texture2D();

	virtual void Update() = 0;
	virtual void PreRender() = 0;
	virtual void Render() = 0;
	virtual void PostRender() = 0;
	void CreateRec();
	bool IsOnMousePos();
	string key;

	int GetItemNum() { return itemNum; }
	void SetItemNum(int value) { itemNum = value; }
	int GetClickCount() { return clickCount; }
	void SetClickCount(int value) { clickCount = value; }

};