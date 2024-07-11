#pragma once
#include <windows.h>
#include<optional>
#include<sstream>
#include"Error.h"
#include"Input.h"
#include"GFX.h"
#include<memory>
#define CHWND_EXCEPT(hr) WND::oErrorException(__LINE__,__FILE__,hr)	//Quality of life
#define CHWND_LAST_EXCEPT() WND::oErrorException(__LINE__,__FILE__,GetLastError())	//Quality of life
class WND
{
public:
	class oErrorException : public Errorexc
	{
	public:
		oErrorException(int line, const char* file, HRESULT hr);
		const char* Result();
		virtual const char* ErrorType();
		static std::string TranslateErrorCode(HRESULT hr);
		HRESULT GetErrorCode();
		std::string GetErrorString();
	private:
		HRESULT hr;

	};
	WND(const char* nazwa);
	~WND();	
	GFX& ReturnGFX();
private:
	static LRESULT CALLBACK WindowProcred(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept;
	static LRESULT CALLBACK WindowprocSetup(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept;
	LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lparam) noexcept ;	//handle do okna, unsinged int, liczby,liczby //mesage handling
private:
	HWND hwnd;
	const char* wName = "WND1"; //to nie jest nazwa pokazywana na gorze tylko nazwa okna 
	HINSTANCE hInstance; //handle do instancji okna 
	std::unique_ptr<GFX> pGFX;
	bool fullscreen=true;
public:
	int Width;
	int	Height;
	void Merge();
	HWND GetHandle() const;
	Myszka Mk;
	Klawiatura Klt;
	bool CurrentMouseState=false; // Dodane w celu wy³¹czenia autoklika w myszce
	void ResizeWindow();


};
