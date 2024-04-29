#include "Mainapp.h"

// DZIALA 


Mainapp::Mainapp()
	:
	WND1(ScreenWidth, ScreenHeight, "testing") // ustawianie parametrów okna
{

	font.InitializeFont(WND1.ReturnGFX().ReturnRenderTarget(), font.pSubregions);
	WND1.ReturnGFX().ReturnRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::DeepSkyBlue),
		&BackgroundColour);

}

int Mainapp::Go()
{
	while (true)
	{
		ProcessMessages();
		DoFrame();

	}
	return msg.wParam;
}
void Mainapp::HandleInput() noexcept
{
	//Za co odpowiada klikniêcie
	if (WND1.Mk.LeftIsPressed())
	{
		int X = WND1.Mk.GetPosX();
		int Y = WND1.Mk.GetPosY();
		switch (SelectionMode)
		{

		case MODE_MOVE:
		{
			SelectionMode++;
			break;
		}
		case MODE_SELECT:
		{

			int LocalTextureCounter = 0;
			int LocalRectCounter = 0;
			for (const auto& texture : TextureHolder)
			{
				LocalRectCounter = 0;
				for (const auto& Rect : texture.destinationRectTab)
				{
					if (X > Rect.left && X<Rect.right && Y>Rect.top && Y < Rect.bottom)
					{
						TextureCounter = LocalTextureCounter;
						SelectionRectCounter = LocalRectCounter;
						break;
					}
					LocalRectCounter++;
				}
				LocalTextureCounter++;
			}
			break;
		}
		case MODE_SCALE:
		{
			if (SelectionRectCounter != -1) {
				D2D1_RECT_F& SelectedRect = TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter];

				SelectedRect.bottom = Y;
				SelectedRect.right = X;
			}
			break;

		}
		case MODE_ROTATE:
		{
			//TODO Mo¿e zaimplementowaæ, nie jest must have ale nice by by³o mieæ
			break;
		}
		}
	}
	else
	{
		if (SelectionMode == MODE_MOVE)
		{

			int X = WND1.Mk.GetPosX();
			int Y = WND1.Mk.GetPosY();
			if (SelectionRectCounter != -1) {
				TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter] =
					D2D1::RectF(X, Y,

						X + TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].right -

						TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].left

						, Y + TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].bottom

						- TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].top
					);
			}
		}
	}
	//Poruszanie sie po tablicy textur
	ISPressed(KEY_R)
	{
		if (TextureHolder.size() != 0) {


			TextureCounter++;
			if (TextureCounter > TextureHolder.size() - 1)
				TextureCounter = 0;
			WND1.Klt.ClearState();
		}
	}
	//Wczytanie Textury
	ISPressed(KEY_F)
	{
		LoadFileTypeTexture();
		WND1.Klt.ClearState();
	}
	//Zmiana Kierunku skalowania
	ISPressed(KEY_SPACE)
	{
		ScaleDirection = -(ScaleDirection);
		WND1.Klt.ClearState();
	}
	//Skalowanie szerokoœci
	ISPressed(KEY_N)
	{
			ScaleTwidth -= 10*ScaleDirection;
			WND1.Klt.ClearState();
	}
	//Skalowanie Wysokoœci
	ISPressed(KEY_M)
	{
			ScaleTheight -= 10 * ScaleDirection;
			WND1.Klt.ClearState();
	}
	//Skalowanie textury 
	ISPressed(KEY_J)
	{
		ScaleTheight -= 10 * ScaleDirection;
		ScaleTwidth -= 10 * ScaleDirection;
		WND1.Klt.ClearState();
	}
	// Reset do domyslnej skali | Reset do domyœlnej pozycji
	ISPressed(KEY_B)
	{
		ScaleTwidth = 0;
		ScaleTheight = 0;
		if (SelectionMode == MODE_SCALE)
		{
			if (SelectionRectCounter != -1 && TextureCounter != -1) {
				TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].bottom = RollbackRectBottom;
				TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].right = RollbackRectRight;
			}
		}
		else if (SelectionMode == MODE_MOVE)
		{
			if (SelectionRectCounter != -1 && TextureCounter != -1) {
				TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].bottom = RollbackRectBottom;
				TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].right = RollbackRectRight;
				TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].left = RollbackRectLeft;
				TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].top = RollbackRectTop;
				SelectionMode++;
			}
		}
		WND1.Klt.ClearState();

	}
	//Zapisanie Poziomu
	ISPressed(KEY_Z)
	{
		SaveFileTypeLevel();
		WND1.Klt.ClearState();
	}
	//Wczytanie Poziomu
	ISPressed(KEY_L)
	{
		LoadFileTypeLevel();
		WND1.Klt.ClearState();
	}
	//Sterowanie
	ISPressed(KEY_LEFT)
	{
		CameraXPosition = MOVMENT_SPEED;
		CameraXState = true;
		AnimationRollback = LEFT;
		//PlayPlayerAnimation(0, 4);
	}
	ISPressed(KEY_RIGHT)
	{
		CameraXPosition = -MOVMENT_SPEED;
		CameraXState = true;
		//PlayPlayerAnimation(5, 9);
		AnimationRollback = RIGHT;

	}

	ISPressed(KEY_DOWN)
	{
		//PlayPlayerAnimation(15, 19);
		CameraYPosition = -MOVMENT_SPEED;
		CameraYState = true;
		AnimationRollback = BOTTOM;
	}
	ISPressed(KEY_UP)
	{
		if (!IsJumping) {
			if (!IsGravityTurnedOn)
			{
				CameraYPosition = MOVMENT_SPEED;
				CameraYState = true;
			}
			GravityChanged = false;
			IsJumping = true;
		}
		//PlayPlayerAnimation(10, 14); // na blache poniewaz ustalam jaka animacja z animation spreadsheeta sie uruchomi
		AnimationRollback = TOP;
	}
	//Sterowanie
	//Ustawienie Gracza na obecnie Wybran¹ texture
	ISPressed(KEY_P)
	{
		if (TextureHolder.size() != 0) {
			CurrentPlayer.CurrentPlayerTexture = TextureHolder[TextureCounter];
			CurrentPlayer.PlayerRect = D2D1::RectF(
				ScreenWidth / 2 - (CurrentPlayer.CurrentPlayerTexture.Twidth + ScaleTwidth) / 2,
				ScreenHeight / 2 - (CurrentPlayer.CurrentPlayerTexture.Theight + ScaleTheight) / 2,
				ScreenWidth / 2 + (CurrentPlayer.CurrentPlayerTexture.Twidth + ScaleTwidth) / 2,
				ScreenHeight / 2 + (CurrentPlayer.CurrentPlayerTexture.Theight + ScaleTheight) / 2
			);
			//zrobione tak by postac byla zawsze na srodku ekranu
		}
	}
	//Wczytanie dzwiekow
	ISPressed(KEY_O)
	{
		LoadFileTypeAudio();

	}
	//Poruszanie sie po tablicy dzwiekow
	ISPressed(KEY_0)
	{
		if (AudioHolder.size() != 0) {
			AudioCounter++;

			if (AudioCounter >= AudioHolder.size())
				AudioCounter = 0;
			WND1.Klt.ClearState();
		}
	}
	ISPressed(KEY_A)
	{
		Animation Anim;
		std::wstring filepath = OpenFileDialog(L"Bitmap Files", L"*.bmp;*.png;*.jpg");
		try {
			std::filesystem::path projectFolder = std::filesystem::current_path();
			std::filesystem::path sourcePath(filepath);
			std::filesystem::path destinationFolder="Animations";
			// Uzyskaj pe³n¹ œcie¿kê do pliku docelowego w folderze projektowym
			std::filesystem::path destinationPath = projectFolder / destinationFolder / sourcePath.filename();

			if (std::filesystem::exists(destinationPath)) {
				MessageBoxA(WND1.GetHandle(), "Plik o takiej samej nazwie, ju¿ istnieje w folderze projektowym.", NULL, MB_OK);
			}

			else {
				std::filesystem::copy_file(sourcePath, destinationPath, std::filesystem::copy_options::overwrite_existing);

				MessageBoxA(WND1.GetHandle(), "Plik skopiowany pomyœlnie.", NULL, MB_OK);
			}
			Anim.SpreadSheetPath = destinationPath;
			AnimHolder.Animations.push_back(Anim);
			AnimHolder.Animations[AnimHolder.Animations.size() - 1].InitializeAnimation(AnimHolder, 90, 90, 5, 4, WND1.ReturnGFX().ReturnRenderTarget(), destinationPath);
			AnimationIndex++;
		}
		catch (const std::exception& e)
		{
			MessageBoxA(WND1.GetHandle(), "B³¹d podczas kopiowania pliku: ", NULL, MB_OK);
		}
		
	}
	//Odtworzenie dzwieku
	ISPressed(KEY_9)
	{
		if(AudioCounter!=-1)
		AudioHolder[AudioCounter].Play(1.0f, 1.0f);
		WND1.Klt.ClearState();
	}
	//Zatrzymanie odtworzenia dzwieku
	ISPressed(KEY_8)
	{
		if (AudioCounter != -1)
		AudioHolder[AudioCounter].Stop();
		WND1.Klt.ClearState();
	}
	//Prze³¹czenie Kolizji Dla Wybranej Textury
	ISPressed(KEY_Q)
	{
		if (TextureCounter != -1)
		TextureHolder[TextureCounter].IsCollisionOn = !TextureHolder[TextureCounter].IsCollisionOn;
		WND1.Klt.ClearState();
	}
	ISPressed(KEY_W)
	{
		std::wstring selectedFilePath = OpenFileDialog(L"Binary Files", L"*.bin;*.dat");
		Currentlevel.ReTargetLevel(selectedFilePath);
	}
	//Zmiana Trybu Selekcji
	ISPressed(KEY_E)
	{
		if (czyrysowaclinie == false)
		{
			SelectionMode++;
			if (SelectionMode == MODE_SCALE)
			{
				if (SelectionRectCounter != -1) {
					if (!TextureHolder[TextureCounter].destinationRectTab.empty()) {
						RollbackRectBottom = TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].bottom;
						RollbackRectRight = TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].right;
					}
				}
			}
			if (SelectionMode == MODE_MOVE)
			{
				if (SelectionRectCounter != -1) {
					if (!TextureHolder[TextureCounter].destinationRectTab.empty()) {
						RollbackRectBottom = TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].bottom;
						RollbackRectRight = TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].right;
						RollbackRectTop = TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].top;
						RollbackRectLeft = TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].left;
					}
				}
			}

			if (SelectionMode > 4)
			{
				SelectionMode = 0;
			}
		}
		WND1.Klt.ClearState();
	}
	ISPressed(KEY_DELETE)
	{
		if (SelectionRectCounter!=-1)
		{
			if (SelectionMode == MODE_SELECT)
			{
				TextureHolder[TextureCounter].destinationRectTab.erase(TextureHolder[TextureCounter].destinationRectTab.begin() + SelectionRectCounter);
				SelectionRectCounter = -1;
			}
		}
		WND1.Klt.ClearState();
	}
	ISPressed(KEY_CONTROL)
	{
		RepeatIfPossible = !RepeatIfPossible;
		WND1.Klt.ClearState();
	}
	ISPressed(KEY_SHIFT)
	{
		if (SelectionMode == MODE_SELECT)
		{
			WND1.Mk.OnMyszkaMove(TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].left, TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter].top);
		}
		WND1.Mk.Axis++;
			if (WND1.Mk.Axis > 2)
			{
				WND1.Mk.Axis = AXIS_NONE;
			}
			WND1.Klt.ClearState();
	}
	ISPressed(KEY_G)
	{
		IsGravityTurnedOn = !IsGravityTurnedOn;
		WND1.Klt.ClearState();
	}
	ISPressed(KEY_T)
	{
		if (AnimHolder.Animations.size() != 0) {
			CurrentPlayer.CurrentPlayerTexture = AnimHolder.AnimationFrames[AnimationIndex][5];

			CurrentPlayer.PlayerRect = D2D1::RectF(
				ScreenWidth / 2 - (CurrentPlayer.CurrentPlayerTexture.Twidth + ScaleTwidth) / 2,
				ScreenHeight / 2 - (CurrentPlayer.CurrentPlayerTexture.Theight + ScaleTheight) / 2,
				ScreenWidth / 2 + (CurrentPlayer.CurrentPlayerTexture.Twidth + ScaleTwidth) / 2,
				ScreenHeight / 2 + (CurrentPlayer.CurrentPlayerTexture.Theight + ScaleTheight) / 2
			);
		}
		WND1.Klt.ClearState();
	}
	ISPressed(KEY_I)
	{
		if (AnimHolder.Animations.size() != 0) {
			AnimationIndex++;
			if (AnimationIndex >= AnimHolder.Animations.size())
				AnimationIndex = 0;
		}
	}

}
//Funkcje Wczytuj¹ce/Zapisuj¹ce
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
		Currentlevel.LoadLevel(TextureHolder,AnimHolder, AudioPathHolder, selectedFilePath, CurrentPlayer);
		//Animacje
		for (auto& Files : AnimHolder.Animations)
		{
			Files.InitializeAnimation(AnimHolder, 90, 90, 5, 4, WND1.ReturnGFX().ReturnRenderTarget(), Files.SpreadSheetPath); AnimationIndex++;
		}
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
		if (TextureHolder.size() != 0)
			TextureCounter = 0;
		//textury

		//audio
		AudioHolder.clear();


		for (auto& Path : AudioPathHolder)
		{
			if (!Path.empty())
			{
				Sound s(Path);
				AudioHolder.push_back(s);
				//po tym jak wyjdziemy ze scopa to destruktor dla s automatycznie siê uruchamia, a audioholder przechowuje kopie

			}

		}
		if(AudioHolder.size()!=0)
		AudioCounter = 0;
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
					selectedFilePath = pszFilePath;
					CoTaskMemFree(pszFilePath);
				}
			}
		}
	}
	Currentlevel.SaveLevel(TextureHolder,AnimHolder,AudioPathHolder, selectedFilePath, CurrentPlayer);
}
void Mainapp::LoadFileTypeTexture()
{
	std::wstring SelectedPath = OpenFileDialog(L"Bitmap Files", L"*.bmp;*.png;*.jpg");
	if (!SelectedPath.empty()) {
		czyrysowaclinie = false;
		TextureCounter++;
		TextureInstance NewTextureInstance;
		TextureHolder.push_back(NewTextureInstance);
		LoadBMPToTexture(
			CopyFileToProjectFolder(SelectedPath),
			WND1.ReturnGFX().ReturnRenderTarget(),
			TextureHolder[TextureCounter].pBitmap.GetAddressOf()
		);
		TextureHolder[TextureCounter].Path = SelectedPath;

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
//Funkcje Wczytuj¹ce/Zapisuj¹ce
void Mainapp::DoLogic()
{
	if (timer.Peek() >= 1.0f / 120.0f) //logika bêdzie sprawdzana co 1.0f/30.0f sekundy a nie co klatke, przez co dla wszystkich frameratów gra bedzie podobnie p³ynna
	{
		GravityChanged = IsGravityTurnedOn; //musi byc przed HandleInputem w celu mozliwosci zmiany tego stanu
		timer.Mark();
		if (AnimHolder.Animations.size() != 0) //reset animacji
			AnimationRollback = -1;
		HandleInput();
	}
}
void Mainapp::PlayAnimation()
{
	if (AnimHolder.Animations[AnimationIndex].AnimationTimer.Peek() >= 1.0f / 5.0f)
	{
		AnimHolder.Animations[AnimationIndex].CurrentFrame++;
		AnimHolder.Animations[AnimationIndex].AnimationTimer.Mark();
		if (AnimHolder.Animations[AnimationIndex].CurrentFrame >= AnimHolder.AnimationFrames[AnimationIndex].size())
			AnimHolder.Animations[AnimationIndex].CurrentFrame = 0;
	}
	WND1.ReturnGFX().ReturnRenderTarget()->DrawBitmap(AnimHolder.AnimationFrames[AnimationIndex][AnimHolder.Animations[AnimationIndex].CurrentFrame].pBitmap.Get(),
		D2D1::RectF(200, 200, 290, 290));
}
void Mainapp::PlayPlayerAnimation(int StartFrame, int EndFrame)
{
	if (AnimHolder.Animations[AnimationIndex].CurrentFrame > EndFrame || AnimHolder.Animations[AnimationIndex].CurrentFrame < StartFrame)
		AnimHolder.Animations[AnimationIndex].CurrentFrame = StartFrame;
	if (AnimHolder.Animations[AnimationIndex].AnimationTimer.Peek() >= 1.0f / 5.0f)
	{
		
		AnimHolder.Animations[AnimationIndex].CurrentFrame++;
		AnimHolder.Animations[AnimationIndex].AnimationTimer.Mark();
		if (AnimHolder.Animations[AnimationIndex].CurrentFrame > EndFrame)
			AnimHolder.Animations[AnimationIndex].CurrentFrame = StartFrame;
	}
	CurrentPlayer.CurrentPlayerTexture = AnimHolder.AnimationFrames[AnimationIndex][AnimHolder.Animations[AnimationIndex].CurrentFrame];
	CurrentPlayer.PlayerRect = D2D1::RectF(
		ScreenWidth / 2 - (CurrentPlayer.CurrentPlayerTexture.Twidth ) / 2,
		ScreenHeight / 2 - (CurrentPlayer.CurrentPlayerTexture.Theight ) / 2,
		ScreenWidth / 2 + (CurrentPlayer.CurrentPlayerTexture.Twidth ) / 2,
		ScreenHeight / 2 + (CurrentPlayer.CurrentPlayerTexture.Theight ) / 2
	);
}

void Mainapp::DoDrawing()
{
	WND1.ReturnGFX().BeginFrame();
	WND1.ReturnGFX().ClearBuffer(0, 0, 0);
	//kolor t³a
//	WND1.ReturnGFX().ReturnRenderTarget()->FillRectangle(&Background, BackgroundColour);
//	WND1.ReturnGFX().ReturnRenderTarget()->DrawRectangle(&Background, BackgroundColour);
	//kolor t³a //zakomentuj by zmieniæ na czarny
	switch (WND1.Mk.Axis)
	{
	case AXIS_Y:
		{
		if (RepeatIfPossible)
		{
			Write("Axis_Y    AutoReapeat:On", 0, ScreenHeight - 24);
		}
		else
		{
			Write("Axis_Y", 0, ScreenHeight - 24);
		}
		
		break;
		}
		case AXIS_X:
		{
			if (RepeatIfPossible)
			{
				Write("Axis_X   AutoReapeat:On", 0, ScreenHeight - 24);
			}
			else 
			{
				Write("Axis_X", 0, ScreenHeight - 24);
			}
			
			break;
		}
	}
	switch (SelectionMode)
	{

	case MODE_MOVE:
	{
		if (SelectionRectCounter != -1)
		{
			ID2D1SolidColorBrush* pBrush = nullptr;
			WND1.ReturnGFX().ReturnRenderTarget()->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Blue),
				&pBrush
			);
			WND1.ReturnGFX().ReturnRenderTarget()->FillRectangle(&TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter], pBrush);
		}
		Write("Move_Mode", 0, 0);
		break;
	}
	case MODE_SELECT:
	{
		if (SelectionRectCounter != -1)
		{
			ID2D1SolidColorBrush* pBrush = nullptr;
			WND1.ReturnGFX().ReturnRenderTarget()->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Blue),
				&pBrush
			);
			WND1.ReturnGFX().ReturnRenderTarget()->FillRectangle(&TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter], pBrush);
		}
		Write("Select_Mode", 0, 0);
		break;

	}
	case MODE_SCALE:
	{
		if (SelectionRectCounter != -1)
		{
			ID2D1SolidColorBrush* pBrush = nullptr;
			WND1.ReturnGFX().ReturnRenderTarget()->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Blue),
				&pBrush
			);
			WND1.ReturnGFX().ReturnRenderTarget()->FillRectangle(&TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter], pBrush);
		}
		Write("Scale_Mode", 0, 0);
		break;
	}
	case MODE_ROTATE:
	{
		if (SelectionRectCounter != -1)
		{
			ID2D1SolidColorBrush* pBrush = nullptr;
			WND1.ReturnGFX().ReturnRenderTarget()->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Blue),
				&pBrush
			);
			WND1.ReturnGFX().ReturnRenderTarget()->FillRectangle(&TextureHolder[TextureCounter].destinationRectTab[SelectionRectCounter], pBrush);
		}
		Write("Rotate_Mode", 0, 0);
		break;
	}
	case MODE_PLACE:
	{
		Write("Place_Mode", 0, 0);
		break;
	}
	}
	if (SelectionMode == MODE_PLACE)
	{
		
			if (RepeatIfPossible)
			{
				bool CanDraw = true;
				if (WND1.Mk.LeftIsPressed())
				{
					
						for (auto& texture : TextureHolder)
						{
							if (texture.pBitmap)
							{

								for (auto& destinationRect : texture.destinationRectTab)
								{
									if (IFColision(D2D1::RectF(WND1.Mk.GetPosX(), WND1.Mk.GetPosY(), TextureHolder[TextureCounter].Twidth + WND1.Mk.GetPosX() + ScaleTwidth,
										TextureHolder[TextureCounter].Theight + WND1.Mk.GetPosY() + ScaleTheight), destinationRect))
									{
										CanDraw = false;
										break;
									}

								}
							}

						}
					
					if (CanDraw)
					{
						if (TextureCounter != -1)
						{
							D2D1_RECT_F destinationRect = D2D1::RectF(WND1.Mk.GetPosX(), WND1.Mk.GetPosY(), TextureHolder[TextureCounter].Twidth + WND1.Mk.GetPosX() + ScaleTwidth,
								TextureHolder[TextureCounter].Theight + WND1.Mk.GetPosY() + ScaleTheight);
							TextureHolder[TextureCounter].destinationRectTab.push_back(destinationRect);
						}
					}
					
					
				}
			}
			else
			{
				NoAutoclickE(			//Ta funkcja jest tutaj nie w handle input dlatego ¿e jest bezpoœrednio zwi¹zana z rysowaniem 
					if (czyrysowaclinie == true)
						WND1.ReturnGFX().Draw(MousePosition, MousePosition);
				if (TextureCounter != -1)
				{
					D2D1_RECT_F destinationRect = D2D1::RectF(WND1.Mk.GetPosX(), WND1.Mk.GetPosY(), TextureHolder[TextureCounter].Twidth + WND1.Mk.GetPosX() + ScaleTwidth,
						TextureHolder[TextureCounter].Theight + WND1.Mk.GetPosY() + ScaleTheight);
					TextureHolder[TextureCounter].destinationRectTab.push_back(destinationRect);
				}
				, else if (czyrysowaclinie == true) //else if jest po przecinku z powodu struktury makra
					WND1.ReturnGFX().Draw(MousePosition);
				)
			}
	}
	switch (AnimationRollback)
	{
		case TOP: {
			PlayPlayerAnimation(10, 14);
			break;
		}
		case BOTTOM: {
			PlayPlayerAnimation(15, 19);
			break;
		}
		case LEFT: {
			PlayPlayerAnimation(0, 4);
			break;
		}
		case RIGHT: {
			PlayPlayerAnimation(5, 9);
			break;
		}
		case -1:
		{
			CurrentPlayer.CurrentPlayerTexture = AnimHolder.AnimationFrames[AnimationIndex][5]; //5 to klatka do ktorej bedzie defaultowac
			CurrentPlayer.PlayerRect = D2D1::RectF(
				ScreenWidth / 2 - (CurrentPlayer.CurrentPlayerTexture.Twidth) / 2,
				ScreenHeight / 2 - (CurrentPlayer.CurrentPlayerTexture.Theight) / 2,
				ScreenWidth / 2 + (CurrentPlayer.CurrentPlayerTexture.Twidth) / 2,
				ScreenHeight / 2 + (CurrentPlayer.CurrentPlayerTexture.Theight) / 2
			);
			break;
		}
	}
	if (CurrentPlayer.CurrentPlayerTexture.pBitmap) //jak textura gracza jest to rysuj
	{



			if (IsJumping)			
				Jump();
			if (GravityChanged)
				UpdateGravity();
		WND1.ReturnGFX().ReturnRenderTarget()->DrawBitmap(CurrentPlayer.CurrentPlayerTexture.pBitmap.Get(), CurrentPlayer.PlayerRect);


	}

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
		if (SelectionMode == MODE_PLACE) {
			if (TextureHolder[TextureCounter].pBitmap) //Rysowanie obecnie wybranej textury w pozycji kursora myszy
			{
				D2D1_RECT_F destinationRect = D2D1::RectF(WND1.Mk.GetPosX(), WND1.Mk.GetPosY(), TextureHolder[TextureCounter].Twidth + WND1.Mk.GetPosX() + ScaleTwidth,
					TextureHolder[TextureCounter].Theight + WND1.Mk.GetPosY() + ScaleTheight);
				WND1.ReturnGFX().ReturnRenderTarget()->DrawBitmap(TextureHolder[TextureCounter].pBitmap.Get(), destinationRect);
			}
		}
	}
	WND1.ReturnGFX().EndFrame();
}
void Mainapp::DoFrame() {
	/*std::ostringstream oss;
	oss << timer.Peek();
	SetWindowTextA(WND1.GetHandle(), oss.str().c_str());*/ //potrzebne do debugowania problemów z timerem
	DoLogic();
	DoDrawing();


}
void Mainapp::UpdateGravity()
{
	std::vector<TextureInstance> Rollback = TextureHolder;

	for (auto& texture : TextureHolder)
	{
		for (auto& rect : texture.destinationRectTab)
		{
			if (texture.IsCollisionOn) {
				if (IFColisionWithSides(CurrentPlayer.PlayerRect, rect) == TOP)
				{
					TextureHolder = Rollback;
					GravityChanged = false;
					break;
				}
			}
		}

	}
	if (GravityChanged) { 
		bool go = true;
		for (auto& texture : TextureHolder)
		{
			if (go) {
				for (auto& rect : texture.destinationRectTab)
				{
					rect.top -= GravitySpeed;
					rect.bottom -= GravitySpeed;
					if (texture.IsCollisionOn) {
						if (IFColision(CurrentPlayer.PlayerRect, rect)) //po l¹dowaniu na bloku przestañ œci¹gaæ gracza w dó³ i pozwól mu na ponowny skok
						{
							CurrentJumpHeight = 0;
							IsJumping = false;
							TextureHolder = Rollback;
							go = false;
							break;
						}
					}
				}
			}
		}

	}
}
void Mainapp::Jump()
{
	if (CurrentJumpHeight <= MaxJumpHeight) 
	{
		for (auto& texture : TextureHolder)
		{

			for (auto& rect : texture.destinationRectTab)
			{
				
				rect.top += 10;
				rect.bottom += 10;

			}

		}
		CurrentJumpHeight += 10;
	}
}
bool Mainapp::IFColision(const D2D1_RECT_F& rect1, const D2D1_RECT_F& rect2) const noexcept //jezeli prostok¹ty siê pokryj¹ zwracana jest odpowiednia wartosc
{
	// Sprawdzamy warunki brzegowe, czyli czy jeden prostok¹t jest po lewej, po prawej, nad lub pod drugim prostok¹tem.
	if (rect1.right < rect2.left || rect1.left > rect2.right || rect1.bottom < rect2.top || rect1.top > rect2.bottom)
		return false;
	return true;
}

int Mainapp::IFColisionWithSides(const D2D1_RECT_F& rect1, const D2D1_RECT_F& rect2) const noexcept 
{
	if (IFColision(rect1, rect2))
	{
		// Obliczamy przeciêcia krawêdzi prostok¹tów.
		bool leftCollision = rect1.right >= rect2.left && rect1.left <= rect2.left;
		bool rightCollision = rect1.left <= rect2.right && rect1.right >= rect2.right;
		bool topCollision = rect1.bottom >= rect2.top && rect1.top <= rect2.top;
		bool bottomCollision = rect1.top <= rect2.bottom && rect1.bottom >= rect2.bottom;

		// Sprawdzamy, z któr¹ stron¹ dosz³o do kolizji i zwracamy odpowiedni¹ wartoœæ.
		if (topCollision)
			return TOP; // Kolizja z górnej strony	
		else if (rightCollision)
			return RIGHT; // Kolizja z prawej strony
		else if (leftCollision)
			return LEFT; // Kolizja z lewej strony
		else if (bottomCollision)
			return BOTTOM; // Kolizja z dolnej strony
	}

}


void Mainapp::UpdateCameraPosition()
{
	if(CurrentCameraSpeed<=MaxCameraSpeed){
		std::vector<TextureInstance> Rollback = TextureHolder;
		for (auto& texture : TextureHolder)
		{
			for (int i = 0; i < texture.destinationRectTab.size(); i++)
			{
				if (CameraXState) {

					texture.destinationRectTab[i].left += CameraXPosition;
					texture.destinationRectTab[i].right += CameraXPosition;
					if (texture.IsCollisionOn) {
						if (IFColision(CurrentPlayer.PlayerRect, texture.destinationRectTab[i]))
						{
							Collision = true;
							break;
						}
					}

				}
				if (CameraYState) {
					texture.destinationRectTab[i].top += CameraYPosition;
					texture.destinationRectTab[i].bottom += CameraYPosition;
					if (texture.IsCollisionOn) {
						if (IFColision(CurrentPlayer.PlayerRect, texture.destinationRectTab[i]))
						{
							Collision = true;
							break;
						}
					}
				}
			}
		
		 CurrentCameraSpeed += MOVMENT_SPEED;

		 if (Collision) //jezeli doszlo do kolizji, zawroc wszelkie zmiany do kamery
		 {
			 TextureHolder = Rollback;
			 CameraXState = false;
			 CameraYState = false;
			 Collision = false;
			 CurrentCameraSpeed = 0;
		 }
			

		
		}
	}
	else
	{
		CameraXState = false;
		CameraYState = false;
		Collision = false;
		CurrentCameraSpeed = 0;
	}
}


std::filesystem::path Mainapp::CopyFileToProjectFolder(const std::wstring& SourceFilePath) const
{
	try {
		std::filesystem::path projectFolder = std::filesystem::current_path();
		std::filesystem::path sourcePath(SourceFilePath);
		std::filesystem::path destinationFolder;
		// Uzyskaj pe³n¹ œcie¿kê do pliku docelowego w folderze projektowym
		std::string extension = sourcePath.extension().string();
		if (extension == ".jpg" || extension == ".png" || extension == ".bmp") {
			destinationFolder ="Textures";
		}
		else if (extension == ".dat" || extension == ".bin") {
			destinationFolder = "Levels";
		}
		else if (extension == ".wav" || extension == ".mp3" || extension == ".ogg")
		{
			destinationFolder = "Audio";
		}
		std::filesystem::path destinationPath = projectFolder/destinationFolder/sourcePath.filename();
		
		if (std::filesystem::exists(destinationPath)) {
			MessageBoxA(WND1.GetHandle(), "Plik o takiej samej nazwie, ju¿ istnieje w folderze projektowym.", NULL, MB_OK);
			return destinationPath;
		}

		else {
			std::filesystem::copy_file(sourcePath, destinationPath, std::filesystem::copy_options::overwrite_existing);

			MessageBoxA(WND1.GetHandle(), "Plik skopiowany pomyœlnie.", NULL, MB_OK);
			return destinationPath;
		}
	}
	catch(const std::exception& e)
	{
		MessageBoxA(WND1.GetHandle(), "B³¹d podczas kopiowania pliku: ", NULL, MB_OK);
	}
}

void Mainapp::ProcessMessages() noexcept
{
	while ((result = PeekMessageA(&msg, nullptr, 0, 0, PM_REMOVE) > 0)) //petla obslugujaca wiadomosci przychodzace
	{
		if (msg.message == WM_QUIT)
		{
			exit(0);
		}
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
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

std::wstring Mainapp::OpenFileDialog(LPCWSTR Filetype, LPCWSTR FileExtension)
{
	std::wstring selectedFilePath;
	Microsoft::WRL::ComPtr<IFileDialog> pFileDialog;
	HRESULT hr = CoCreateInstance(CLSID_FileOpenDialog, nullptr, CLSCTX_INPROC_SERVER, IID_PPV_ARGS(&pFileDialog));
	if (SUCCEEDED(hr)) {
		// Ustawianie opcji dialogu, np. filtry plików audio,pliki binarne itp.
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
							D2D1_BITMAP_PROPERTIES bitmapProperties;
							bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
							bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
							bitmapProperties.dpiX = 96.0f;
							bitmapProperties.dpiY = 96.0f;

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

void Mainapp::LoadBMPSubregionToTexture(const std::wstring& filePath, Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, const D2D1_RECT_F& sourceRegion, std::vector< Microsoft::WRL::ComPtr<ID2D1Bitmap>>& ppBitmap) const
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
							assert(sourceRegion.right < width );
							assert( sourceRegion.bottom < height);
								// subregion musi siê mieœciæ w bitmapie

							UINT subWidth = static_cast<UINT>(sourceRegion.right - sourceRegion.left);
							UINT subHeight = static_cast<UINT>(sourceRegion.bottom - sourceRegion.top);

							std::vector<BYTE> pixelData(subWidth * subHeight * 4);
							D2D1_BITMAP_PROPERTIES bitmapProperties;
							bitmapProperties.pixelFormat.format = DXGI_FORMAT_B8G8R8A8_UNORM;
							bitmapProperties.pixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
							bitmapProperties.dpiX = 96.0f;
							bitmapProperties.dpiY = 96.0f;

							WICRect rect = { static_cast<INT>(sourceRegion.left), static_cast<INT>(sourceRegion.top), static_cast<INT>(sourceRegion.right), static_cast<INT>(sourceRegion.bottom) };
							hr = pConverter->CopyPixels(&rect, subWidth * 4, subWidth * subHeight * 4, pixelData.data());
							if (SUCCEEDED(hr))
							{
								// zamieniamy kluczowy kolor na przeŸroczysty
								for (UINT i = 0; i < subWidth * subHeight; ++i)
								{
									if (*reinterpret_cast<UINT32*>(&pixelData[i * 4]) == KeyColour)
									{
										*reinterpret_cast<UINT32*>(&pixelData[i * 4]) = 0; //alpha to 0
									}
								}
								//faktyczne tworzenie bitmapy
								Microsoft::WRL::ComPtr<ID2D1Bitmap> pBitmap;
								hr = pRenderTarget->CreateBitmap(
									D2D1::SizeU(subWidth, subHeight),
									pixelData.data(),
									subWidth * 4,
									&bitmapProperties,
									pBitmap.GetAddressOf()
								);
								ppBitmap.push_back(pBitmap);

							}
						}
					}
				}
			}
		}
	}
}
void Mainapp::Write(std::string Text,int StartPositionX, int StartPositionY)
{
	int temp;
	for (int i = 0; i < Text.size(); i++)
	{
		 temp =int(Text[i])-32;
		 WND1.ReturnGFX().ReturnRenderTarget()->DrawBitmap(font.pSubregions[temp].Get(), 
			 D2D1::RectF(StartPositionX+i*13, StartPositionY, StartPositionX+13+i*13, StartPositionY +24));
	}
	
}


