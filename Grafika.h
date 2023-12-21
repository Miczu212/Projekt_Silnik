#pragma once
#include<memory>
#include <wrl.h>
#include"Error.h"
#include<vector>
#include<d2d1.h>
#include"DXErr.h"
#include<DirectXMath.h>
class Grafika
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
	Grafika(HWND hwnd, RECT recti);
	Grafika(const Grafika&) = delete; //wprowadzone bo rule of 3/5
	Grafika& operator=(const Grafika&) = delete;//wprowadzone bo rule of 3/5
	~Grafika();//wprowadzone bo rule of 3/5
	void EndFrame();
	void ClearBuffer(float r, float g, float b);
	void Draw();
	int ScreenWidth, ScreenHeight;
	void BeginFrame();
	void ENDFRAME();
	void Draw1();
	
private:
	//refaktor na 2d
	Microsoft::WRL::ComPtr<ID2D1Factory> pFactory;
	Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRTarget;
	struct ConstantBuffer
	{
		DirectX::XMMATRIX transformmatrix;
	};
	RECT rect;
	HRESULT result;
	int AACountSetting = 1, AAQualitySetting = 0;
	bool IsWindowed = true;	
};