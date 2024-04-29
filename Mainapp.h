#pragma once
#include<math.h>
#include"ConstantDefines.h"
#include"ComManager.h"
#include <algorithm>
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
#include "Animation.h"
#pragma comment(lib, "gdiplus.lib")
class Mainapp
{
private: //Tablice
	std::vector<LevelInstance> LevelHolder;
	std::vector<TextureInstance> TextureHolder;
	std::vector<Sound> AudioHolder;
	std::vector<std::wstring> AudioPathHolder;
private:
	//Flagi bool
	bool DrawAnim = false;
	bool CameraXState = false;
	bool CameraYState = false;
	bool Collision = false;
	bool czyrysowaclinie = true;
	bool RepeatIfPossible = false;
	bool GravityChanged = false;
	bool IsGravityTurnedOn = false;
	bool IsJumping = false;
private: //Wartoœci int
	const int ScreenWidth = 1680;
	const int ScreenHeight = 820;
	int GravitySpeed = 3;
	int CameraXPosition = 0;
	int CameraYPosition = 0;
	int CurrentCameraSpeed = 0;
	int MaxCameraSpeed = 400;
	int ScaleDirection = 1;
	int CurrentJumpHeight = 0;
	int MaxJumpHeight = 400;
private: // Flagi int
	int AnimationIndex = 0;
	int TextureCounter = -1;
	int AudioCounter = -1;
	int SelectionMode = MODE_PLACE;
	int SelectionRectCounter = -1;
private: //Wszystkie rollbacki (kopie zapasowe do wczytania pozycji)
	int RollbackRectBottom=0;
	int RollbackRectRight=0;
	int RollbackRectLeft=0;
	int RollbackRectTop=0;
private: //Ró¿ne
	const D2D1_RECT_F Background = D2D1::RectF(0, 0, ScreenWidth, ScreenHeight);
	UINT32 KeyColour = 0xFFFF00FF;
	MSG msg;
	BOOL result;
	WND WND1;
	Timer timer;
	D2D1_POINT_2F MousePosition;
	UINT ScaleTwidth = 0, ScaleTheight = 0;
	LevelInstance Currentlevel;
	Player CurrentPlayer;
	ID2D1SolidColorBrush* BackgroundColour = nullptr;
public: // Wszystko co ma model Singelton (¿e ma byæ tylko jedno)
	SoundHandler& Soundhandler = SoundHandler::Get();
	ComManager& commanager = ComManager::Get();
	AnimationHolder& AnimHolder = AnimationHolder::Get();
	Font& font = Font::Get();
public: //Podstawowe metody
	Mainapp();
	int Go();
	void PlayAnimation();
	void DoFrame();
	void DoLogic();
	void DoDrawing();
	void ProcessMessages() noexcept;
	void HandleInput() noexcept;
public: //Metody Wczytywania
	void LoadFileTypeAudio();
	void LoadFileTypeLevel();
	void SaveFileTypeLevel();
	void LoadFileTypeTexture();
public: //Inicjalizacja textury wczytanej do postaci czytelnej przez program
	void LoadBMPToTexture(const std::wstring& filePath, Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, ID2D1Bitmap** ppBitmap);
	void LoadBMPSubregionToTexture(const std::wstring& filePath, Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget,
		const D2D1_RECT_F& sourceRegion, std::vector<Microsoft::WRL::ComPtr<ID2D1Bitmap>>& ppBitmap) const;
public: //Metody zmieniaj¹ce postaæ ekranu
	void Jump();
	void UpdateGravity();
	void UpdateCameraPosition();
	void Write(std::string Text, int StartPositionX, int StartPositionY);
public: //Metody Kolizyjne
	bool IFColision(const D2D1_RECT_F& rect1, const D2D1_RECT_F& rect2) const noexcept;
	int IFColisionWithSides(const D2D1_RECT_F& rect1, const D2D1_RECT_F& rect2) const noexcept;
public: //Ró¿ne
	std::filesystem::path CopyFileToProjectFolder(const std::wstring& SourceFilePath) const;
	std::wstring OpenFileDialog(LPCWSTR Filetype, LPCWSTR FileExtension);




};