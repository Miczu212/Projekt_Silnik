#include "Mainapp.h"
#include<random>
#include<chrono>

// DZIALA 


Mainapp::Mainapp()
	: 
	WND1(ScreenWidth, ScreenHeight, "testing") // ustawianie parametr�w okna
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

void Mainapp::DoFrame() {
	std::filesystem::path CopiedPath;
	std::random_device rd; //tecza ale boli w oczy
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 10);
	int randomNumber = dis(gen);
	int randomNumber2 = dis(gen);
	int randomNumber3 = dis(gen);
	WND1.ReturnGFX().BeginFrame();

	WND1.ReturnGFX().ClearBuffer(0, 0, 0); // by wylaczyc tencze wstaw tu sta�e
	if (WND1.Mk.LeftIsPressed() == true)
		WND1.ReturnGFX().Draw(MousePosition, MousePosition);
	else
		WND1.ReturnGFX().Draw(MousePosition);


	if (WND1.ReturnGFX().pBitmap.Get())
	{
		//dostawanie wysokosci i szerokosci textury
		Gdiplus::GdiplusStartupInput gdiplusStartupInput;
		ULONG_PTR gdiplusToken;
		Gdiplus::GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);
		Gdiplus::Bitmap* bitmap = Gdiplus::Bitmap::FromFile(TextureTab[TextureTabCounter].c_str());
		if (bitmap) {

			Twidth = bitmap->GetWidth();
			Theight = bitmap->GetHeight();
			delete bitmap;
		}
		else {
		}

		Gdiplus::GdiplusShutdown(gdiplusToken);
		//
		D2D1_RECT_F destinationRect = D2D1::RectF(0.0f, 0.0f, Twidth, Theight);
		WND1.ReturnGFX().ReturnRenderTarget()->Clear();
		WND1.ReturnGFX().ReturnRenderTarget()->DrawBitmap(WND1.ReturnGFX().pBitmap.Get(), destinationRect);
	}
	if (WND1.Klt.KeyIsPressed(KEY_R))
	{
		TextureTabCounter++;
		Twidth = 0;
		Theight = 0;
		if (TextureTabCounter > TextureTab.size() - 1)
		{
			TextureTabCounter = 0;
		}
		WND1.ReturnGFX().pBitmap.Reset();  // Zwalnianie poprzedniego zasobu.
		LoadBMPToTexture(TextureTab[TextureTabCounter], WND1.ReturnGFX().ReturnRenderTarget(), WND1.ReturnGFX().pBitmap.GetAddressOf());
		WND1.Klt.ClearState();
	}
	if (WND1.Klt.KeyIsPressed(KEY_F))
	{

		Microsoft::WRL::ComPtr<IFileDialog> pFileDialog;
		HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));

		if (SUCCEEDED(hr))
		{
			// Ustaw opcje dialogu, np. filtry plik�w
			COMDLG_FILTERSPEC fileTypes[] = { L"Bitmap Files", L"*.bmp;*.png;*.jpg" };
			pFileDialog->SetFileTypes(_countof(fileTypes), fileTypes);

			// Poka� okno dialogowe
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
						std::wstring selectedFilePath = pszFilePath;
						// Mo�esz tak�e skopiowa� plik do folderu projektu itp.
						LoadBMPToTexture(CopiedPath = CopyBitmapToProjectFolder(selectedFilePath), WND1.ReturnGFX().ReturnRenderTarget(), WND1.ReturnGFX().pBitmap.GetAddressOf());
						TextureTab.push_back(CopiedPath);
						Twidth = 0;
						Theight = 0;
						TextureTabCounter++;
						// Zwolnij pami��
						CoTaskMemFree(pszFilePath);
					}


				}
			}


		}

		WND1.Klt.ClearState();
	}
	if (WND1.Klt.KeyIsPressed(KEY_Q))
	{
		SetWindowTextA(WND1.GetHandle(), "jak naciskam Q to sie zmienia na ta");
		WND1.Klt.ClearState(); // wymagane, inaczej przycisk jest uznawany za "Wiecznie wcisniety"
	}
		WND1.ReturnGFX().EndFrame();
	
}

std::filesystem::path Mainapp::CopyBitmapToProjectFolder(const std::wstring& SourceFilePath)
{
	try {
		std::filesystem::path projectFolder = std::filesystem::current_path();
		std::filesystem::path sourcePath(SourceFilePath);
			// Uzyskaj pe�n� �cie�k� do pliku docelowego w folderze projektowym
		std::filesystem::path destinationPath = projectFolder / sourcePath.filename();
			// Skopiuj plik
		std::filesystem::copy_file(sourcePath, destinationPath, std::filesystem::copy_options::overwrite_existing);

		MessageBoxA(WND1.GetHandle(), "Plik skopiowany pomy�lnie.", NULL, MB_OK);
		return destinationPath;
	}
	catch(const std::exception& e)
	{
		MessageBoxA(WND1.GetHandle(), "B��d podczas kopiowania pliku: ", NULL, MB_OK);
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
				oss << e.GetPosX() << "," << e.GetPosY();			// do stringstreama dodajemy wsp�rz�dne gdzie lpm zosta� wci�ni�ty
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
