#include "Mainapp.h"
int __stdcall WinMain(HINSTANCE hinstance,HINSTANCE hPrevInstance,LPSTR comline,int consoleshow)
{
	
	try {
		Mainapp app;
		app.Go();
	}
		catch (Errorexc& e)	// Pamiêtaj o kolejnosci o WND::oErrorexc& e to wci¹¿ jest Errorexc& e
		{
			MessageBoxA(nullptr, e.Result(), e.ErrorType(), MB_OK | MB_ICONEXCLAMATION);
		}
		catch (std::exception& e)
		{
			MessageBoxA(nullptr, e.what(), "Standard Exception", MB_OK | MB_ICONEXCLAMATION);
		}
		catch (...)
		{
			MessageBoxA(nullptr, "No Details about Exception", "Unknown Exception", MB_OK | MB_ICONEXCLAMATION);
		}
	
	

		return -1;
}

