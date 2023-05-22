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
		catch (okno::oErrorexc& e)	// Pamiêtaj o kolejnosci o okno::oErrorexc& e to wci¹¿ jest Errorexc& e
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
// DZIALA 