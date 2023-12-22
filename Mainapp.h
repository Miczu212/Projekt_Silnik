#pragma once
#include "WND.h"
#include "Timer.h"
class Mainapp
{
private:
	const int ScreenWidth = 1024;
	const int ScreenHeight = 820;
	MSG msg;
	BOOL result;
	WND WND1;
	Timer timer;
public:
	Mainapp();
	int Go();
	void DoFrame();
	void ProcessMessages();


};