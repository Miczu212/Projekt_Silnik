#pragma once
#include <d2d1.h>
#include <wrl.h>
#include <filesystem>
class TextureInstance
{
public:
	Microsoft::WRL::ComPtr<ID2D1Bitmap> pBitmap;
	UINT Twidth=0, Theight=0;
	std::vector<D2D1_RECT_F> destinationRectTab; 
	std::vector< Microsoft::WRL::ComPtr<ID2D1Bitmap>> pSubregions;
	std::wstring Path;
	bool IsCollisionOn = true;


};
