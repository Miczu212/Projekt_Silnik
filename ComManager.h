#pragma once
#include <ObjBase.h>
class ComManager
{
public:
	ComManager()
	{
		CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	}
	~ComManager()
	{
		CoUninitialize();
	}
};