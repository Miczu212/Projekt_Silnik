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
						TextureInstanceTab[TextureInstanceTabCounter].Path = CopiedPath;
						// Zwolnij pamiêæ
						CoTaskMemFree(pszFilePath);



						//dostawanie wysokosci i szerokosci textury
						Gdiplus::GdiplusStartupInput gdiplusStartupInput;
						ULONG_PTR gdiplusToken;
						Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
						Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(TextureInstanceTab[TextureInstanceTabCounter].Path.c_str());
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
		Currentlevel.SaveLevel(TextureInstanceTab, selectedFilePath, CurrentPlayer);


		WND1.Klt.ClearState();
	}
	if (ISPressed(KEY_L))
	{
		czyrysowaclinie = false;
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
		Currentlevel.LoadLevel(TextureInstanceTab, selectedFilePath, CurrentPlayer);
		LoadBMPToTexture(
			CurrentPlayer.CurrentPlayerTexture.Path,
			WND1.ReturnGFX().ReturnRenderTarget(),
			CurrentPlayer.CurrentPlayerTexture.pBitmap.GetAddressOf()
		);
		for (TextureInstanceTabCounter = 0; TextureInstanceTabCounter < TextureInstanceTab.size(); TextureInstanceTabCounter++)
		{
			LoadBMPToTexture(TextureInstanceTab[TextureInstanceTabCounter].Path,
				WND1.ReturnGFX().ReturnRenderTarget(),
				TextureInstanceTab[TextureInstanceTabCounter].pBitmap.GetAddressOf());
		}
		TextureInstanceTabCounter = 0;
		WND1.Klt.ClearState();
	}
	if (ISPressed(KEY_LEFT))
	{
		CameraXPosition = CameraSpeed;
		CameraXState = true;
		WND1.Klt.ClearState();
	}
	if (ISPressed(KEY_RIGHT))
	{
		CameraXPosition = -CameraSpeed;
		CameraXState = true;
		WND1.Klt.ClearState();
	}
	if (ISPressed(KEY_DOWN))
	{
		CameraYPosition = -CameraSpeed;
		CameraYState = true;
		WND1.Klt.ClearState();
	}
	if (ISPressed(KEY_UP))
	{
		CameraYPosition = CameraSpeed;
		CameraYState = true;
		WND1.Klt.ClearState();
	}
	if (ISPressed(KEY_P))
	{
		CurrentPlayer.CurrentPlayerTexture = TextureInstanceTab[TextureInstanceTabCounter];

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
		WND1.Klt.ClearState();
	}
	if (ISPressed(KEY_O))
	{
		LoadAudio();
	}
	if (ISPressed(KEY_0))
	{
		AudioCounter++;
		if (AudioCounter >= AudioHolder.size())
			AudioCounter = 0;
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

			if (TextureInstanceTab[TextureInstanceTabCounter].pBitmap)
			{
				D2D1_RECT_F destinationRect = D2D1::RectF(WND1.Mk.GetPosX(), WND1.Mk.GetPosY(), TextureInstanceTab[TextureInstanceTabCounter].Twidth + WND1.Mk.GetPosX() + ScaleTwidth,
					TextureInstanceTab[TextureInstanceTabCounter].Theight + WND1.Mk.GetPosY() + ScaleTheight);
				WND1.ReturnGFX().ReturnRenderTarget()->DrawBitmap(TextureInstanceTab[TextureInstanceTabCounter].pBitmap.Get(), destinationRect);
			}
			
		}
	if (CurrentPlayer.CurrentPlayerTexture.pBitmap) //jak textura gracza jest to rysuj
	{
		WND1.ReturnGFX().ReturnRenderTarget()->DrawBitmap(CurrentPlayer.CurrentPlayerTexture.pBitmap.Get(), CurrentPlayer.PlayerRect);
	}
		

	WND1.ReturnGFX().EndFrame();
}
void Mainapp::DoFrame() {

	DoLogic();
	DoDrawing();
	
}

bool Mainapp::IFColision(const D2D1_RECT_F& rect1, const D2D1_RECT_F& rect2) //jezeli prostok¹ty siê pokryj¹ zwracana jest odpowiednia wartosc
{
	// Sprawdzamy warunki brzegowe, czyli czy jeden prostok¹t jest po lewej, po prawej, nad lub pod drugim prostok¹tem.
	if (rect1.right < rect2.left || rect1.left > rect2.right || rect1.bottom < rect2.top || rect1.top > rect2.bottom)
		return false;
	return true;
}
void Mainapp::UpdateCameraPosition()
{
	std::vector<TextureInstance> Rollback = TextureInstanceTab;
	if (CameraXState || CameraYState)
	{
		for (auto& texture : TextureInstanceTab)
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
		TextureInstanceTab = Rollback;
	//jezeli bede chcial by cos nie mialo kolizji bede musial dodac drug¹ pêtlê która bêdzie sczytywaæ na nowo i sprawdzac parametr czy ma miec kolizje i jak ma miec to skipuje 
	for (auto& texture : TextureInstanceTab)
	{
		if (!texture.IsCollisionOn) { //czy textura ma kolizje jest przypisane do jakby obrazka nie do pojedynczego recta, dlatego jak chcemy mieæ 2 ró¿ne textury z kolizj¹ i bez to po prostu
									 //bêdzie trzeba wczytaæ 2 textury i ustawiæ im 2 ró¿ne parametry
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
	}

		CameraXPosition = 0;
		CameraYPosition = 0;
		CameraXState = false;
		CameraYState = false;
		Collision = false;
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
void Mainapp::LoadAudio()
{
	std::wstring selectedFilePath;
	Microsoft::WRL::ComPtr<IFileDialog> pFileDialog;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

	if (SUCCEEDED(hr))
	{
		// Ustaw opcje dialogu, np. filtry plików audio
		COMDLG_FILTERSPEC fileTypes[] = { L"Audio Files", L"*.wav;*.mp3;*.ogg" };
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
					AudioCounter++;

					// Konwertuj na std::wstring lub std::string
					selectedFilePath = pszFilePath;

				}
				try {
					std::filesystem::path projectFolder = std::filesystem::current_path();
					std::filesystem::path sourcePath(pszFilePath);
					// Uzyskaj pe³n¹ œcie¿kê do pliku docelowego w folderze projektowym
					std::filesystem::path destinationPath = projectFolder / sourcePath.filename();
					// Skopiuj plik
					std::filesystem::copy_file(sourcePath, destinationPath, std::filesystem::copy_options::overwrite_existing);

					MessageBoxA(WND1.GetHandle(), "Plik skopiowany pomyœlnie.", NULL, MB_OK);
				}
				catch (const std::exception& e)
				{
					MessageBoxA(WND1.GetHandle(), "B³¹d podczas kopiowania pliku: ", NULL, MB_OK);
				}
				CoTaskMemFree(pszFilePath);
			}
		}
	}
	// Po uzyskaniu œcie¿ki pliku
	IXAudio2MasteringVoice* pMasteringVoice = nullptr;
	if (SUCCEEDED(XAudio2Create(SoundHandler.pXAudio2.GetAddressOf(), 0, XAUDIO2_DEFAULT_PROCESSOR)))
	{
		if (SUCCEEDED(SoundHandler.pXAudio2->CreateMasteringVoice(&pMasteringVoice)))
		{
			// Ustaw format dŸwiêku
			SoundHandler.waveFormat.wFormatTag = WAVE_FORMAT_PCM;
			SoundHandler.waveFormat.nChannels = 2; // Stereo
			SoundHandler.waveFormat.nSamplesPerSec = 44100; // Przyk³adowa czêstotliwoœæ próbkowania
			SoundHandler.waveFormat.nAvgBytesPerSec = 44100 * 2 * 2; // Przyk³adowa liczba bajtów na sekundê
			SoundHandler.waveFormat.nBlockAlign = 4;
			SoundHandler.waveFormat.wBitsPerSample = 16;
			SoundHandler.waveFormat.cbSize = 0;
			GetWaveFormat(selectedFilePath, SoundHandler.waveFormat);
			// Wczytaj plik audio
			LoadAudioData(selectedFilePath);
			// Utwórz Ÿród³owy g³os XAudio2
			if (SUCCEEDED(SoundHandler.pXAudio2->CreateSourceVoice(&SoundHandler.pSourceVoice, &SoundHandler.waveFormat)))
			{
				// Przeka¿ dane dŸwiêku do Ÿród³owego g³osu

				SoundHandler.buffer.AudioBytes = static_cast<UINT32>(SoundHandler.audioData.size());
				SoundHandler.buffer.pAudioData = SoundHandler.audioData.data();
				SoundHandler.buffer.Flags = XAUDIO2_END_OF_STREAM;

				hr = SoundHandler.pSourceVoice->SubmitSourceBuffer(&SoundHandler.buffer); // mam dobr¹ i z³¹ wiadomoœæ, zaimplementowalem obsluge dzwiêku, zla to taka ze nie dziala
				if (hr == S_OK) {
					// Rozpocznij odtwarzanie
					SoundHandler.pSourceVoice->Start();
				}

			}
		}
	}
}
void Mainapp::GetWaveFormat(const std::wstring& filePath, WAVEFORMATEX& waveFormat)
{
    MMCKINFO ckRIFF = { 0 };
    MMCKINFO ckFmt = { 0 };

    // Otwórz plik .wav
    HMMIO hFile = mmioOpenW(const_cast<LPWSTR>(filePath.c_str()), nullptr, MMIO_READ);
    if (hFile == nullptr)
    {
        // Obs³uga b³êdów otwierania pliku .wav
        return;
    }

    // SprawdŸ, czy to jest plik .wav
    if (mmioDescend(hFile, &ckRIFF, nullptr, 0) != MMSYSERR_NOERROR)
    {
        // Obs³uga b³êdów opisu pliku .wav
        mmioClose(hFile, 0);
        return;
    }

    if (ckRIFF.ckid != FOURCC_RIFF || ckRIFF.fccType != mmioFOURCC('W', 'A', 'V', 'E'))
    {
        // Obs³uga b³êdów - to nie jest plik .wav
        mmioClose(hFile, 0);
        return;
    }

    // ZnajdŸ fmt chunk
    ckFmt.ckid = mmioFOURCC('f', 'm', 't', ' ');
    if (mmioDescend(hFile, &ckFmt, &ckRIFF, MMIO_FINDCHUNK) != MMSYSERR_NOERROR)
    {
        // Obs³uga b³êdów opisu fmt chunk
        mmioClose(hFile, 0);
        return;
    }

    // Odczytaj dane fmt chunk
    if (mmioRead(hFile, reinterpret_cast<HPSTR>(&waveFormat), sizeof(WAVEFORMATEX)) != sizeof(WAVEFORMATEX))
    {
        // Obs³uga b³êdów odczytu danych fmt chunk
        mmioClose(hFile, 0);
        return;
    }

    // Zamknij plik .wav
    mmioClose(hFile, 0);
}
void Mainapp::LoadAudioData(const std::wstring& filePath)
{
	std::ifstream file(filePath, std::ios::binary);
	if (file.is_open())
	{
		// Pobierz rozmiar pliku
		file.seekg(0, std::ios::end);
		size_t fileSize = file.tellg();
		file.seekg(0, std::ios::beg);

		// Zarezerwuj pamiêæ dla danych audio
		SoundHandler.audioData.resize(fileSize);

		// Wczytaj dane audio
		file.read(reinterpret_cast<char*>(SoundHandler.audioData.data()), fileSize);

		file.close();
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

