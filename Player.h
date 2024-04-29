#pragma once
#include <d2d1.h>
#include <wrl.h>
#include <filesystem>
#include "TextureInstance.h"
class Player
{
public:
	Player& operator=(const Player& other)
	{
		this->CurrentPlayerTexture = other.CurrentPlayerTexture;
		this->PlayerRect = other.PlayerRect;
		return *this;
	}
	class PlayerTexture 
	{
	public:
		PlayerTexture operator=(TextureInstance& Texture)
		{
			this->pBitmap = Texture.pBitmap;
			this->Path = Texture.Path;
			this->Theight = Texture.Theight;
			this->Twidth = Texture.Twidth;
			return *this;
		}

		Microsoft::WRL::ComPtr<ID2D1Bitmap> pBitmap;
		UINT Twidth = 0, Theight = 0;
		std::wstring Path;
	};

	PlayerTexture CurrentPlayerTexture;
	D2D1_RECT_F PlayerRect;
};
