#pragma once

struct FontDesc
{	
	ID2D1SolidColorBrush* brush;	
	IDWriteTextFormat* format;

	FontDesc() : brush(nullptr), format(nullptr)
	{
	}
	~FontDesc()
	{
		brush->Release();
		format->Release();
	}
};

class DirectWrite : public Singleton<DirectWrite>
{
private:
	friend class Singleton;

	ID2D1Factory1* factory;
	IDWriteFactory* writeFactory;

	ID2D1Device* device;
	ID2D1DeviceContext* context;

	ID2D1Bitmap1* targetBitmap;

	map<string, FontDesc*> fonts;

	DirectWrite();
	~DirectWrite();

public:
	void Add(string key, wstring font, Float3 color = Float3(1, 1, 1), float size = 20.0f,
		DWRITE_FONT_WEIGHT weight = DWRITE_FONT_WEIGHT_NORMAL,
		DWRITE_FONT_STYLE style = DWRITE_FONT_STYLE_NORMAL,
		DWRITE_FONT_STRETCH stretch = DWRITE_FONT_STRETCH_NORMAL);

	void RenderText(wstring text, string key, Float2 pos, Float2 size);

	ID2D1DeviceContext* GetDC() { return context; }
};