#include "okno.h"
int __stdcall WinMain(
	HINSTANCE hinstance,
	HINSTANCE hPrevInstance,
	LPSTR comline,
	int consoleshow
)

{

	okno okno1(820,640,"testing");
	MSG msg;
	BOOL result;
		while ((result=GetMessageA(&msg, nullptr, 0, 0) > 0))
		{			
			TranslateMessage(&msg);
			DispatchMessageA(&msg);
		}
		return msg.wParam;
}

// DZIALA 