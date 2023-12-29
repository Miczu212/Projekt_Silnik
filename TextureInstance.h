#pragma once
#include <d2d1.h>
#include <wrl.h>
#include <filesystem>
class TextureInstance
{
public:
	Microsoft::WRL::ComPtr<ID2D1Bitmap> pBitmap;
	UINT Twidth=0, Theight=0;
	D2D1_RECT_F GetRect();
	D2D1_POINT_2F TexturePoint; //suprise tool that will help us later
	std::filesystem::path GetPath();
	void SetPath(std::filesystem::path path);
	void SetRect(D2D1_RECT_F rect);
private:
std::filesystem::path Path;
D2D1_RECT_F destinationRect; //suprise tool that will help us later

};