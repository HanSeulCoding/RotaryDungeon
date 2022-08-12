#include "Framework.h"

Texture2D::Texture2D(wstring textureFile,wstring shaderFile)
	:Sprite(shaderFile),top(0),bottom(0),right(0),left(0)
{
	material->SetDiffuseMap(textureFile);

}

Texture2D::~Texture2D()
{
}

void Texture2D::CreateRec()
{
	top = WIN_HEIGHT - (localPosition.y + (localScale.y * 0.5f));
	bottom = WIN_HEIGHT - abs(localPosition.y - (localScale.y * 0.5f));
	right = localPosition.x + (localScale.x * 0.5f);
	left = localPosition.x - (localScale.x * 0.5f);
}

bool Texture2D::IsOnMousePos()
{
	if (MOUSEPOS.x > left && MOUSEPOS.x < right)
	{
		if (MOUSEPOS.y < bottom && MOUSEPOS.y > top)
		{
			isMouseOn = true;
			return true;
		}
		else
		{
			isMouseOn = false;
			return false;
		}
	}
	else
	{
		isMouseOn = false;
		return false;
	}
}


	



