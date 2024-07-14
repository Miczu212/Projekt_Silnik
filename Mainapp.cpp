#include "Mainapp.h"

// DZIALA 


Mainapp::Mainapp()
	:
	WND1("testing") // ustawianie parametrów okna
{

	font.InitializeFont(WND1.ReturnGFX().ReturnRenderTarget(), font.pSubregions);
	WND1.ReturnGFX().ReturnRenderTarget()->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::DeepSkyBlue),
		&BackgroundColour);
	ScreenHeight = WND1.Height;
	ScreenWidth = WND1.Width;

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

					SelectionMode=MODE_SELECT;
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
			case MODE_TRIGERS:
			{
				if (AddTrigerbox) 
				{
					StartTempPos = D2D1::Point2F(X, Y);
					AddTrigerbox = !AddTrigerbox;
					FinalizeTrigerbox = true;
				}
				else
				{
					TrigerBoxCounter = 0;
						for (const auto& Rect : TrigerBoxHolder)
						{
							if (X > Rect.TrigerBoxPosition.left && X<Rect.TrigerBoxPosition.right && Y>Rect.TrigerBoxPosition.top && Y < Rect.TrigerBoxPosition.bottom)
							{
								break;
							}
							TrigerBoxCounter++;
						}
						if (TrigerBoxCounter > TrigerBoxHolder.size() - 1)
							TrigerBoxCounter = -1;
					
				}
				break;
			}
		}
	}
	else
	{
		int X = WND1.Mk.GetPosX();
		int Y = WND1.Mk.GetPosY();
		if (SelectionMode == MODE_MOVE)
		{		
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
		else if (SelectionMode == MODE_TRIGERS)
		{
			if (FinalizeTrigerbox)
			{
				TrigerBoxInstance Temp;
				Temp.TrigerBoxPosition = D2D1::RectF(StartTempPos.x, StartTempPos.y, X, Y);
				TrigerBoxHolder.push_back(Temp);
				FinalizeTrigerbox = false;
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
		}
		WND1.Klt.ClearState();
	}
	//Wczytanie Textury
	ISPressed(KEY_F)
	{
		if(SelectionMode==MODE_PLACE)
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
		if (TextureCounter != -1) {
			ScaleTheight -= 10 * ScaleDirection;
			ScaleTwidth -= 10 * ScaleDirection;
		}
		else
		{
			CurrentPlayer.CurrentPlayerTexture.Theight -= 10 * ScaleDirection;
			CurrentPlayer.CurrentPlayerTexture.Twidth -= 10 * ScaleDirection;
		}
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
		else if (SelectionMode == MODE_SELECT)
			SelectionRectCounter = -1;
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
		czyrysowaclinie = false;
		WND1.Klt.ClearState();
	}
	//Sterowanie
	ISPressed(KEY_LEFT)
	{
		CameraXPosition = MOVMENT_SPEED;
		CameraXState = true;
		if (!StartJumpAnimation) {
			AnimationRollback = LEFT;
			AnimationIndex = 2;
			PlayPlayerAnimation(0, 5);
			StartWalkLeftAnimation = true;	
		}
	}
	ISPressed(KEY_RIGHT)
	{
		CameraXPosition = -MOVMENT_SPEED;
		CameraXState = true;
		if (!StartJumpAnimation) {
			AnimationIndex = 1;
			PlayPlayerAnimation(0, 5);
			AnimationRollback = RIGHT;
			StartWalkRightAnimation = true;
		}	
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
		if (!HadEnoughOfJumping) {
			if (!IsJumping) {
				if (!IsGravityTurnedOn)
				{
					CameraYPosition = MOVMENT_SPEED;
					CameraYState = true;
				}
				GravityChanged = false;
				IsJumping = true;
				AnimationIndex = 0;
				if (AnimationRollback == LEFT)
					AnimationIndex = 3;
				StartJumpAnimation = true;  // na blache poniewaz ustalam jaka animacja z animation spreadsheeta sie uruchomi
				try {
					AudioHolder[1].Play(1.0f, 1.0f);
				}
				catch (...)
				{

				}
			}
		}		
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
		CollisionRect = D2D1::RectF(CurrentPlayer.PlayerRect.left + 33, CurrentPlayer.PlayerRect.top, CurrentPlayer.PlayerRect.right - 33, CurrentPlayer.PlayerRect.bottom); //sprawdzanie kolizji nie jest prowadzone na player rectie w celu mozliwosci jej korekcji
		WND1.Klt.ClearState();
	}
	//Wczytanie dzwiekow
	ISPressed(KEY_O)
	{
		czyrysowaclinie = false;
		LoadFileTypeAudio();
		WND1.Klt.ClearState();
	}
	//Poruszanie sie po tablicy dzwiekow
	ISPressed(KEY_0)
	{
		if (AudioHolder.size() != 0) {
			AudioCounter++;

			if (AudioCounter >= AudioHolder.size())
				AudioCounter = 0;
		}
		WND1.Klt.ClearState();
	}
	ISPressed(KEY_A)
	{
		WND1.Klt.FlushChar();
		czyrysowaclinie = false;
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
			LoadAnimation = true;
			TempAnim.SpreadSheetPath = destinationPath;
		}
		catch (const std::exception& e)
		{
			MessageBoxA(WND1.GetHandle(), "B³¹d podczas kopiowania pliku: ", NULL, MB_OK);
		}
		WND1.Klt.ClearState();
		
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
		{
			AudioHolder[AudioCounter].Stop();
			WND1.Klt.ClearState();
		}
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
		WND1.Klt.ClearState();
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
		if (TrigerBoxCounter != -1)
		{
			if (SelectionMode == MODE_TRIGERS)
			{
				TrigerBoxHolder.erase(TrigerBoxHolder.begin()+ TrigerBoxCounter);
				TrigerBoxCounter = -1;
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
	ISPressed(KEY_Y) //Dodanie TrigerBoxa
	{
		if(SelectionMode==MODE_TRIGERS)
		AddTrigerbox = true;
		WND1.Klt.ClearState();
	}
	ISPressed(KEY_ESCAPE)
	{
		DestroyWindow(WND1.GetHandle());
		PostQuitMessage(0);
	}
	ISPressed(KEY_F11)
	{
		WND1.ResizeWindow();
		WND1.Klt.ClearState();
	}
	ISPressed(KEY_ENTER) {
		
		try {
			switch (AnimTempCounter) {
				case 0:
				{
					TempAnim.FrameAmmountX = std::stoi(oss.str());
					oss.str("");
					AnimTempCounter++;
					break;
				}
				case 1:
				{
					TempAnim.FrameAmmountY = std::stoi(oss.str());
					oss.str("");
					AnimTempCounter++;
					break;
				}
				case 2:
				{
					TempAnim.FrameSizeX = std::stoi(oss.str());
					oss.str("");
					AnimTempCounter++;
					break;
				}
				case 3:
				{
					TempAnim.FrameSizeY = std::stoi(oss.str());
					oss.str("");
					AnimTempCounter++;
					break;
				}
			}
			
		}
		catch (...)
		{
			oss.str("");
		}
		WND1.Klt.FlushChar();
		WND1.Klt.ClearState();
	}
	ISPressed(KEY_TAB)
	{
		AnimHolder.Animations[AnimationIndex].ScaleHeight = ScaleDirection * ScaleTheight;
		AnimHolder.Animations[AnimationIndex].ScaleWidth = ScaleDirection * ScaleTwidth;
		WND1.Klt.ClearState();
	}
	ISPressed(KEY_I)
	{
		AudioHolder[AudioCounter].Loop = !AudioHolder[AudioCounter].Loop;
		WND1.Klt.ClearState();
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
		TrigerBoxHolder.clear();
		AudioCounter = -1;
		TrigerBoxCounter=-1;
		std::vector<bool> IsLoopedVector;
		Currentlevel.LoadLevel(TextureHolder,AnimHolder, AudioPathHolder,IsLoopedVector, selectedFilePath, CurrentPlayer,TrigerBoxHolder);
		//Animacje
		for (auto& Files : AnimHolder.Animations)
		{
			Files.InitializeAnimation(AnimHolder, WND1.ReturnGFX().ReturnRenderTarget(), Files.SpreadSheetPath); AnimationIndex++;
		}
		//if (!std::filesystem::exists(CurrentPlayer.CurrentPlayerTexture.Path)) { Zakomentowane z powodu w jaki teraz wczytuje texture, inaczej wczytywa³o by coœ innego
			if (AnimHolder.AnimationFrames.size() != 0)
				if (AnimHolder.AnimationFrames[0].size() != 0)
					CurrentPlayer.CurrentPlayerTexture = AnimHolder.AnimationFrames[0][0];
		//}
		//else
		//{
		//	LoadBMPToTexture(CurrentPlayer.CurrentPlayerTexture.Path, WND1.ReturnGFX().ReturnRenderTarget(), CurrentPlayer.CurrentPlayerTexture.pBitmap.GetAddressOf());
		//}

		size_t size = TextureHolder.size();
		int i = 0;
		for (TextureCounter = 0; i < size;  TextureCounter++)
		{
			if (!LoadBMPToTexture(TextureHolder[TextureCounter].Path, WND1.ReturnGFX().ReturnRenderTarget(), TextureHolder[TextureCounter].pBitmap.GetAddressOf()))
			{
				TextureHolder.erase(TextureHolder.begin());
				TextureCounter--;
			}
			i++;
		}
		if (TextureHolder.size() == 0)
			TextureCounter = -1;
		else
			TextureCounter = 0;
		//textury

		//audio
		AudioHolder.clear();

		i = 0;
		for (auto& Path : AudioPathHolder)
		{
			if (std::filesystem::exists(Path))
			{
				Sound s(Path);			
				AudioHolder.push_back(s);
				AudioHolder[i].Loop= IsLoopedVector[i];
				//po tym jak wyjdziemy ze scopa to destruktor dla s automatycznie siê uruchamia, a audioholder przechowuje kopie

			}
			i++;

		}
		if(AudioHolder.size()!=0)
		AudioCounter = 0;
		//audio
		if (AnimHolder.AnimationFrames.size() != 0)
			AnimationIndex = 0;
		//animacje
		if(CurrentPlayer.CurrentPlayerTexture.pBitmap)
			CollisionRect = D2D1::RectF(CurrentPlayer.PlayerRect.left + 33, CurrentPlayer.PlayerRect.top, CurrentPlayer.PlayerRect.right - 33, CurrentPlayer.PlayerRect.bottom); //sprawdzanie kolizji nie jest prowadzone na player rectie w celu mozliwosci jej korekcji
		
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
	std::vector<bool> Loops;
	for (int i = 0; i < AudioHolder.size(); i++)
		Loops.push_back(AudioHolder[i].Loop);
	Currentlevel.SaveLevel(TextureHolder,AnimHolder,AudioPathHolder,Loops, selectedFilePath, CurrentPlayer,TrigerBoxHolder);
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
		{
			StartWalkLeftAnimation = false;
			StartWalkRightAnimation = false;
		}
		HandleInput();
		Rollback = TextureHolder;
		UpdateCameraPosition();
		if (IsJumping)
			Jump();
		else
		{
			UpdateGravityAbsolute(); // updatuje grawitacje gdy po prostu schodzimy z bloku
		}
		 if(HadEnoughOfJumping){
			if (GravityChanged)
				UpdateGravity();

		}

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
void Mainapp::PlayPlayerAnimation(int StartFrame, int EndFrame) //¿eby wszystko dzia³a³o normalnie po prostu wszystkie boole sprawdzaj¹ce czy animacja jest odpalona musz¹ byæ falsowane tutaj 
{		
	//, jak zbierze siê ich na tyle ¿e by³o by ich za du¿o to zmienimy to na inta wybieraj¹cego i tutaj tylko bedzie go zmieniac na -1
	try {

		if (AnimHolder.Animations[AnimationIndex].CurrentFrame > EndFrame || AnimHolder.Animations[AnimationIndex].CurrentFrame < StartFrame)
			AnimHolder.Animations[AnimationIndex].CurrentFrame = StartFrame;
		if (AnimHolder.Animations[AnimationIndex].AnimationTimer.Peek() >= 1.0f / 10.0f)
		{
			
			AnimHolder.Animations[AnimationIndex].CurrentFrame++;
			AnimHolder.Animations[AnimationIndex].AnimationTimer.Mark();
			if (AnimHolder.Animations[AnimationIndex].CurrentFrame > EndFrame)
			{
				if (AnimationIndex == 1 || AnimationIndex == 2)
				{
					AudioHolder[2].Play(1.0f, 0.3f);
				}
				AnimHolder.Animations[AnimationIndex].CurrentFrame = StartFrame;
				StartJumpAnimation = false;
				StartWalkLeftAnimation = false;
				StartWalkRightAnimation = false;
				

			}
		}
		CurrentPlayer.CurrentPlayerTexture = AnimHolder.AnimationFrames[AnimationIndex][AnimHolder.Animations[AnimationIndex].CurrentFrame];
		CurrentPlayer.CurrentPlayerTexture.Twidth += AnimHolder.Animations[AnimationIndex].ScaleWidth;
		CurrentPlayer.CurrentPlayerTexture.Theight += AnimHolder.Animations[AnimationIndex].ScaleHeight;
		
		CurrentPlayer.PlayerRect = D2D1::RectF(
			ScreenWidth / 2 - (CurrentPlayer.CurrentPlayerTexture.Twidth) / 2,
			ScreenHeight / 2 - (CurrentPlayer.CurrentPlayerTexture.Theight) / 2,
			ScreenWidth / 2 + (CurrentPlayer.CurrentPlayerTexture.Twidth) / 2,
			ScreenHeight / 2 + (CurrentPlayer.CurrentPlayerTexture.Theight) / 2
		);
	}
	catch (...)
	{

	}
}

void Mainapp::DoDrawing()
{
	WND1.ReturnGFX().BeginFrame();
	WND1.ReturnGFX().ClearBuffer(0, 0, 0);

	//kolor t³a
//	WND1.ReturnGFX().ReturnRenderTarget()->FillRectangle(&Background, BackgroundColour);
//	WND1.ReturnGFX().ReturnRenderTarget()->DrawRectangle(&Background, BackgroundColour);
	//kolor t³a //zakomentuj by zmieniæ na czarny
	if (LoadAnimation) {

		if (!WND1.Klt.CharIsEmpty()) {
			oss << WND1.Klt.ReadChar();
			
		}
		switch (AnimTempCounter)
		{

			case 0:
			{
				Write("Podaj ilosc klatek w osi X", 300, 300);
				break;

			}
			case 1:
			{
				Write("Podaj ilosc klatek w osi Y", 300, 300);
				break;

			}
			case 2:
			{
				Write("Podaj Rozmiar klatek w osi X", 300, 300);
				break;

			}
			case 3:
			{
				Write("Podaj Rozmiar klatek w osi Y", 300, 300);
				break;

			}
			case 4:
			{
				LoadAnimation = false;
				AnimHolder.Animations.push_back(TempAnim);
				AnimHolder.Animations[AnimHolder.Animations.size() - 1].InitializeAnimation(AnimHolder, WND1.ReturnGFX().ReturnRenderTarget(), TempAnim.SpreadSheetPath);
				AnimationIndex++;
				AnimTempCounter = 0;
				break;
			}
		}		
	}

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
	case MODE_PLACE:
	{
		Write("Place_Mode", 0, 0);
		break;
	}
	case MODE_TRIGERS:
	{
		if (TrigerBoxHolder.size()!=0)
		{
			ID2D1SolidColorBrush* pBrushB = nullptr;
			ID2D1SolidColorBrush* pBrushR = nullptr;
			WND1.ReturnGFX().ReturnRenderTarget()->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Blue),
				&pBrushB
			);
			WND1.ReturnGFX().ReturnRenderTarget()->CreateSolidColorBrush(
				D2D1::ColorF(D2D1::ColorF::Red),
				&pBrushR
			);
			for (int i = 0; i < TrigerBoxHolder.size(); i++)
			{		
				WND1.ReturnGFX().ReturnRenderTarget()->FillRectangle(TrigerBoxHolder[i].TrigerBoxPosition, pBrushB);
				if (i == TrigerBoxCounter)
					WND1.ReturnGFX().ReturnRenderTarget()->FillRectangle(TrigerBoxHolder[i].TrigerBoxPosition, pBrushR);
			}
		}
		std::ostringstream oss;
		oss << "Selected TrigerBox nr "<<TrigerBoxCounter;
		Write("Triger_Mode", 0, 0);
		if(TrigerBoxCounter!=-1)
		Write(oss.str(), 0, 60);
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
					if (czyrysowaclinie)
						WND1.ReturnGFX().Draw(MousePosition, MousePosition);
				if (TextureCounter != -1)
				{
					D2D1_RECT_F destinationRect = D2D1::RectF(WND1.Mk.GetPosX(), WND1.Mk.GetPosY(), TextureHolder[TextureCounter].Twidth + WND1.Mk.GetPosX() + ScaleTwidth,
						TextureHolder[TextureCounter].Theight + WND1.Mk.GetPosY() + ScaleTheight);
					TextureHolder[TextureCounter].destinationRectTab.push_back(destinationRect);
				}
				, else if (czyrysowaclinie) //else if jest po przecinku z powodu struktury makra
					WND1.ReturnGFX().Draw(MousePosition);
				)
			}
	}
	
	if (StartJumpAnimation) {
		PlayPlayerAnimation(0, 5);

	}
	else
	{
		switch (AnimationRollback) //Rollback s³u¿y do powrotu do startowej pozycji w przypadku przerwania imputu
		{
			/*case BOTTOM: {
				PlayPlayerAnimation(0, 14);
				break;
			}*/
		case LEFT: {
			if (!StartWalkLeftAnimation) {
				AnimationIndex = 5;
				PlayPlayerAnimation(0, 3);
			}
			break;
		}
		case RIGHT: {
			if (!StartWalkRightAnimation) {
				AnimationIndex = 4;
				PlayPlayerAnimation(0, 3);
			}
			break;
		}



		}
	}
	if (CurrentPlayer.CurrentPlayerTexture.pBitmap) //jak textura gracza jest to rysuj
	{
		
				WND1.ReturnGFX().ReturnRenderTarget()->DrawBitmap(CurrentPlayer.CurrentPlayerTexture.pBitmap.Get(), CurrentPlayer.PlayerRect);
			


	}
	if (TextureHolder.size()!= 0)
	{
		
		
		
		
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
void Mainapp::UpdateGravityAbsolute()
{
	
	bool Gravitas = true;
	for (auto& texture : TextureHolder)
	{
		for (auto& rect : texture.destinationRectTab)
		{
			if (texture.IsCollisionOn) {
				if (IFColisionWithSides(CollisionRect, rect) == TOP)
				{
					TextureHolder = Rollback;
					Gravitas = false;
					break;
				}
			}
		}

	}
	if (Gravitas) {
		for (auto& texture : TextureHolder)
		{
			Rollback = TextureHolder;
				for (auto& rect : texture.destinationRectTab)
				{
					rect.top -= GravitySpeed;
					rect.bottom -= GravitySpeed;
					if (texture.IsCollisionOn) {
						if (IFColision(CollisionRect, rect)) //po l¹dowaniu na bloku przestañ œci¹gaæ gracza w dó³ i pozwól mu na ponowny skok
						{
							CurrentJumpHeight = 0;
							IsJumping = false;
							HadEnoughOfJumping = false;
							TextureHolder = Rollback;
							break;
						}
					}
				}
			
		}

	}
}
void Mainapp::UpdateGravity()
{
	

	for (auto& texture : TextureHolder)
	{
		for (auto& rect : texture.destinationRectTab)
		{
			if (texture.IsCollisionOn) {
				if (IFColisionWithSides(CollisionRect, rect) == TOP)
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
						if (IFColision(CollisionRect, rect)) //po l¹dowaniu na bloku przestañ œci¹gaæ gracza w dó³ i pozwól mu na ponowny skok
						{
							CurrentJumpHeight = 0;
							IsJumping = false;
							HadEnoughOfJumping = false;
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
				if (IFColision(rect, CollisionRect))
				{
					CurrentJumpHeight = MaxJumpHeight;
					TextureHolder = Rollback;
					break;
				}

			}

		}
		CurrentJumpHeight += 10;
	}
	else
	{
		HadEnoughOfJumping = true;
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
		
		for (auto& texture : TextureHolder)
		{
			for (int i = 0; i < texture.destinationRectTab.size(); i++)
			{
				if (CameraXState) {

					texture.destinationRectTab[i].left += CameraXPosition;
					texture.destinationRectTab[i].right += CameraXPosition;
					if (texture.IsCollisionOn) { // dodane z powodu du¿ej pustej przestrzeni w player rectie, po prostu zmienilem kolizje by sie zgadzala z faktycznym stanem rzeczy
						if (IFColision(CollisionRect, texture.destinationRectTab[i]))
						{
							Collision = true;
							break;
						}
					}

				}
				if (!IsJumping) {
					if (CameraYState) {
						texture.destinationRectTab[i].top += CameraYPosition;
						texture.destinationRectTab[i].bottom += CameraYPosition;
						if (texture.IsCollisionOn) {
							if (IFColision(CollisionRect, texture.destinationRectTab[i]))
							{
								Collision = true;
								break;
							}
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

bool Mainapp::LoadBMPToTexture(const std::wstring& filePath, Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, ID2D1Bitmap** ppBitmap)
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
								return true;
							}
						}
					}
				}
			}
		}
	}
	return false;
}

bool Mainapp::LoadBMPSubregionToTexture(const std::wstring& filePath, Microsoft::WRL::ComPtr<ID2D1HwndRenderTarget> pRenderTarget, const D2D1_RECT_F& sourceRegion, std::vector< Microsoft::WRL::ComPtr<ID2D1Bitmap>>& ppBitmap) const
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
								return true;

							}
						}
					}
				}
			}
		}
	}
	return false;
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




