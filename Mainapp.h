#pragma once
#include "WND.h"
#include "Timer.h"
#include <filesystem>
#include <iostream>
#include <wincodec.h>
#include <ShObjIdl.h>
#include <gdiplus.h>
#include<vector>
#pragma comment(lib, "gdiplus.lib")
class Mainapp
{
private:
	int TextureTabCounter = -1;
	std::vector<std::filesystem::path> TextureTab;
	const int ScreenWidth = 1024;
	const int ScreenHeight = 820;
	MSG msg;
	BOOL result;
	WND WND1;
	Timer timer;
	D2D1_POINT_2F MousePosition;
	UINT Twidth=0, Theight=0;
	UINT ScaleTwidth = 0, ScaleTheight = 0;
	
	
public:
	Mainapp();
	int Go();
	void DoFrame();
	std::filesystem::path CopyBitmapToProjectFolder(const std::wstring& sourceFilePath);
	void ProcessMessages();
	void LoadBMPToTexture(const std::wstring& filePath, Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, ID2D1Bitmap** ppBitmap);
	


};