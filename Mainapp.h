#pragma once
#include "okno.h"
class Mainapp
{
public:
	Mainapp();
	int Go();
	void DoFrame();
	MSG msg;
	BOOL result;
	Okno okno1;

};