#pragma once
#include "okno.h"
#include "Timer.h"
class Mainapp
{
public:
	const int* pScreenHeight = &ScreenHeight;
	const int* pScreenWidth = &ScreenWidth;
private:
	const int ScreenWidth = 820;
	const int ScreenHeight = 640;
	MSG msg;
	BOOL result;
	Okno okno1;
	Timer timer;
public:
	Mainapp();
	int Go();
	void DoFrame();
	void ProcessMessages();


};