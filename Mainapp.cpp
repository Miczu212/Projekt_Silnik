#include "Mainapp.h"

// DZIALA 

int Mainapp::Go()
{

	Okno okno1(820, 640, "testing");
	while ((result = GetMessageA(&msg, nullptr, 0, 0) > 0))
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



	}
	return msg.wParam;
}

void Mainapp::DoFrame()
{

}
