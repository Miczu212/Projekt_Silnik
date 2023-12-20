#include "Grafika.h"

Grafika::Grafika(HWND hwnd,RECT recti)
{
	rect = recti;
	DXGI_SWAP_CHAIN_DESC sd = {};
	sd.BufferDesc.Width = 0;	
	sd.BufferDesc.Height = 0;
	sd.BufferDesc.Format = DXGI_FORMAT_B8G8R8A8_UNORM; //rozk³ad pixeli
	sd.BufferDesc.RefreshRate.Numerator = 0; 
	sd.BufferDesc.RefreshRate.Denominator = 0;
	sd.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
	sd.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED; // w przypadku gdy rzeczy sie nakladaj¹
	sd.SampleDesc.Count = AACountSetting;	//konfiguracja antialiasingu na razie wyl¹czony
	sd.SampleDesc.Quality = AAQualitySetting;	//konfiguracja antialiasingu na razie wyl¹czony
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.BufferCount = 1;
	sd.OutputWindow = hwnd;
	sd.Windowed = IsWindowed;
	sd.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	sd.Flags = 0;

	 result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, nullptr, 0, D3D11_SDK_VERSION, &sd, &pSwap, &pDev, nullptr, &pDevContext);
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
	pDevContext->ClearRenderTargetView(pTarget.Get(), color);
	pBackBuffer->Release();
}

void Grafika::Draw()
{
	struct Vertex	//definicja krawedzi
	{
		float x;
		float y;
	};
	const Vertex vertices[] = //implementacja krawedzi
	{
		{0.0f,0.0f},
		{0.5f,0.5f},
		{-0.3f,-0.5f},
		{0.0f,0.0f}, 

	};
	Microsoft::WRL::ComPtr<ID3D11Buffer> pMatrixBuffer;
		D3D11_BUFFER_DESC matrixBufferDesc = {};
		matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
		matrixBufferDesc.ByteWidth = sizeof(ConstantBuffer); // TODO zrobic constant buffer, zeby mo¿na by³o rysowaæ nie lokalnie w viewportcie a globalnie
		matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
		matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		matrixBufferDesc.MiscFlags = 0;
			result = pDev->CreateBuffer(&matrixBufferDesc, nullptr, &pMatrixBuffer);
			if (result != S_OK) {
				MessageBox(nullptr, "Wyst¹pi³ problem przy tworzeniu sta³ej bufory macierzy", "Error", MB_OK);
				exit(0);
			}
	Microsoft::WRL::ComPtr<ID3D11Buffer> pVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11VertexShader> pVertexShader;
	Microsoft::WRL::ComPtr<ID3DBlob> pBlob;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>pPixelShader;
	D3D11_VIEWPORT vp = {};
	vp.Width = rect.right - rect.left;
	vp.Height = rect.bottom - rect.top;
	vp.MinDepth = 0;
	vp.MaxDepth = 1;
	vp.TopLeftX = 0;
	vp.TopLeftY = 0;
	pDevContext->RSSetViewports(1u, &vp);

	D3D11_BUFFER_DESC bd = {};
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.CPUAccessFlags = 0u;
	bd.MiscFlags = 0u;
	bd.ByteWidth = sizeof(vertices);
	bd.StructureByteStride = sizeof(Vertex);
	D3D11_SUBRESOURCE_DATA sd = {};
	sd.pSysMem = vertices;
	pDev->CreateBuffer(&bd, &sd, &pVertexBuffer);
	const UINT stride = sizeof(Vertex);
	CONST UINT Offset = 0u;

		//wykorzystujemy tego samego bloba poniewaz po uzyciu go do utworzenia Vertex Shadera mozna go uwolnic , proces ten dzieje sie automatycznie poniewaz ComPtr
	pDevContext->IASetVertexBuffers(0u, 1u, pVertexBuffer.GetAddressOf(), &stride, &Offset);
		result = D3DReadFileToBlob(L"Shaderkrawedzi.cso", &pBlob);
		if (result != S_OK) {
			MessageBox(nullptr, "Wyst¹pi³ problem przy wczytywaniu Shadera", "Error", MB_OK);
		}
	pDev->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader);// utworzenie shaderów
		pDevContext->VSSetShader(pVertexShader.Get(), nullptr, 0u);// ustawienie shaderów

		Microsoft::WRL::ComPtr<ID3D11InputLayout> pInputLayout;
		const D3D11_INPUT_ELEMENT_DESC ied[] =
		{
			{"Position",0,DXGI_FORMAT_R32G32B32A32_FLOAT,0u,0u,D3D11_INPUT_PER_VERTEX_DATA,0u},
		};
		pDev->CreateInputLayout(ied, (UINT)std::size(ied), pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &pInputLayout);

	//wykorzystujemy tego samego bloba poniewaz po uzyciu go do utworzenia Vertex Shadera mozna go uwolnic , proces ten dzieje sie automatycznie poniewaz ComPtr
		 result = D3DReadFileToBlob(L"ShaderPixeli.cso", &pBlob);
		if (result != S_OK) {
			MessageBox(nullptr, "Wyst¹pi³ problem przy wczytywaniu Shadera", "Error", MB_OK);
		}
	pDev->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader); // utworzenie shaderów
		pDevContext->PSSetShader(pPixelShader.Get(), nullptr, 0u);// ustawienie shaderów




	pDevContext->IASetInputLayout(pInputLayout.Get());
	pDevContext->OMSetRenderTargets(1u, pTarget.GetAddressOf(), nullptr);
	pDevContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY::D3D10_PRIMITIVE_TOPOLOGY_LINELIST);

	pDevContext->Draw((UINT)std::size(vertices), 0u);
}