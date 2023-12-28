#pragma once
#include "WND.h"
#include "Timer.h"
#include <ShObjIdl.h>
#include <filesystem>
#include <iostream>
class Mainapp
{
private:
	const int ScreenWidth = 1024;
	const int ScreenHeight = 820;
	MSG msg;
	BOOL result;
	WND WND1;
	Timer timer;
	D2D1_POINT_2F MousePosition;
public:
	Mainapp();
	int Go();
	void DoFrame();
	void CopyBitmapToProjectFolder(const std::wstring& sourceFilePath);
	void ProcessMessages();


};