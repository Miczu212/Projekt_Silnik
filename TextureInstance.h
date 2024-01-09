#pragma once
#include <d2d1.h>
#include <wrl.h>
#include <filesystem>
class TextureInstance
{
public:
	Microsoft::WRL::ComPtr<ID2D1Bitmap> pBitmap;
	UINT Twidth=0, Theight=0;
	//std::vector<D2D1_POINT_2F> TexturePointTab; //suprise tool that will help us later
	std::vector<D2D1_RECT_F> destinationRectTab; //suprise tool that will help us later
	std::filesystem::path GetPath();
	void SetPath(std::filesystem::path path);
private:
std::filesystem::path Path;


};
