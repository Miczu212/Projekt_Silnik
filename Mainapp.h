#pragma once
#include<math.h>
#include"ConstantDefines.h"
#include"ComManager.h"
#include "WND.h"
#include "Player.h"
#include "Timer.h"
#include <filesystem>
#include <iostream>
#include <wincodec.h>
#include <ShObjIdl.h>
#include <gdiplus.h>
#include<vector>
#include"LevelInstance.h"
#include"Font.h"
#include"TextureInstance.h"
#include"FunctionDefines.h"
#include <codecvt>
#include "SoundHandler.h"
#pragma comment(lib, "gdiplus.lib")
class Mainapp
{
private:
	std::vector<LevelInstance> LevelHolder;
	std::vector<TextureInstance> TextureHolder;
	std::vector<Sound> AudioHolder;
	std::vector<std::wstring> AudioPathHolder;
private:
	//flagi
	bool CameraXState = false;
	bool CameraYState = false;
	bool Collision = false;
	bool czyrysowaclinie = true;
	bool RepeatIfPossible = false;
private:
	const int ScreenWidth = 1680;
	const int ScreenHeight = 820;
	int CameraXPosition = 0;
	int CameraYPosition = 0;
	int CameraSpeed = 10;
	int TextureCounter = -1;
	int AudioCounter = -1;
	int SelectionMode = MODE_PLACE;
	int SelectionRectCounter = -1;
private:
	int RollbackRectBottom=0;
	int RollbackRectRight=0;
	int RollbackRectLeft=0;
	int RollbackRectTop=0;
private:
	UINT32 KeyColour = 0xFFFF00FF;
	MSG msg;
	BOOL result;
	WND WND1;
	Timer timer;
	D2D1_POINT_2F MousePosition;
	UINT ScaleTwidth = 0, ScaleTheight = 0;
	LevelInstance Currentlevel;
	Player CurrentPlayer;
	SoundHandler& Soundhandler = SoundHandler::Get();
    ComManager commanager;
    Font font;
	const D2D1_RECT_F Background = D2D1::RectF(0, 0, ScreenWidth, ScreenHeight);
	ID2D1SolidColorBrush* BackgroundColour = nullptr;
public:
	Mainapp();
	int Go();
	void DoFrame();
	void DoLogic();
	void DoDrawing();
public:

public:
	void LoadFileTypeAudio();
	void LoadFileTypeLevel();
	void SaveFileTypeLevel();
	void LoadFileTypeTexture();
public:
	void LoadBMPSubregionToTexture(const std::wstring& filePath, Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget,
		const D2D1_RECT_F& sourceRegion, std::vector<Microsoft::WRL::ComPtr<ID2D1Bitmap>>& ppBitmap) const;
	void Write(std::string Text, int StartPositionX, int StartPositionY);
	void UpdateCameraPosition();
	void HandleInput() noexcept;
	void ProcessMessages() noexcept;
	void LoadBMPToTexture(const std::wstring& filePath, Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, ID2D1Bitmap** ppBitmap);
	bool IFColision(const D2D1_RECT_F& rect1, const D2D1_RECT_F& rect2) const noexcept;
public:
	std::filesystem::path CopyFileToProjectFolder(const std::wstring& SourceFilePath) const;
	std::wstring OpenFileDialog(LPCWSTR Filetype, LPCWSTR FileExtension);




};