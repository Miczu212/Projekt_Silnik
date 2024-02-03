#pragma once
#include <ObjBase.h>
class ComManager
{
private:
	ComManager()
	{
		CoInitializeEx(nullptr, COINIT_APARTMENTTHREADED);
	}
public:
	~ComManager()
	{
		CoUninitialize();
	}
	static ComManager& Get() noexcept
	{
		static ComManager instance;
		return instance;
	}
};