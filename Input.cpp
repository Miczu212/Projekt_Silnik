#include "Input.h"
bool Klawiatura::KeyIsPressed(unsigned char keycode) const
{
	return keystates[keycode];
}

Klawiatura::Event Klawiatura::ReadKey()
{
	if (keybuffer.size() > 0u)
	{
		Klawiatura::Event e = keybuffer.front();
		keybuffer.pop();
		return e;
	}
	else
	{
		return Klawiatura::Event();
	}
}

bool Klawiatura::KeyIsEmpty() const
{
	return keybuffer.empty();
}

char Klawiatura::ReadChar()
{
	if (charbuffer.size() > 0u)
	{
		unsigned char charcode = charbuffer.front();
		charbuffer.pop();
		return charcode;
	}
	else
	{
		return 0;
	}
}

bool Klawiatura::CharIsEmpty() const
{
	return charbuffer.empty();
}

void Klawiatura::FlushKey()
{
	keybuffer = std::queue<Event>();
}

void Klawiatura::FlushChar()
{
	charbuffer = std::queue<char>();
}

void Klawiatura::Flush()
{
	FlushKey();
	FlushChar();
}

void Klawiatura::EnableAutorepeat()
{
	autorepeatEnabled = true;
}

void Klawiatura::DisableAutorepeat()
{
	autorepeatEnabled = false;
}

bool Klawiatura::AutorepeatIsEnabled() const
{
	return autorepeatEnabled;
}

void Klawiatura::OnKeyPressed(unsigned char keycode)
{
	keystates[keycode] = true;
	keybuffer.push(Klawiatura::Event(Klawiatura::Event::Press, keycode));
	TrimBuffer(keybuffer);
}

void Klawiatura::OnKeyReleased(unsigned char keycode)
{
	keystates[keycode] = false;
	keybuffer.push(Klawiatura::Event(Klawiatura::Event::Release, keycode));
	TrimBuffer(keybuffer);
}

void Klawiatura::OnChar(char character)
{
	charbuffer.push(character);
	TrimBuffer(charbuffer);
}

void Klawiatura::ClearState()
{
	keystates.reset();
	keybuffer = {};
}

template<typename T>
void Klawiatura::TrimBuffer(std::queue<T>& buffer)
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}
std::pair<int, int> Myszka::GetPos() const
{
	return { x,y };
}

int Myszka::GetPosX() const
{
	return x;
}

int Myszka::GetPosY() const
{
	return y;
}

bool Myszka::LeftIsPressed() const
{
	return leftIsPressed;
}

bool Myszka::RightIsPressed() const
{
	return rightIsPressed;
}

bool Myszka::IsInWindow() const
{
	return isInWindow;
}

Myszka::Event Myszka::Read()
{
	if (buffer.size() > 0u)
	{
		Myszka::Event e = buffer.front();
		buffer.pop();
		return e;
	}
	else
	{
		return Myszka::Event();
	}
}

void Myszka::Flush()
{
	buffer = std::queue<Event>();
}

void Myszka::OnMyszkaLeave()
{
	isInWindow = false;
}

void Myszka::OnMyszkaEnter()
{
	isInWindow = true;
}

void Myszka::OnMyszkaMove(int newx, int newy)
{
	x = newx;
	y = newy;

	buffer.push(Myszka::Event(Myszka::Event::Move, *this));
	TrimBuffer();
}

void Myszka::OnLeftPressed(int x, int y)
{
	leftIsPressed = true;

	buffer.push(Myszka::Event(Myszka::Event::LPress, *this));
	TrimBuffer();
}

void Myszka::OnLeftReleased(int x, int y)
{
	leftIsPressed = false;

	buffer.push(Myszka::Event(Myszka::Event::LRelease, *this));
	TrimBuffer();
}

void Myszka::OnRightPressed(int x, int y)
{
	rightIsPressed = true;

	buffer.push(Myszka::Event(Myszka::Event::RPress, *this));
	TrimBuffer();
}

void Myszka::OnRightReleased(int x, int y)
{
	rightIsPressed = false;

	buffer.push(Myszka::Event(Myszka::Event::RRelease, *this));
	TrimBuffer();
}

void Myszka::OnWheelUp(int x, int y)
{
	buffer.push(Myszka::Event(Myszka::Event::WheelUp, *this));
	TrimBuffer();
}

void Myszka::OnWheelDown(int x, int y)
{
	buffer.push(Myszka::Event(Myszka::Event::WheelDown, *this));
	TrimBuffer();
}

void Myszka::TrimBuffer()
{
	while (buffer.size() > bufferSize)
	{
		buffer.pop();
	}
}

