#pragma once
#include"GFX.h"
#include"TextureInstance.h"
#include "vector"
#include "WND.h"
#include <wincodec.h>
#include"AnimationHolder.h"
class Animation 
{
public:
	Animation();
	void InitializeAnimation(AnimationHolder& animhold, int Framesizex, int Framesizey, int FrameAmmountx, int FrameAmmounty, Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, std::wstring filePath);
	void LoadFrame(const std::wstring& filePath, Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, const D2D1_RECT_F& sourceRegion, Microsoft::WRL::ComPtr<ID2D1Bitmap>& ppBitmap);
	std::vector<TextureInstance>* Frames;
	int AnimationHolderIndex;
	int CurrentFrame = 0;
private:


	
};