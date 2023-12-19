#include "Mainapp.h"
int __stdcall WinMain(HINSTANCE hinstance,HINSTANCE hPrevInstance,LPSTR comline,int consoleshow)
{
	Mainapp app;
	try {
		 app.Go();
	}


		catch (Okno::oErrorexc& e)	// Pami�taj o kolejnosci o Okno::oErrorexc& e to wci�� jest Errorexc& e
		{
			MessageBoxA(nullptr, e.Result(), "B��d przewidziany", MB_OK | MB_ICONEXCLAMATION);
		}
		catch (Errorexc& e)
		{
			MessageBoxA(nullptr, e.Result(), "B��d nieprzewidziany", MB_OK | MB_ICONEXCLAMATION);
		}

		catch (...)
		{
			MessageBoxA(nullptr, "Brak szczeg��w", "B��d nieprzewidziany", MB_OK | MB_ICONEXCLAMATION);
		}

		return -1;
}

