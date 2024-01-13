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
	std::filesystem::path GetPath() const;
	void SetPath(std::filesystem::path path);
	std::wstring PATHTest;
private:
std::filesystem::path Path;


};
