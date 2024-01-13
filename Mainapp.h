#pragma once
#include "WND.h"
#include "Timer.h"
#include <filesystem>
#include <iostream>
#include <wincodec.h>
#include <ShObjIdl.h>
#include <gdiplus.h>
#include<vector>
#include"LevelInstance.h"
#include"TextureInstance.h"
#pragma comment(lib, "gdiplus.lib")
class Mainapp
{
private:
	int TextureInstanceTabCounter =-1;
	std::vector<TextureInstance> TextureInstanceTab;
	const int ScreenWidth = 1024;
	const int ScreenHeight = 820;
	MSG msg;
	BOOL result;
	WND WND1;
	Timer timer;
	D2D1_POINT_2F MousePosition;
	UINT ScaleTwidth = 0, ScaleTheight = 0;
    LevelInstance Currentlevel;
	
public:
	std::vector<LevelInstance> LevelInstanceTab;
	Mainapp();
	int Go();
	void DoFrame();
	void HandleInput();
	void DoLogic();
	void DoDrawing();
	std::filesystem::path CopyBitmapToProjectFolder(const std::wstring& sourceFilePath);
	void ProcessMessages();
	void LoadBMPToTexture(const std::wstring& filePath, Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, ID2D1Bitmap** ppBitmap);
	


};