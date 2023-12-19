#pragma once
#include<d3d11.h>
#pragma comment(lib,"d3d11.lib")
class Grafika
{
public:
	Grafika(HWND hwnd);
	Grafika(const Grafika&) = delete; //wprowadzone bo rule of 3/5
	Grafika& operator=(const Grafika&) = delete;//wprowadzone bo rule of 3/5
	~Grafika();//wprowadzone bo rule of 3/5
	void EndFrame();
	void ClearBuffer(float r, float g, float b);
private:
	ID3D11Device* pDev = nullptr;
	IDXGISwapChain* pSwap = nullptr;
	ID3D11DeviceContext* pDevContext = nullptr;
	ID3D11RenderTargetView* pTarget = nullptr;
};