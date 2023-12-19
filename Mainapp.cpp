#include "Mainapp.h"

// DZIALA 

Mainapp::Mainapp()
	: 
	okno1(820, 640, "testing") // ustawianie parametrów okna
{}

int Mainapp::Go()
{

	
	while ((result = GetMessageA(&msg, nullptr, 0, 0) > 0)) //petla obslugujaca wiadomosci przychodzace
	{
		TranslateMessage(&msg);
		DispatchMessageA(&msg);
		while (!okno1.Mk.IsEmpty())
		{
			const auto e = okno1.Mk.Read();
			if (e.GetType() == Myszka::Event::Type::Move)
			{
				std::ostringstream oss;
				oss << e.GetPosX() << "," << e.GetPosY();			// do stringstreama dodajemy wspó³rzêdne gdzie lpm zosta³ wciœniêty

				SetWindowTextA(*okno1.hwndcopy, oss.str().c_str());
			}
		}
		if (okno1.Klt.KeyIsPressed(0x51))
		{
			SetWindowTextA(*okno1.hwndcopy, "jak naciskam Q to sie zmienia na ta");
			okno1.Klt.ClearState(); // wymagane, inaczej przycisk jest uznawany za "Wiecznie wcisniety"
		}


		DoFrame();
	}
	return msg.wParam;
}

void Mainapp::DoFrame()
{
	okno1.grafika().ClearBuffer(255, 0, 0); // czerwono mi
	okno1.grafika().EndFrame();
}
