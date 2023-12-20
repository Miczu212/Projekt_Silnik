#include <windows.h>
#include<optional>
#include<sstream>
#include"Error.h"
#include"Input.h"
#include"Grafika.h"
#include<memory>
#define CHWND_EXCEPT(hr) Okno::oErrorexc(__LINE__,__FILE__,hr)	//Quality of life
#define CHWND_LAST_EXCEPT() Okno::oErrorexc(__LINE__,__FILE__,GetLastError())	//Quality of life
class Okno
{
public:
	class oErrorexc : public Errorexc
	{
	public:
		oErrorexc(int line, const char* file, HRESULT hr);
		const char * Result();
		virtual const char* ErrorType();
		static std::string TranslateErrorCode(HRESULT hr);
		HRESULT GetErrorCode();
		std::string GetErrorDescript();
		
	private:
		HRESULT hr;

	};
	Okno(int szerokosc, int wysokosc, const char* nazwa);
	~Okno();	
	Grafika& grafika();
private:
	static LRESULT CALLBACK WindowProcred(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept;
	static LRESULT CALLBACK WindowprocSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept;
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept ;	//handle do okna, unsinged int, liczby,liczby //mesage handling
private:
	int szerokosc;
	int	wysokosc;
	HWND hwnd;
	const char* wnazwa = "Okno1"; //to nie jest nazwa pokazywana na gorze tylko nazwa okna 
	HINSTANCE hInstance; //handle do instancji okna 
	std::unique_ptr<Grafika> pGrafika;
public:
	void Merge();
	Myszka Mk;
	Klawiatura Klt;
	HWND *pHwnd = &hwnd; //publiczny jest pointer do handla, zrobione w celu zabezpieczenia handla

};
