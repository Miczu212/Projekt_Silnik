#include "Okno.h"
Okno::Okno(int szerokosc, int wysokosc, const char* nazwa) //definicja konstruktora
{
	this->hInstance = GetModuleHandleA(nullptr);
	this->szerokosc = szerokosc;
	this->wysokosc = wysokosc;

	WNDCLASS winclass = {}; //potrzebne by zarejestrowac Okno
		winclass.lpfnWndProc = WindowprocSetup; //wskazniki
		winclass.hInstance = hInstance;//zrozumiale przez sie
		winclass.lpszClassName = wnazwa;//zrozumiale przez sie
	
	RegisterClassA(&winclass); 
		
	RECT winRect; //polozenie okna
		winRect.left = 100;
		winRect.right = szerokosc + winRect.left;
		winRect.top = 100;
		winRect.bottom = wysokosc + winRect.top;
		
		if (		//error handling
			AdjustWindowRect(&winRect, WS_CAPTION | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_SYSMENU, FALSE)==0) //cos jak rejestracja winclass tylko dla recta
		{
			throw CHWND_LAST_EXCEPT();
		}
	hwnd = CreateWindowA(wnazwa, nazwa, WS_CAPTION | WS_MINIMIZEBOX |WS_MAXIMIZEBOX  | WS_SYSMENU, CW_USEDEFAULT, CW_USEDEFAULT,
		 winRect.right - winRect.left,winRect.bottom - winRect.top, nullptr, nullptr, hInstance, this);// dokumentacja jest na stronie microsoftu 
	if (hwnd == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	pGrafika = std::make_unique<Grafika>(hwnd, winRect);
	pGrafika->ScreenHeight = wysokosc;
	pGrafika->ScreenWidth = szerokosc;
	UpdateWindow(hwnd);
}

Okno::~Okno() //definicja destruktora
{
	UnregisterClass(wnazwa, hInstance);
	DestroyWindow(hwnd);
}
Grafika& Okno::grafika()
{
	return *pGrafika;
}
  LRESULT  Okno::WindowprocSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept	//jest tylko za pierwszym razym uruchomiane
{
	if (msg == WM_NCCREATE)	//wszystko to jest ustawiane tylko za pierwszym razem zeby zdobyc potrzebny pointer 
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lparam);	//wyciagniecie pointera
		Okno* const pWnd = static_cast<Okno*>(pCreate->lpCreateParams);		//przygotowanie do zalokowania pointera
		SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd)); //zalokowanie pointera do windows api
		SetWindowLongPtrA(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&Okno::WindowProcred));	//zmienienie funkcji handlujacej wiadomosci
		return pWnd->WindowProc(hwnd, msg, wParam, lparam);//zmienienie funkcji handlujacej wiadomosci
	}
	else
			return DefWindowProcA(hwnd, msg, wParam, lparam);

}
 LRESULT  Okno::WindowProcred(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept {
	Okno* const pWnd = reinterpret_cast<Okno*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA)); //odzyskanie pointera zestorowanego w windows api
		return pWnd->WindowProc(hwnd, msg, wParam, lparam); //przekazanie wiadomosci do aktualnej funkcji przetwarzania wiadomosci
}

 LRESULT  Okno::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept //Przetwarzanie Wiadomosci
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYUP:
		Klt.OnKeyPressed(static_cast<unsigned char>(wParam));
		break;
	case WM_KEYDOWN:
		Klt.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_CHAR:
		Klt.OnChar(static_cast<unsigned char>(wParam));
		break;
	case WM_KILLFOCUS:
		Klt.ClearState(); // po wyjsciu z okna usuwa klawisze z queue zeby nie naciskaly sie poza obecnosci¹ w oknie
		break;
	case WM_MOUSEMOVE:
	{
		POINTS points = MAKEPOINTS(lparam);
		Mk.OnMyszkaMove(points.x, points.y);
		break;
	}
	case WM_RBUTTONDOWN:
	{
		const POINTS points = MAKEPOINTS(lparam);
		Mk.OnLeftPressed(points.x, points.y);
		break;
	}
	case WM_RBUTTONUP:
	{
		const POINTS points = MAKEPOINTS(lparam);
		Mk.OnRightReleased(points.x, points.y);
		break;
	}

	case WM_LBUTTONUP:
	{
		const POINTS points = MAKEPOINTS(lparam);
		Mk.OnLeftReleased(points.x, points.y);
		break;
	}
	case WM_MOUSEWHEEL:
	{
		const POINTS points = MAKEPOINTS(lparam);
		if (GET_WHEEL_DELTA_WPARAM(wParam) > 0)
		{
			Mk.OnWheelUp(points.x, points.y);
		}
		else if (GET_WHEEL_DELTA_WPARAM(wParam) < 0)
		{
			Mk.OnWheelDown(points.x, points.y);
		}
		break;
	}
	 //wParam musi byc przypisane do Du¿ych liter ale dzia³a równie¿ na ma³e je¿eli chcia³bym rozró¿niæ du¿e i ma³e litery
						//musialbym uzyc WM_CHAR

	case WM_LBUTTONDOWN:
		const POINTS points = MAKEPOINTS(lparam);
		Mk.OnLeftReleased(points.x, points.y);
		break;
	}
	return DefWindowProcA(hwnd, msg, wParam, lparam);

}

 Okno::oErrorException::oErrorException(int line, const char* file, HRESULT hr) 
	 :
	 Errorexc(line, file) 
		
 {
	 this->hr = hr;
 }



 const char* Okno::oErrorException::Result()
 {
	 std::ostringstream oss;
	 oss << ErrorType() << std::endl << "[Kod B³êdu] " << GetErrorCode() << std::endl << "[Opis] " << GetErrorString() << std::endl << OGString();
	 bufor = oss.str();
	 return bufor.c_str();
 }

const char* Okno::oErrorException::ErrorType()
 {
	 return "B³¹d Okna";
 }

 std::string Okno::oErrorException::TranslateErrorCode(HRESULT hr)
 {
	 char* msgbuff = nullptr;	//bufor na nasz¹ wiadomoœæ (wskaŸniki and stuff)
	 DWORD msglen = FormatMessageA(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS	//przerabianie wiadomoœci na d³ugoœæ
		 , nullptr, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), reinterpret_cast<LPSTR>(&msgbuff), 0, nullptr); 
	 if (msglen == 0)	//je¿eli d³ugoœæ wynosi 0 oznacza to ¿e nast¹pi³ problem i kod b³êdu nie zosta³ zidentyfikowany
	 {
		 return"Problem z znalezieniem kodu b³êdu";
	 }
	 std::string errorstring = msgbuff;
	 LocalFree(msgbuff);
	 return errorstring;
 }

 HRESULT Okno::oErrorException::GetErrorCode()
 {
	 return hr;
 }

 std::string Okno::oErrorException::GetErrorString()
 {
	 return  TranslateErrorCode(hr);
 }
