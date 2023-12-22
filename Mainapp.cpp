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

void Mainapp::DoFrame() {
	std::random_device rd; //tecza ale boli w oczy
	std::mt19937 gen(rd());
	std::uniform_int_distribution<> dis(0, 10);
	int randomNumber = dis(gen);
	int randomNumber2 = dis(gen);
	int randomNumber3 = dis(gen);
	WND1.ReturnGFX().BeginFrame();
	WND1.ReturnGFX().ClearBuffer(randomNumber, randomNumber2, randomNumber3); // by wylaczyc tencze wstaw tu sta³e
	WND1.ReturnGFX().Draw();
	WND1.ReturnGFX().EndFrame();

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

				SetWindowTextA(*WND1.pHwnd, oss.str().c_str());
			}
		}
		if (WND1.Klt.KeyIsPressed(0x51))
		{
			SetWindowTextA(*WND1.pHwnd, "jak naciskam Q to sie zmienia na ta");
			WND1.Klt.ClearState(); // wymagane, inaczej przycisk jest uznawany za "Wiecznie wcisniety"
		}

	}
}
