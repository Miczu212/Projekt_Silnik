#pragma once
#include<d3d11.h>
#include<d3dcompiler.h>
#include<memory>
#include <wrl.h>
#pragma comment(lib,"d3d11.lib")
#pragma comment(lib,"D3DCompiler.lib")
class Grafika
{
public:
	Grafika(HWND hwnd, RECT recti);
	Grafika(const Grafika&) = delete; //wprowadzone bo rule of 3/5
	Grafika& operator=(const Grafika&) = delete;//wprowadzone bo rule of 3/5
	~Grafika();//wprowadzone bo rule of 3/5
	void EndFrame();
	void ClearBuffer(float r, float g, float b);
	void Draw();
	int ScreenWidth, ScreenHeight;
private:
	RECT rect;
	Microsoft::WRL::ComPtr<ID3D11Device> pDev;
	Microsoft::WRL::ComPtr<IDXGISwapChain> pSwap;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> pDevContext;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> pTarget;
	HRESULT result;
	int AACountSetting = 1, AAQualitySetting = 0;
	bool IsWindowed = true;	
};