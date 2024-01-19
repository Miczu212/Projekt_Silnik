#include "Mainapp.h"
#include<random>
#include<chrono>

// DZIALA 


Mainapp::Mainapp()
	: 
	WND1(ScreenWidth, ScreenHeight, "testing") // ustawianie parametrów okna
{}

int Mainapp::Go()
{
	while (true)
	{
		ProcessMessages();
		DoFrame();

	}
	return msg.wParam;
}
void Mainapp::HandleInput()
{
	//Zmiana miedzy texturami wyswietlanymi
	if (WND1.Klt.KeyIsPressed(KEY_R))
	{
		TextureInstanceTabCounter++;
		if (TextureInstanceTabCounter > TextureInstanceTab.size() - 1)
			TextureInstanceTabCounter = 0;

		TextureInstanceTab[TextureInstanceTabCounter].pBitmap.Reset();  // Zwalnianie poprzedniego zasobu.
		LoadBMPToTexture(TextureInstanceTab[TextureInstanceTabCounter].GetPath(), WND1.ReturnGFX().ReturnRenderTarget(), TextureInstanceTab[TextureInstanceTabCounter].pBitmap.GetAddressOf());
		WND1.Klt.ClearState();
	}
	//Wczytanie Textury
	if (WND1.Klt.KeyIsPressed(KEY_F))
	{
		std::filesystem::path CopiedPath;

		Microsoft::WRL::ComPtr<IFileDialog> pFileDialog;
		HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

		if (SUCCEEDED(hr))
		{
			// Ustaw opcje dialogu, np. filtry plików
			COMDLG_FILTERSPEC fileTypes[] = { L"Bitmap Files", L"*.bmp;*.png;*.jpg" };
			pFileDialog->SetFileTypes(_countof(fileTypes), fileTypes);

			// Poka¿ okno dialogowe
			if (SUCCEEDED(pFileDialog->Show(WND1.GetHandle())))
			{
				// Pobierz wybrany plik
				Microsoft::WRL::ComPtr<IShellItem> pShellItem;
				if (SUCCEEDED(pFileDialog->GetResult(&pShellItem)))
				{
					PWSTR pszFilePath;
					if (SUCCEEDED(pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)))
					{
						TextureInstanceTabCounter++;
						// Konwertuj na std::wstring lub std::string
						std::wstring selectedFilePath = pszFilePath;
						TextureInstance NewTextureInstance;
						TextureInstanceTab.push_back(NewTextureInstance);
						// Mo¿esz tak¿e skopiowaæ plik do folderu projektu itp.

						LoadBMPToTexture(
							CopiedPath = CopyBitmapToProjectFolder(selectedFilePath),
							WND1.ReturnGFX().ReturnRenderTarget(),
							TextureInstanceTab[TextureInstanceTabCounter].pBitmap.GetAddressOf()
						);
						TextureInstanceTab[TextureInstanceTabCounter].SetPath(CopiedPath);
						TextureInstanceTab[TextureInstanceTabCounter].PATHTest=CopiedPath;
						// Zwolnij pamiêæ
						CoTaskMemFree(pszFilePath);



						//dostawanie wysokosci i szerokosci textury
						Gdiplus::GdiplusStartupInput gdiplusStartupInput;
						ULONG_PTR gdiplusToken;
						Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
						Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(TextureInstanceTab[TextureInstanceTabCounter].GetPath().c_str());
						if (bitmap) {

							TextureInstanceTab[TextureInstanceTabCounter].Twidth = bitmap->GetWidth();
							TextureInstanceTab[TextureInstanceTabCounter].Theight = bitmap->GetHeight();
							delete bitmap;
						}
						else {
						}

						Gdiplus::GdiplusShutdown(gdiplusToken);

					}


				}
			}


		}

		WND1.Klt.ClearState();
	}
	//zmiana tytulu okna
	if (WND1.Klt.KeyIsPressed(KEY_Q))
	{
		SetWindowTextA(WND1.GetHandle(), "jak naciskam Q to sie zmienia na ta");
		WND1.Klt.ClearState(); // wymagane, inaczej przycisk jest uznawany za "Wiecznie wcisniety"
	}
	//skalowanie textury zmniejszenie
	if (WND1.Klt.KeyIsPressed(KEY_N))
	{
		ScaleTwidth -= 10;
		ScaleTheight -= 10;
		WND1.Klt.ClearState();
	}
	//skalowanie textury zwiekszenie
	if (WND1.Klt.KeyIsPressed(KEY_M))
	{
		ScaleTwidth += 10;
		ScaleTheight += 10;
		WND1.Klt.ClearState();
	}
	// Reset do domyslnych width i height
	if (WND1.Klt.KeyIsPressed(KEY_B))
	{
		ScaleTwidth = 0;
		ScaleTheight = 0;
		WND1.Klt.ClearState();
	}
	if (WND1.Klt.KeyIsPressed(KEY_Z)) //TODO ZROBIC SYSTEM WCZYTYWANIA I ZAPISYWANIA LEVELI
	{
		Currentlevel.SaveLevel(TextureInstanceTab[TextureInstanceTabCounter],"Level");
		WND1.Klt.ClearState();
	}
	if (WND1.Klt.KeyIsPressed(KEY_L))
	{
		Currentlevel.LoadLevel(TextureInstanceTab[TextureInstanceTabCounter], "Level");
		TextureInstanceTab[TextureInstanceTabCounter].pBitmap.Reset();
		LoadBMPToTexture(TextureInstanceTab[TextureInstanceTabCounter].PATHTest,
			WND1.ReturnGFX().ReturnRenderTarget(),
			TextureInstanceTab[TextureInstanceTabCounter].pBitmap.GetAddressOf());
		WND1.Klt.ClearState();
	}
}
void Mainapp::DoLogic() 
{
	HandleInput();
	
}
void Mainapp::DoDrawing()
{
	WND1.ReturnGFX().BeginFrame();
	WND1.ReturnGFX().ClearBuffer(0, 0, 0); // by wylaczyc tencze wstaw tu sta³e
	if (WND1.CurrentMouseState == false) {
		if (WND1.Mk.LeftIsPressed() == true) {
			WND1.CurrentMouseState = true;
			WND1.ReturnGFX().Draw(MousePosition, MousePosition);
			if (TextureInstanceTabCounter != -1) {
				//TextureInstanceTab[TextureInstanceTabCounter].TexturePointTab.push_back(MousePosition);		
				D2D1_RECT_F destinationRect = D2D1::RectF(WND1.Mk.GetPosX(), WND1.Mk.GetPosY(), TextureInstanceTab[TextureInstanceTabCounter].Twidth + WND1.Mk.GetPosX() + ScaleTwidth,
					TextureInstanceTab[TextureInstanceTabCounter].Theight + WND1.Mk.GetPosY() + ScaleTheight);

				TextureInstanceTab[TextureInstanceTabCounter].destinationRectTab.push_back(destinationRect);
			}
			Sleep(100);
		}

		else
			WND1.ReturnGFX().Draw(MousePosition);
	}
	if (TextureInstanceTab.size() != 0)
	{
		if (TextureInstanceTab[TextureInstanceTabCounter].pBitmap)
		{
			WND1.ReturnGFX().ReturnRenderTarget()->Clear();
			for (int i = 0; i < TextureInstanceTab[TextureInstanceTabCounter].destinationRectTab.size(); i++)
			{
				WND1.ReturnGFX().ReturnRenderTarget()->DrawBitmap(TextureInstanceTab[TextureInstanceTabCounter].pBitmap.Get(), TextureInstanceTab[TextureInstanceTabCounter].destinationRectTab[i]); //rysowanie bitmap

			}
			D2D1_RECT_F destinationRect = D2D1::RectF(WND1.Mk.GetPosX(), WND1.Mk.GetPosY(), TextureInstanceTab[TextureInstanceTabCounter].Twidth + WND1.Mk.GetPosX() + ScaleTwidth,
				TextureInstanceTab[TextureInstanceTabCounter].Theight + WND1.Mk.GetPosY() + ScaleTheight);
			WND1.ReturnGFX().ReturnRenderTarget()->DrawBitmap(TextureInstanceTab[TextureInstanceTabCounter].pBitmap.Get(), destinationRect); //rysowanie bitmap
		}


	}


	WND1.ReturnGFX().EndFrame();
}
void Mainapp::DoFrame() {

	DoLogic();
	DoDrawing();
	
}

std::filesystem::path Mainapp::CopyBitmapToProjectFolder(const std::wstring& SourceFilePath)
{
	try {
		std::filesystem::path projectFolder = std::filesystem::current_path();
		std::filesystem::path sourcePath(SourceFilePath);
			// Uzyskaj pe³n¹ œcie¿kê do pliku docelowego w folderze projektowym
		std::filesystem::path destinationPath = projectFolder / sourcePath.filename();
			// Skopiuj plik
		std::filesystem::copy_file(sourcePath, destinationPath, std::filesystem::copy_options::overwrite_existing);

		MessageBoxA(WND1.GetHandle(), "Plik skopiowany pomyœlnie.", NULL, MB_OK);
		return destinationPath;
	}
	catch(const std::exception& e)
	{
		MessageBoxA(WND1.GetHandle(), "B³¹d podczas kopiowania pliku: ", NULL, MB_OK);
	}
}

void Mainapp::ProcessMessages()
{
	while ((result = PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE) > 0)) //petla obslugujaca wiadomosci przychodzace
	{
		if (msg.message == WM_QUIT)
		{
			exit(0);
		}
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
		//WND1.grafika().Draw();
		while (!WND1.Mk.IsEmpty())
		{
			const auto e = WND1.Mk.Read();
			if (e.GetType() == Myszka::Event::Type::Move)
			{
				std::ostringstream oss;
				oss << e.GetPosX() << "," << e.GetPosY();			// do stringstreama dodajemy wspó³rzêdne gdzie lpm zosta³ wciœniêty
				MousePosition.x = e.GetPosX();
				MousePosition.y = e.GetPosY();

				SetWindowTextA(WND1.GetHandle(), oss.str().c_str());
			}
		}

	}

}
void Mainapp::LoadBMPToTexture(const std::wstring& filePath, Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, ID2D1Bitmap** ppBitmap)
{
	Microsoft::WRL::ComPtr<IWICImagingFactory> pWICFactory;
	Microsoft::WRL::ComPtr<IWICBitmapDecoder> pDecoder;
	Microsoft::WRL::ComPtr<IWICBitmapFrameDecode> pFrame;
	Microsoft::WRL::ComPtr<IWICFormatConverter> pConverter;
	HRESULT hr = CoCreateInstance(CLSID_WICImagingFactory, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pWICFactory));
	if (SUCCEEDED(hr))
	{
		hr = pWICFactory->CreateDecoderFromFilename(filePath.c_str(), nullptr, GENERIC_READ, WICDecodeMetadataCacheOnLoad, &pDecoder);
		if (SUCCEEDED(hr))
		{
			hr = pDecoder->GetFrame(0, &pFrame);
			if (SUCCEEDED(hr))
			{
				hr = pWICFactory->CreateFormatConverter(&pConverter);
				if (SUCCEEDED(hr))
				{
					hr = pConverter->Initialize(pFrame.Get(), GUID_WICPixelFormat32bppPBGRA, WICBitmapDitherTypeNone, nullptr, 0.0, WICBitmapPaletteTypeMedianCut);
					if (SUCCEEDED(hr))
					{
						hr = pRenderTarget->CreateBitmapFromWicBitmap(pConverter.Get(), nullptr, ppBitmap);
					}
				}
			}
		}
	}
}
