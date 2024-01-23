#include "Mainapp.h"

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
	if (ISPressed(KEY_R))
	{
		TextureInstanceTabCounter++;
		if (TextureInstanceTabCounter > TextureInstanceTab.size() - 1)
			TextureInstanceTabCounter = 0;

	//	TextureInstanceTab[TextureInstanceTabCounter].pBitmap.Reset();  // Zwalnianie poprzedniego zasobu. 
	//	LoadBMPToTexture(TextureInstanceTab[TextureInstanceTabCounter].GetPath(), WND1.ReturnGFX().ReturnRenderTarget(), TextureInstanceTab[TextureInstanceTabCounter].pBitmap.GetAddressOf());
		WND1.Klt.ClearState();
	}
	//Wczytanie Textury
	if (ISPressed(KEY_F))
	{
		czyrysowaclinie = false;
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
	if (ISPressed(KEY_Q))
	{
		SetWindowTextA(WND1.GetHandle(), "jak naciskam Q to sie zmienia na ta");
		WND1.Klt.ClearState(); // wymagane, inaczej przycisk jest uznawany za "Wiecznie wcisniety"
	}
	//skalowanie textury zmniejszenie
	if (ISPressed(KEY_N))
	{
		ScaleTwidth -= 10;
		ScaleTheight -= 10;
		WND1.Klt.ClearState();
	}
	//skalowanie textury zwiekszenie
	if (ISPressed(KEY_M))
	{
		ScaleTwidth += 10;
		ScaleTheight += 10;
		WND1.Klt.ClearState();
	}
	// Reset do domyslnych width i height
	if (ISPressed(KEY_B))
	{
		ScaleTwidth = 0;
		ScaleTheight = 0;
		WND1.Klt.ClearState();
	}
	if (ISPressed(KEY_Z)) //TODO ZROBIC SYSTEM WCZYTYWANIA I ZAPISYWANIA LEVELI
	{
		std::wstring selectedFilePath;
		Microsoft::WRL::ComPtr<IFileDialog> pFileDialog;
		HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

		if (SUCCEEDED(hr))
		{
			// Ustaw opcje dialogu, np. filtry plików
			COMDLG_FILTERSPEC fileTypes[] = { L"Binary Files", L"*.bin;*.dat" };
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
						// Konwertuj na std::wstring lub std::string
						selectedFilePath = pszFilePath;
						// Zwolnij pamiêæ
						CoTaskMemFree(pszFilePath);
					}
				}
			}
		}
			Currentlevel.SaveLevel(TextureInstanceTab, selectedFilePath);
		
	
		WND1.Klt.ClearState();
	}
	if (ISPressed(KEY_L))
	{
		
		std::wstring selectedFilePath;
		Microsoft::WRL::ComPtr<IFileDialog> pFileDialog;
		HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

		if (SUCCEEDED(hr))
		{
			// Ustaw opcje dialogu, np. filtry plików
			COMDLG_FILTERSPEC fileTypes[] = { L"Binary Files", L"*.bin;*.dat" };
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
						// Konwertuj na std::wstring lub std::string
						selectedFilePath = pszFilePath;
						// Zwolnij pamiêæ
						CoTaskMemFree(pszFilePath);

					}


				}
			}


		}
		Currentlevel.LoadLevel(TextureInstanceTab, selectedFilePath);
		for (int i = 0; i < TextureInstanceTab.size(); i++)
		{
			LoadBMPToTexture(TextureInstanceTab[i].PATHTest,
				WND1.ReturnGFX().ReturnRenderTarget(),
				TextureInstanceTab[i].pBitmap.GetAddressOf());
		}
		
		WND1.Klt.ClearState();
	}
	if (ISPressed(KEY_LEFT))
	{
		CameraXPosition=10;
		CameraXState = true;
		WND1.Klt.ClearState();
	}
	if (ISPressed(KEY_RIGHT))
	{
		CameraXPosition = -10;
		CameraXState = true;
		WND1.Klt.ClearState();
	}
	if (ISPressed(KEY_DOWN))
	{
		CameraYPosition = -10;
		CameraYState = true;
		WND1.Klt.ClearState();
	}
	if (ISPressed(KEY_UP))
	{
		CameraYPosition = 10;
		CameraYState = true;
		WND1.Klt.ClearState();
	}
	if (ISPressed(KEY_P))
	{
		PlayerTexture = TextureInstanceTab[TextureInstanceTabCounter];

		PlayerRect = D2D1::RectF(
			ScreenWidth / 2 - (PlayerTexture.Twidth + ScaleTwidth) / 2,
			ScreenHeight / 2 - (PlayerTexture.Theight + ScaleTheight) / 2,
			ScreenWidth / 2 + (PlayerTexture.Twidth + ScaleTwidth) / 2,
			ScreenHeight / 2 + (PlayerTexture.Theight + ScaleTheight) / 2
		); //zrobione tak by postac byla zawsze na srodku ekranu

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
	NoAutoclick(			//Ta funkcja jest tutaj nie w handle input dlatego ¿e jest bezpoœrednio zwi¹zana z rysowaniem 
		if (WND1.Mk.LeftIsPressed() == true) {
			WND1.CurrentMouseState = true;
			if(czyrysowaclinie==true)
			WND1.ReturnGFX().Draw(MousePosition, MousePosition);
			if (TextureInstanceTabCounter != -1) {
				//TextureInstanceTab[TextureInstanceTabCounter].TexturePointTab.push_back(MousePosition);		
				D2D1_RECT_F destinationRect = D2D1::RectF(WND1.Mk.GetPosX(), WND1.Mk.GetPosY(), TextureInstanceTab[TextureInstanceTabCounter].Twidth + WND1.Mk.GetPosX() + ScaleTwidth,
					TextureInstanceTab[TextureInstanceTabCounter].Theight + WND1.Mk.GetPosY() + ScaleTheight);
				TextureInstanceTab[TextureInstanceTabCounter].destinationRectTab.push_back(destinationRect);
			}
		}
		else if (czyrysowaclinie == true)
			WND1.ReturnGFX().Draw(MousePosition);
	)
		if (TextureInstanceTab.size()!= 0)
		{
			UpdateCameraPosition();
			for (auto& texture : TextureInstanceTab)
			{
				if (texture.pBitmap)
				{
					// Rysuj wszystkie instancje tekstur na ekranie
					for (auto& destinationRect : texture.destinationRectTab)
					{
						WND1.ReturnGFX().ReturnRenderTarget()->DrawBitmap(texture.pBitmap.Get(), destinationRect);
					}
				}
			}

			D2D1_RECT_F destinationRect = D2D1::RectF(WND1.Mk.GetPosX(), WND1.Mk.GetPosY(), TextureInstanceTab[TextureInstanceTabCounter].Twidth + WND1.Mk.GetPosX() + ScaleTwidth,
				TextureInstanceTab[TextureInstanceTabCounter].Theight + WND1.Mk.GetPosY() + ScaleTheight);
			WND1.ReturnGFX().ReturnRenderTarget()->DrawBitmap(TextureInstanceTab[TextureInstanceTabCounter].pBitmap.Get(), destinationRect);
		}
	if(PlayerTexture.pBitmap) //jak textura gracza jest to rysuj
		WND1.ReturnGFX().ReturnRenderTarget()->DrawBitmap(PlayerTexture.pBitmap.Get(), PlayerRect);

	WND1.ReturnGFX().EndFrame();
}
void Mainapp::DoFrame() {

	DoLogic();
	DoDrawing();
	
}
void Mainapp::UpdateCameraPosition()
{
	if (CameraXState || CameraYState)
	{
		for (auto& texture : TextureInstanceTab)
		{
			for (int i = 0; i < texture.destinationRectTab.size(); i++)
			{
				if (CameraXState) {
					texture.destinationRectTab[i].left += CameraXPosition;
					texture.destinationRectTab[i].right += CameraXPosition;
				}
				if (CameraYState) {
					texture.destinationRectTab[i].top += CameraYPosition;
					texture.destinationRectTab[i].bottom += CameraYPosition;
				}
			}
		}
		CameraXPosition = 0;
		CameraYPosition = 0;
		CameraXState = false;
		CameraYState = false;
	}
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
						
						UINT width, height;
						hr = pConverter->GetSize(&width, &height);
						if (SUCCEEDED(hr))
						{
							
							std::vector<BYTE> pixelData(width * height * 4);

							
							WICRect rect = { 0, 0, static_cast<INT>(width), static_cast<INT>(height) };
							hr = pConverter->CopyPixels(&rect, width * 4, width * height * 4, pixelData.data());
							if (SUCCEEDED(hr))
							{
								// zamieniamy kluczowy kolor na przeŸroczysty
								for (UINT i = 0; i < width * height; ++i)
								{
									if (*reinterpret_cast<UINT32*>(&pixelData[i * 4]) == KeyColour)
									{
										*reinterpret_cast<UINT32*>(&pixelData[i * 4]) = 0; //alpha to 0
									}
								}

								D2D1_BITMAP_PROPERTIES bitmapProperties;
								bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
								bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
								bitmapProperties.dpiX = 96.0f;
								bitmapProperties.dpiY = 96.0f;

								//faktyczne tworzenie bitmapy
								hr = pRenderTarget->CreateBitmap(
									D2D1::SizeU(width, height),
									pixelData.data(),
									width * 4,
									&bitmapProperties,
									ppBitmap
								);
							}
						}
					}
				}
			}
		}
	}
}

