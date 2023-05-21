#include <windows.h>
#include<optional>
#include<sstream>
class okno
{
public:
	okno(int szerokosc, int wysokosc, const char* nazwa);
	~okno();	
private:
	static LRESULT CALLBACK HandleMsgThunk(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept;
	static LRESULT CALLBACK HandleMsgSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept;
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept ;	//handle do okna, unsinged int, liczby,liczby //mesage handling
private:
	int szerokosc;
	int	wysokosc;
	HWND hwnd;
	const char* wnazwa = "okno1"; //to nie jest nazwa pokazywana na gorze tylko nazwa okna 
	HINSTANCE hInstance; //handle do instancji okna 
};