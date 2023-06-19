#pragma once
#include <windows.h>
#include <string>
//#include "Window.h"
#include <queue>
#include <bitset>
enum MOUSE_KEY
{
	LEFT,
	RIGHT,
};

class InputHandler
{
	friend class Window;
public:
	class Event {

	public:
		enum class Type {
			Press,
			Release,
			Invalid,
		};
	private:
		Type _type;
		unsigned char _code;
	public:
		Event() :_type(Type::Invalid), _code(0u) {};
		Event(Type type, unsigned char code) noexcept :_type(type), _code(code) {};
		bool isPress() const noexcept {

			return _type == Type::Press;
		}
		bool isRelease() const noexcept {
			return _type == Type::Release;
		}
		bool isValid() const noexcept {

			return _type != Type::Invalid;
		}

		unsigned char getCode() const noexcept {
			return _code;
		}
	};
public:

	InputHandler() = default;
	InputHandler(const InputHandler&) = delete;
	InputHandler& operator=(const InputHandler&) = delete;

	//< key event stuff>
	bool isPressed(unsigned char keykode) const noexcept;
	Event readKey() noexcept;
	bool isEmpty() const noexcept;
	void flushKey() noexcept;
	//< key event stuff>

	//< Char event stuff>
	char readChar() noexcept;
	bool charIdEmpty() const noexcept;
	void flushChar() noexcept;
	void flush() noexcept;
	//< /Char event stuff>

	void enableAutorepeat() noexcept;
	void disaableAutorepeat() noexcept;
	bool isEnabledAutorepeat() noexcept;
private:
	void onkeyPressed(unsigned char keycode) noexcept;
	void onKeyReleased(unsigned char keycode) noexcept;
	void onChar(char cha) noexcept;
	void clearState()noexcept;
	template<typename T>
	static void trimBuffer(std::queue<T>& buffer) noexcept;

	static constexpr unsigned int _nKeys = 256u;
	static constexpr unsigned int _bufferSize = 16u;
	bool _autorepeatIsEnabled = false;
	std::bitset<_nKeys> _keyStates;
	std::queue<Event> _keyBuffer;
	std::queue<char> _charBuffer;
};

