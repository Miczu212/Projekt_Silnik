#include "okno.h"
okno::okno(int szerokosc, int wysokosc, const char* nazwa) //definicja konstruktora
	:
	szerokosc(szerokosc),
	wysokosc(wysokosc),
	hInstance(GetModuleHandleA(nullptr))
{
	WNDCLASS winclass = {}; //potrzebne by zarejestrowac okno
		winclass.lpfnWndProc = HandleMsgSetup; //wskazniki
		winclass.hInstance = hInstance;//zrozumiale przez sie
		winclass.lpszClassName = wnazwa;//zrozumiale przez sie
	
	RegisterClassA(&winclass); 
		
	RECT winRect; //polozenie okna
		winRect.left = 100;
		winRect.right = szerokosc + winRect.left;
		winRect.top = 100;
		winRect.bottom = wysokosc + winRect.top;

	AdjustWindowRect(&winRect, WS_CAPTION | WS_MINIMIZEBOX|WS_MAXIMIZEBOX | WS_SYSMENU, FALSE); //cos jak rejestracja winclass tylko dla recta

	hwnd = CreateWindowA(wnazwa, nazwa, WS_CAPTION | WS_MINIMIZEBOX |WS_MAXIMIZEBOX  | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
		 winRect.right - winRect.left,winRect.bottom - winRect.top, nullptr, nullptr, hInstance, this);// dokumentacja jest na stronie microsoftu 
	
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	UpdateWindow(hwnd);
}

okno::~okno() //definicja destruktora
{
	UnregisterClass(wnazwa, hInstance);
	DestroyWindow(hwnd);
}
  LRESULT  okno::HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept	//jest tylko za pierwszym razym uruchomiane
{
	if (msg == WM_NCCREATE)	//wszystko to jest ustawiane tylko za pierwszym razem zeby zdobyc potrzebny pointer 
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lparam);	//wyciagniecie pointera
		okno* const pWnd = static_cast<okno*>(pCreate->lpCreateParams);		//przygotowanie do zalokowania pointera
		SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd)); //zalokowanie pointera do windows api
		SetWindowLongPtrA(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&okno::HandleMsgThunk));	//zmienienie funkcji handlujacej wiadomosci
		return pWnd->WindowProc(hwnd, msg, wParam, lparam);//zmienienie funkcji handlujacej wiadomosci
	}
	else
			return DefWindowProcA(hwnd, msg, wParam, lparam);

}
 LRESULT  okno::HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept {
	okno* const pWnd = reinterpret_cast<okno*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA)); //odzyskanie pointera zestorowanego w windows api
		return pWnd->WindowProc(hwnd, msg, wParam, lparam); //przekazanie wiadomosci do aktualnej funkcji przetwarzania wiadomosci
}

 LRESULT  okno::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept //Przetwarzanie Wiadomosci
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYDOWN:
		if (wParam == 'Q') //wParam musi byc przypisane do Du¿ych liter ale dzia³a równie¿ na ma³e je¿eli chcia³bym rozró¿niæ du¿e i ma³e litery
							//musialbym uzyc WM_CHAR
		{
			SetWindowTextA(hwnd, "testowa_zmiana_nazwy_okna");
		}
		break;
	case WM_KEYUP:
		if (wParam == 'Q') 
		{
			SetWindowTextA(hwnd, "Zmiana_Wykonana_pomyslnie");
		}
		break;
	case WM_LBUTTONDOWN:
		const POINTS points = MAKEPOINTS(lparam);
		std::ostringstream oss;
		oss << points.x << "," << points.y;			// do stringstreama dodajemy wspó³rzêdne gdzie lpm zosta³ wciœniêty
	}
	return DefWindowProcA(hwnd, msg, wParam, lparam);
}