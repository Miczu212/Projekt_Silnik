#include "Mainapp.h"
int __stdcall WinMain(HINSTANCE hinstance,HINSTANCE hPrevInstance,LPSTR comline,int consoleshow)
{
	Mainapp app;
	try {
		 app.Go();
	}


		catch (Okno::oErrorexc& e)	// Pamiêtaj o kolejnosci o Okno::oErrorexc& e to wci¹¿ jest Errorexc& e
		{
			MessageBoxA(nullptr, e.Result(), "B³¹d przewidziany", MB_OK | MB_ICONEXCLAMATION);
		}
		catch (Errorexc& e)
		{
			MessageBoxA(nullptr, e.Result(), "B³¹d nieprzewidziany", MB_OK | MB_ICONEXCLAMATION);
		}

		catch (...)
		{
			MessageBoxA(nullptr, "Brak szczegó³ów", "B³¹d nieprzewidziany", MB_OK | MB_ICONEXCLAMATION);
		}

		return -1;
}

