#pragma once
#include"GFX.h"
#include"TextureInstance.h"
#include "vector"
#include "WND.h"
#include <wincodec.h>
#include"Timer.h"
class AnimationHolder;
class Animation 
{
public:
	Animation();
	void InitializeAnimation(AnimationHolder& animhold, Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, std::wstring filePath);
	void LoadFrame(const std::wstring& filePath, Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, const D2D1_RECT_F& sourceRegion, TextureInstance &TempInstance);
	Timer AnimationTimer;
	int CurrentFrame=0;
	std::wstring SpreadSheetPath;
	int FrameSizeX,FrameSizeY;
	int FrameAmmountX, FrameAmmountY;
};
class AnimationHolder
{
public:
	std::vector<Animation> Animations;
	std::vector<std::vector<TextureInstance>> AnimationFrames;
	static AnimationHolder& Get() noexcept
	{
		static AnimationHolder instance;
		return instance;
	}

};