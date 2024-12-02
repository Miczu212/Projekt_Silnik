#include "WND.h"
WND::WND(const char* nazwa) //definicja konstruktora
{
	this->hInstance = GetModuleHandleA(nullptr);

	WNDCLASS winclass = {}; //potrzebne by zarejestrowac WND
		winclass.lpfnWndProc = WindowprocSetup; //wskazniki
		winclass.hInstance = hInstance;//zrozumiale przez sie
		winclass.lpszClassName = wName;//zrozumiale przez sie
	
	RegisterClassA(&winclass); 
	Width = GetSystemMetrics(SM_CXSCREEN);
	Height = GetSystemMetrics(SM_CYSCREEN);
	
	RECT winRect; //polozenie okna
		winRect.left = 0;
		winRect.right = Width + winRect.left;
		winRect.top = 0;
		winRect.bottom = Height + winRect.top;
		
		if (		//error handling
			AdjustWindowRect(&winRect, WS_POPUP, FALSE)==0) //cos jak rejestracja winclass tylko dla recta //
		{
			throw CHWND_LAST_EXCEPT();
		}
	hwnd = CreateWindowA(wName, nazwa, WS_POPUP, CW_USEDEFAULT, CW_USEDEFAULT,
		 winRect.right - winRect.left,winRect.bottom - winRect.top, nullptr, nullptr, hInstance, this);// dokumentacja jest na stronie microsoftu 
	if (hwnd == nullptr)
	{
		throw CHWND_LAST_EXCEPT();
	}
	ShowWindow(hwnd, SW_SHOWDEFAULT);
	pGFX = std::make_unique<GFX>(hwnd, winRect);
	pGFX->ScreenHeight = Height;
	pGFX->ScreenWidth = Width;
	UpdateWindow(hwnd);
}

WND::~WND() //definicja destruktora
{

	UnregisterClass(wName, hInstance);
	DestroyWindow(hwnd);
	if (wName != nullptr)
	{
		wName = nullptr;
	}
	
}

GFX& WND::ReturnGFX()
{
	return *pGFX;
}
  LRESULT  WND::WindowprocSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept	//jest tylko za pierwszym razym uruchomiane
{
	if (msg == WM_NCCREATE)	//wszystko to jest ustawiane tylko za pierwszym razem zeby zdobyc potrzebny pointer 
	{
		const CREATESTRUCTW* const pCreate = reinterpret_cast<CREATESTRUCTW*>(lparam);	//wyciagniecie pointera
		WND* const pWnd = static_cast<WND*>(pCreate->lpCreateParams);		//przygotowanie do zalokowania pointera
		SetWindowLongPtrA(hwnd, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(pWnd)); //zalokowanie pointera do windows api
		SetWindowLongPtrA(hwnd, GWLP_WNDPROC, reinterpret_cast<LONG_PTR>(&WND::WindowProcred));	//zmienienie funkcji handlujacej wiadomosci
		return pWnd->WindowProc(hwnd, msg, wParam, lparam);//zmienienie funkcji handlujacej wiadomosci
	}
	else
			return DefWindowProcA(hwnd, msg, wParam, lparam);

}
 LRESULT  WND::WindowProcred(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept {
	WND* const pWnd = reinterpret_cast<WND*>(GetWindowLongPtrA(hwnd, GWLP_USERDATA)); //odzyskanie pointera zestorowanego w windows api
		return pWnd->WindowProc(hwnd, msg, wParam, lparam); //przekazanie wiadomosci do aktualnej funkcji przetwarzania wiadomosci
}

 LRESULT  WND::WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept //Przetwarzanie Wiadomosci
{
	switch (msg)
	{	
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	case WM_KEYUP:
		Klt.OnKeyReleased(static_cast<unsigned char>(wParam));
		break;
	case WM_KEYDOWN:
		if (!(lparam & 0x40000000) || Klt.AutorepeatIsEnabled()) //Implementacja Autorepeata, jezeli chce uzyc gdzies autorepeata, to musze uzyc clearstate, jak nie to nie daje i sie repeatuje
		{
			Klt.OnKeyPressed(static_cast<unsigned char>(wParam));
		}
		
		break;
	case WM_CHAR:
		Klt.OnChar(static_cast<unsigned char>(wParam));
		break;
	case WM_KILLFOCUS:
	{
		Klt.ClearState(); // po wyjsciu z okna usuwa klawisze z queue zeby nie naciskaly sie poza obecnosci¹ w oknie
		break;
	}
	case WM_MOUSEMOVE:
	{
		POINTS points = MAKEPOINTS(lparam);
		Mk.OnMyszkaMove(points.x, points.y);
		break;
	}
		case WM_RBUTTONDOWN:
		{
			const POINTS points = MAKEPOINTS(lparam);
			Mk.OnRightPressed(points.x, points.y);
			break;
		}
		case WM_RBUTTONUP:
		{
			const POINTS points = MAKEPOINTS(lparam);
			Mk.OnRightReleased(points.x, points.y);
			CurrentMouseState = false;
			break;
		}

		case WM_LBUTTONUP:
		{
			const POINTS points = MAKEPOINTS(lparam);
			Mk.OnLeftReleased(points.x, points.y);
			CurrentMouseState = false;
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
		{
			const POINTS points = MAKEPOINTS(lparam);
			Mk.OnLeftPressed(points.x, points.y);
			break;
		}

		case WM_SIZE:
		{
			if (pGFX) {
				RECT rect;
				GetClientRect(hwnd, &rect);
				UINT width = rect.right - rect.left;
				UINT height = rect.bottom - rect.top;
				pGFX->Resize(width, height);
				UpdateWindow(hwnd);
			}
			break;
		}

	}
	
	return DefWindowProcA(hwnd, msg, wParam, lparam);

}

 HWND WND::GetHandle() const
 {
	 return hwnd;
 }

 void WND::ResizeWindow()
 {
	 fullscreen = !fullscreen;
	 RECT winRect; // po³o¿enie okna
	 
	 if (fullscreen)
	 {
		 Width = GetSystemMetrics(SM_CXSCREEN);
		 Height = GetSystemMetrics(SM_CYSCREEN);
		 winRect.left = 0;
		 winRect.right = Width;
		 winRect.top = 0;
		 winRect.bottom = Height;

		 // Ustawienie stylu na pe³noekranowy
		 DWORD style = GetWindowLong(hwnd, GWL_STYLE);
		 style &= ~WS_OVERLAPPEDWINDOW;
		 style |= WS_POPUP;
		 SetWindowLong(hwnd, GWL_STYLE, style);

		 // Ustawienie pozycji i rozmiaru okna
		 SetWindowPos(hwnd, HWND_TOPMOST, winRect.left, winRect.top,
			 winRect.right - winRect.left, winRect.bottom - winRect.top,
			 SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		 pGFX->ScreenHeight = Height;
		 pGFX->ScreenWidth = Width;
	 }
	 else
	 {
		 
		 // Przywrócenie trybu okienkoweg
		 winRect.left =0;
		 winRect.right = Width + winRect.left-50;
		 winRect.top = 50;
		 winRect.bottom = Height + winRect.top;

		 // Ponowne ustawienie stylu na okienkowy
		 DWORD style = GetWindowLong(hwnd, GWL_STYLE);
		 style &= ~WS_POPUP;
		 style |= WS_OVERLAPPEDWINDOW;
		 SetWindowLong(hwnd, GWL_STYLE, style);

		 // Dostosowanie okna do nowego rozmiaru
		 AdjustWindowRect(&winRect, WS_CAPTION | WS_MINIMIZEBOX | WS_SIZEBOX | WS_SYSMENU, FALSE);

		 // Ustawienie pozycji i rozmiaru okna
		 SetWindowPos(hwnd, nullptr, winRect.left, winRect.top,
			 winRect.right - winRect.left, winRect.bottom - winRect.top,
			 SWP_FRAMECHANGED | SWP_SHOWWINDOW);
		 pGFX->ScreenHeight = Height;
		 pGFX->ScreenWidth = Width;
	 }
 }

 WND::oErrorException::oErrorException(int line, const char* file, HRESULT hr) 
	 :
	 Errorexc(line, file) 
		
 {
	 this->hr = hr;
 }



 const char* WND::oErrorException::Result()
 {
	 std::ostringstream oss;
	 oss << ErrorType() << std::endl << "[Kod B³êdu] " << GetErrorCode() << std::endl << "[Opis] " << GetErrorString() << std::endl << OGString();
	 bufor = oss.str();
	 return bufor.c_str();
 }

const char* WND::oErrorException::ErrorType()
 {
	 return "B³¹d Okna";
 }

 std::string WND::oErrorException::TranslateErrorCode(HRESULT hr)
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

 HRESULT WND::oErrorException::GetErrorCode()
 {
	 return hr;
 }

 std::string WND::oErrorException::GetErrorString()
 {
	 return  TranslateErrorCode(hr);
 }