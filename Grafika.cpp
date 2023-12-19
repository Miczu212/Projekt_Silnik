#include "Grafika.h"

Grafika::Grafika(HWND hwnd)
{
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;	
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; //rozk³ad pixeli
	sd.BufferDesc.RefreshRate.Numerator = 0; 
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // w przypadku gdy rzeczy sie nakladaj¹
	sd.SampleDesc.Count = 1;	//konfiguracja antialiasingu na razie wyl¹czony
	sd.SampleDesc.Quality = 0;	//konfiguracja antialiasingu na razie wyl¹czony
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = true;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	auto result= D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &pSwap, &pDev, nullptr, &pDevContext);
	if (result != S_OK) {
		MessageBox(nullptr, "Wyst¹pi³ problem przy tworzeniu okna", "Error", MB_OK);
		exit(0);
	}
}

Grafika::~Grafika()
// uwolnienie zasobów
{
	if (pDev != nullptr)
		pDev->Release();
	if (pDevContext != nullptr)
		pDevContext->Release();
	if (pSwap != nullptr)
		pSwap->Release();
	if(pTarget !=nullptr)
		pTarget->Release();
}

void Grafika::EndFrame()
{
	pSwap->Present(1u, 0u); // tutaj zrozumialem to tak ze UINT u1 odpowiada za frames per second i 1u to 60 2u to 30
}


void Grafika::ClearBuffer(float r, float g, float b)
{
	ID3D11Resource* pBackBuffer = nullptr;
	pSwap->GetBuffer(0, __uuidof(ID3D11Resource), reinterpret_cast<void**>(&pBackBuffer));
	pDev->CreateRenderTargetView(pBackBuffer, nullptr, &pTarget);
	const float color[] = { r,g,b,1.0f };
	pDevContext->ClearRenderTargetView(pTarget, color);
}
