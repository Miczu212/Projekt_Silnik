#include "Mainapp.h"

// DZIALA 


Mainapp::Mainapp()
	: 
	WND1(ScreenWidth, ScreenHeight, "testing") // ustawianie parametrÛw okna
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
	//Poruszanie sie po tablicy textur
	ISPressed(KEY_R)
	{
		TextureCounter++;
		if (TextureCounter > TextureHolder.size() - 1)
			TextureCounter = 0;
		
	}
	//Wczytanie Textury
	ISPressed(KEY_F)
	{	
		LoadFileTypeTexture();
	}
	//Skalowanie textury zmniejszenie
	ISPressed(KEY_N)
	{
		ScaleTwidth -= 10;
		ScaleTheight -= 10;
		
	}
	//Skalowanie textury zwiekszenie
	ISPressed(KEY_M)
	{
		ScaleTwidth += 10;
		ScaleTheight += 10;
		
	}
	// Reset do domyslnych width i height
	ISPressed(KEY_B)
	{
		ScaleTwidth = 0;
		ScaleTheight = 0;
		
	}
	//Zapisanie Poziomu
	ISPressed(KEY_Z)
	{ 
		SaveFileTypeLevel();
	}
	//Wczytanie Poziomu
	ISPressed(KEY_L)
	{
		LoadFileTypeLevel();
	}
	//Sterowanie
	ISPressed(KEY_LEFT)
	{
		CameraXPosition = CameraSpeed;
		CameraXState = true;
		
	}
	ISPressed(KEY_RIGHT)
	{
		CameraXPosition = -CameraSpeed;
		CameraXState = true;
		
	}
	ISPressed(KEY_DOWN)
	{
		CameraYPosition = -CameraSpeed;
		CameraYState = true;
		
	}
	ISPressed(KEY_UP)
	{
		CameraYPosition = CameraSpeed;
		CameraYState = true;
		
	}
	//Sterowanie
	//Ustawienie Gracza na obecnie Wybranπ texture
	ISPressed(KEY_P)
	{
		CurrentPlayer.CurrentPlayerTexture = TextureHolder[TextureCounter];

		LoadBMPToTexture(
			CurrentPlayer.CurrentPlayerTexture.Path,
			WND1.ReturnGFX().ReturnRenderTarget(),
			CurrentPlayer.CurrentPlayerTexture.pBitmap.GetAddressOf()
		);
		CurrentPlayer.PlayerRect = D2D1::RectF(
			ScreenWidth / 2 - (CurrentPlayer.CurrentPlayerTexture.Twidth + ScaleTwidth) / 2,
			ScreenHeight / 2 - (CurrentPlayer.CurrentPlayerTexture.Theight + ScaleTheight) / 2,
			ScreenWidth / 2 + (CurrentPlayer.CurrentPlayerTexture.Twidth + ScaleTwidth) / 2,
			ScreenHeight / 2 + (CurrentPlayer.CurrentPlayerTexture.Theight + ScaleTheight) / 2
		);
		//zrobione tak by postac byla zawsze na srodku ekranu
		
	}
	//Wczytanie dzwiekow
	ISPressed(KEY_O)
	{
		LoadFileTypeAudio();
		
	}
	//Poruszanie sie po tablicy dzwiekow
	ISPressed(KEY_0)
	{
		AudioCounter++;

		if (AudioCounter >= AudioHolder.size())
			AudioCounter = 0;
		
	}
	//Odtworzenie dzwieku
	ISPressed(KEY_9)
	{
		AudioHolder[AudioCounter].Play(1.0f,1.0f);
	}
	//Zatrzymanie odtworzenia dzwieku
	ISPressed(KEY_8)
	{
		AudioHolder[AudioCounter].Stop();
	}
	//Prze≥πczenie Kolizji Dla Wybranej Textury
	ISPressed(KEY_Q)
	{
		TextureHolder[TextureCounter].IsCollisionOn = !TextureHolder[TextureCounter].IsCollisionOn;
	}
	ISPressed(KEY_W)
	{
		std::wstring selectedFilePath = OpenFileDialog(L"Binary Files", L"*.bin;*.dat");
		Currentlevel.ReTargetLevel(selectedFilePath);
	}
	WND1.Klt.ClearState();
}
//Funkcje Wczytujπce/Zapisujπce
void Mainapp::LoadFileTypeAudio()
{
	std::wstring selectedFilePath = OpenFileDialog(L"Audio Files", L"*.wav;*.mp3;*.ogg");
	if (!selectedFilePath.empty()) {
		CopyFileToProjectFolder(selectedFilePath);
		AudioCounter++;
		Sound s(selectedFilePath);
		AudioHolder.push_back(s);
		AudioPathHolder.push_back(selectedFilePath);
	}
}
void Mainapp::LoadFileTypeLevel()
{

	std::wstring selectedFilePath = OpenFileDialog(L"Binary Files", L"*.bin;*.dat");
	if (!selectedFilePath.empty())
	{
		czyrysowaclinie = false;
		AudioHolder.clear();
		TextureHolder.clear();
		AudioPathHolder.clear();
		Currentlevel.LoadLevel(TextureHolder,AudioPathHolder, selectedFilePath, CurrentPlayer);
		//textury
		LoadBMPToTexture(
			CurrentPlayer.CurrentPlayerTexture.Path,
			WND1.ReturnGFX().ReturnRenderTarget(),
			CurrentPlayer.CurrentPlayerTexture.pBitmap.GetAddressOf()
		);
		for (TextureCounter = 0; TextureCounter < TextureHolder.size(); TextureCounter++)
		{
			LoadBMPToTexture(TextureHolder[TextureCounter].Path,
				WND1.ReturnGFX().ReturnRenderTarget(),
				TextureHolder[TextureCounter].pBitmap.GetAddressOf());
			
		}
		TextureCounter = TextureHolder.size() - 1;
		//textury

		//audio
		AudioHolder.clear();
		
		
		for (auto& Path : AudioPathHolder)
		{
			if (!Path.empty())
			{
				Sound s(Path);
				AudioHolder.push_back(s);
				//po tym jak wyjdziemy ze scopa to destruktor dla s automatycznie siÍ uruchamia, a audioholder przechowuje kopie

			}
			
		}
		AudioCounter = AudioHolder.size() - 1;
		//audio
	}
}
void Mainapp::SaveFileTypeLevel()
{
	std::wstring selectedFilePath;
	Microsoft::WRL::ComPtr<IFileDialog> pFileDialog;
	HRESULT hr = CoCreateInstance(CLSID_FileSaveDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
	if (SUCCEEDED(hr))
	{
		COMDLG_FILTERSPEC fileTypes[] = { L"Binary Files", L"*.bin;*.dat" };
		pFileDialog->SetFileTypes(_countof(fileTypes), fileTypes);
		// Pokaø okno dialogowe
		if (SUCCEEDED(pFileDialog->Show(WND1.GetHandle())))
		{
			// Pobierz wybrany plik
			Microsoft::WRL::ComPtr<IShellItem> pShellItem;
			if (SUCCEEDED(pFileDialog->GetResult(&pShellItem)))
			{
				PWSTR pszFilePath;
				if (SUCCEEDED(pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath)))
				{
					selectedFilePath = pszFilePath;
					CoTaskMemFree(pszFilePath);
				}
			}
		}
	}
	Currentlevel.SaveLevel(TextureHolder,AudioPathHolder, selectedFilePath, CurrentPlayer);



}
void Mainapp::LoadFileTypeTexture()
{
	std::wstring CopiedPath = OpenFileDialog(L"Bitmap Files", L"*.bmp;*.png;*.jpg");
	if (!CopiedPath.empty()) {
		czyrysowaclinie = false;
		TextureCounter++;
		TextureInstance NewTextureInstance;
		TextureHolder.push_back(NewTextureInstance);
		LoadBMPToTexture(
			CopyFileToProjectFolder(CopiedPath),
			WND1.ReturnGFX().ReturnRenderTarget(),
			TextureHolder[TextureCounter].pBitmap.GetAddressOf()
		);
		TextureHolder[TextureCounter].Path = CopiedPath;

		//dostawanie wysokosci i szerokosci textury
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(TextureHolder[TextureCounter].Path.c_str());
		if (bitmap) {

			TextureHolder[TextureCounter].Twidth = bitmap->GetWidth();
			TextureHolder[TextureCounter].Theight = bitmap->GetHeight();
			delete bitmap;
		}
		else {
		}

		Gdiplus::GdiplusShutdown(gdiplusToken);

	}
}
//Funkcje Wczytujπce/Zapisujπce
void Mainapp::DoLogic() 
{
	if (timer.Peek() >= 1.0f / 30.0f) //logika bÍdzie sprawdzana co 1.0f/30.0f sekundy a nie co klatke, przez co dla wszystkich frameratÛw gra bedzie podobnie p≥ynna
	{
		timer.Mark();
		HandleInput();
	}
}
void Mainapp::DoDrawing()
{
	WND1.ReturnGFX().BeginFrame();
	WND1.ReturnGFX().ClearBuffer(0, 0, 0); // by wylaczyc tencze wstaw tu sta≥e
	NoAutoclickE(			//Ta funkcja jest tutaj nie w handle input dlatego øe jest bezpoúrednio zwiπzana z rysowaniem 
			if(czyrysowaclinie==true)
			WND1.ReturnGFX().Draw(MousePosition, MousePosition);
			if (TextureCounter != -1) {
				//TextureHolder[TextureCounter].TexturePointTab.push_back(MousePosition);		
				D2D1_RECT_F destinationRect = D2D1::RectF(WND1.Mk.GetPosX(), WND1.Mk.GetPosY(), TextureHolder[TextureCounter].Twidth + WND1.Mk.GetPosX() + ScaleTwidth,
					TextureHolder[TextureCounter].Theight + WND1.Mk.GetPosY() + ScaleTheight);
				TextureHolder[TextureCounter].destinationRectTab.push_back(destinationRect);
			}	
		, else if (czyrysowaclinie == true) //else if jest po przecinku z powodu struktury makra
			WND1.ReturnGFX().Draw(MousePosition);
	)
			

		if (TextureHolder.size()!= 0)
		{
			UpdateCameraPosition();
			for (auto& texture : TextureHolder)
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

			if (TextureHolder[TextureCounter].pBitmap)
			{
				D2D1_RECT_F destinationRect = D2D1::RectF(WND1.Mk.GetPosX(), WND1.Mk.GetPosY(), TextureHolder[TextureCounter].Twidth + WND1.Mk.GetPosX() + ScaleTwidth,
					TextureHolder[TextureCounter].Theight + WND1.Mk.GetPosY() + ScaleTheight);
				WND1.ReturnGFX().ReturnRenderTarget()->DrawBitmap(TextureHolder[TextureCounter].pBitmap.Get(), destinationRect);
			}
			
		}
	if (CurrentPlayer.CurrentPlayerTexture.pBitmap) //jak textura gracza jest to rysuj
	{
		WND1.ReturnGFX().ReturnRenderTarget()->DrawBitmap(CurrentPlayer.CurrentPlayerTexture.pBitmap.Get(), CurrentPlayer.PlayerRect);
	}
		

	WND1.ReturnGFX().EndFrame();
}
void Mainapp::DoFrame() {
	/*std::ostringstream oss;
	oss << timer.Peek();
	SetWindowTextA(WND1.GetHandle(), oss.str().c_str());*/ //potrzebne do debugowania problemÛw z timerem
	DoLogic();
	DoDrawing();
	
	
}

bool Mainapp::IFColision(const D2D1_RECT_F& rect1, const D2D1_RECT_F& rect2) //jezeli prostokπty siÍ pokryjπ zwracana jest odpowiednia wartosc
{
	// Sprawdzamy warunki brzegowe, czyli czy jeden prostokπt jest po lewej, po prawej, nad lub pod drugim prostokπtem.
	if (rect1.right < rect2.left || rect1.left > rect2.right || rect1.bottom < rect2.top || rect1.top > rect2.bottom)
		return false;
	return true;
}
void Mainapp::UpdateCameraPosition()
{
	std::vector<TextureInstance> Rollback = TextureHolder;
	if (CameraXState || CameraYState)
	{
		for (auto& texture : TextureHolder)
		{
				for (int i = 0; i < texture.destinationRectTab.size(); i++)
				{
					if (CameraXState) {

						texture.destinationRectTab[i].left += CameraXPosition;
						texture.destinationRectTab[i].right += CameraXPosition;
						if (IFColision(CurrentPlayer.PlayerRect, texture.destinationRectTab[i]))
						{
							Collision = true;
							break;
						}

					}
					if (CameraYState) {
						texture.destinationRectTab[i].top += CameraYPosition;
						texture.destinationRectTab[i].bottom += CameraYPosition;
						if (IFColision(CurrentPlayer.PlayerRect, texture.destinationRectTab[i]))
						{
							Collision = true;
							break;
						}
					}
				}
			}
		
	if(Collision) //jezeli doszlo do kolizji, zawroc wszelkie zmiany do kamery
		TextureHolder = Rollback;

	for (auto& texture : TextureHolder) //Petla co sprawdza textury co majπ nie mieÊ kolizji
	{
		if (!texture.IsCollisionOn) { //czy textura ma kolizje jest przypisane do jakby obrazka nie do pojedynczego recta, dlatego jak chcemy mieÊ 2 rÛøne textury z kolizjπ i bez to po prostu
									 //bÍdzie trzeba wczytaÊ 2 textury i ustawiÊ im 2 rÛøne parametry
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
	}

		CameraXPosition = 0;
		CameraYPosition = 0;
		CameraXState = false;
		CameraYState = false;
		Collision = false;
	}
}


std::filesystem::path Mainapp::CopyFileToProjectFolder(const std::wstring& SourceFilePath)
{
	try {
		std::filesystem::path projectFolder = std::filesystem::current_path();
		std::filesystem::path sourcePath(SourceFilePath);
			// Uzyskaj pe≥nπ úcieøkÍ do pliku docelowego w folderze projektowym
		std::filesystem::path destinationPath = projectFolder / sourcePath.filename();
			// Skopiuj plik
		std::filesystem::copy_file(sourcePath, destinationPath, std::filesystem::copy_options::overwrite_existing);

		MessageBoxA(WND1.GetHandle(), "Plik skopiowany pomyúlnie.", NULL, MB_OK);
		return destinationPath;
	}
	catch(const std::exception& e)
	{
		MessageBoxA(WND1.GetHandle(), "B≥πd podczas kopiowania pliku: ", NULL, MB_OK);
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
				oss << e.GetPosX() << "," << e.GetPosY();			// do stringstreama dodajemy wspÛ≥rzÍdne gdzie lpm zosta≥ wciúniÍty
				MousePosition.x = e.GetPosX();
				MousePosition.y = e.GetPosY();

				SetWindowTextA(WND1.GetHandle(), oss.str().c_str());
			}
		}

	}

}

std::wstring Mainapp::OpenFileDialog(LPCWSTR Filetype, LPCWSTR FileExtension)
{
	std::wstring selectedFilePath;
	Microsoft::WRL::ComPtr<IFileDialog> pFileDialog;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
	if (SUCCEEDED(hr)) {
		// Ustawianie opcji dialogu, np. filtry plikÛw audio,pliki binarne itp.
		COMDLG_FILTERSPEC fileTypes[] = { Filetype, FileExtension };
		pFileDialog->SetFileTypes(_countof(fileTypes), fileTypes);

		// Pokazywanie okna dialogowego
		if (SUCCEEDED(pFileDialog->Show(nullptr))) {
			// Pobieranie wybranego pliku
			Microsoft::WRL::ComPtr<IShellItem> pShellItem;
			if (SUCCEEDED(pFileDialog->GetResult(&pShellItem))) {
				PWSTR pszFilePath;
				if (SUCCEEDED(pShellItem->GetDisplayName(SIGDN_FILESYSPATH, &pszFilePath))) {
					selectedFilePath = pszFilePath;
					CoTaskMemFree(pszFilePath);
				}
			}
		}
	}
	return selectedFilePath;
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
								// zamieniamy kluczowy kolor na przeüroczysty
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

