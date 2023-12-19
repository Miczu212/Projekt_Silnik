#pragma once
#include <queue>
#include <bitset>

class Klawiatura
{
	friend class MainWindow;
public:
	class Event
	{
	public:
		enum Type
		{
			Press,
			Release,
			Invalid
		};
	private:
		Type type;
		unsigned char code;
	public:
		Event()
			:
			type(Invalid),
			code(0u)
		{}
		Event(Type type, unsigned char code)
			:
			type(type),
			code(code)
		{}
		bool IsPress() const
		{
			return type == Press;
		}
		bool IsRelease() const
		{
			return type == Release;
		}
		bool IsValid() const
		{
			return type != Invalid;
		}
		unsigned char GetCode() const
		{
			return code;
		}
	};
public:
	Klawiatura() = default;
	Klawiatura(const Klawiatura&) = delete;
	Klawiatura& operator=(const Klawiatura&) = delete;
	bool KeyIsPressed(unsigned char keycode) const;
	Event ReadKey();
	bool KeyIsEmpty() const;
	char ReadChar();
	bool CharIsEmpty() const;
	void FlushKey();
	void FlushChar();
	void Flush();
	void EnableAutorepeat();
	void DisableAutorepeat();
	bool AutorepeatIsEnabled() const;
	void OnKeyPressed(unsigned char keycode);
	void OnKeyReleased(unsigned char keycode);
	void OnChar(char character);
	void ClearState();
	template<typename T>
	void TrimBuffer(std::queue<T>& buffer);
private:
	static constexpr unsigned int nKeys = 256u; //ustawiamy rozmiar na 256 bitów poniewaz ilosc ró¿nych keycodów nie przekracza bajta
	static constexpr unsigned int bufferSize = 4u;
	bool autorepeatEnabled = false;
	std::bitset<nKeys> keystates;
	std::queue<Event> keybuffer;
	std::queue<Event> Clearbuffer;
	std::queue<char> charbuffer;
};
class Myszka
{
	friend class MainWindow;
public:
	class Event
	{
	public:
		enum Type
		{
			LPress,
			LRelease,
			RPress,
			RRelease,
			WheelUp,
			WheelDown,
			Move,
			Invalid
		};
	private:
		Type type;
		bool leftIsPressed;
		bool rightIsPressed;
		int x;
		int y;
	public:
		Event()
			:
			type(Invalid),
			leftIsPressed(false),
			rightIsPressed(false),
			x(0),
			y(0)
		{}
		Event(Type type, const Myszka& parent)
			:
			type(type),
			leftIsPressed(parent.leftIsPressed),
			rightIsPressed(parent.rightIsPressed),
			x(parent.x),
			y(parent.y)
		{}
		bool IsValid() const
		{
			return type != Invalid;
		}
		Type GetType() const
		{
			return type;
		}
		std::pair<int, int> GetPos() const
		{
			return{ x,y };
		}
		int GetPosX() const
		{
			return x;
		}
		int GetPosY() const
		{
			return y;
		}
		bool LeftIsPressed() const
		{
			return leftIsPressed;
		}
		bool RightIsPressed() const
		{
			return rightIsPressed;
		}
	};
public:
	Myszka() = default;
	Myszka(const Myszka&) = delete;
	Myszka& operator=(const Myszka&) = delete;
	std::pair<int, int> GetPos() const;
	int GetPosX() const;
	int GetPosY() const;
	bool LeftIsPressed() const;
	bool RightIsPressed() const;
	bool IsInWindow() const;
	Myszka::Event Read();
	bool IsEmpty() const
	{
		return buffer.empty();
	}
	void Flush();
	void OnMyszkaMove(int x, int y);
	void OnMyszkaLeave();
	void OnMyszkaEnter();
	void OnLeftPressed(int x, int y);
	void OnLeftReleased(int x, int y);
	void OnRightPressed(int x, int y);
	void OnRightReleased(int x, int y);
	void OnWheelUp(int x, int y);
	void OnWheelDown(int x, int y);
	void TrimBuffer();
private:
	static constexpr unsigned int bufferSize = 4u;
	int x;
	int y;
	bool leftIsPressed = false;
	bool rightIsPressed = false;
	bool isInWindow = false;
	std::queue<Event> buffer;
};