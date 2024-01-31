#pragma once
#include <d2d1.h>
#include <wrl.h>
#include <filesystem>
class Font
{
public:
	Font();
	void InitializeFont(Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, std::vector<Microsoft::WRL::ComPtr<ID2D1Bitmap>>& ppBitmap) const;
	void LoadLetter(Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, const D2D1_RECT_F& sourceRegion, std::vector<Microsoft::WRL::ComPtr<ID2D1Bitmap>>& ppBitmap) const;
public:
	 std::vector< Microsoft::WRL::ComPtr<ID2D1Bitmap>> pSubregions;
};