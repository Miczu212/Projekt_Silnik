#pragma once
#include<memory>
#include <wrl.h>
#include"Error.h"
#include<vector>
#include<d2d1.h>
#include"DXErr.h"
#include<DirectXMath.h>
class GFX
{
public:
	class gException : public Errorexc
	{
		using Errorexc::Errorexc;
	};
	class HrException : public gException
	{
	public:
		HrException(int line, const char* file, HRESULT hr);
		const char* Result() override;
		const char* ErrorType() override;
		HRESULT GetErrorCode();
		std::string GetErrorString();
		std::string GetErrorDescription();
	private:
		HRESULT hr;
	};
	class DeviceRemovedexc : public HrException
	{
		using HrException::HrException;
	public:
		const char* ErrorType() override;
	private:
	};
	GFX(HWND hwnd, RECT recti);
	GFX(const GFX&) = delete; //wprowadzone bo rule of 3/5
	GFX& operator=(const GFX&) = delete;//wprowadzone bo rule of 3/5
	~GFX();//wprowadzone bo rule of 3/5
	void EndFrame();
	void ClearBuffer(float r, float g, float b);
	int ScreenWidth, ScreenHeight;
	void BeginFrame();
	void Draw(D2D1_POINT_2F mousepoint1, D2D1_POINT_2F mousepoint2);
	void Draw(D2D1_POINT_2F mousepoint1);
	
private:
	//refaktor na 2d
	int ColorofLine = 0;
	Microsoft::WRL::ComPtr<ID2D1Factory> pFactory;
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRTarget;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> pBrush;
	D2D1_POINT_2F MousePoint;
	RECT rect;
	HRESULT result;
	int AACountSetting = 1, AAQualitySetting = 0;
	bool IsWindowed = true;	
};