#include "okno.h"
int __stdcall WinMain(
	HINSTANCE hinstance,
	HINSTANCE hPrevInstance,
	LPSTR comline,
	int consoleshow
)

{
	try {
		okno okno1(820, 640, "testing");
		MSG msg;
		BOOL result;
		while ((result = GetMessageA(&msg, nullptr, 0, 0) > 0))
		{
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		return msg.wParam;
	}
		catch (okno::oErrorexc& e)	// Pami�taj o kolejnosci o okno::oErrorexc& e to wci�� jest Errorexc& e
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
// DZIALA 