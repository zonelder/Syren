#include "input_handler.h"
#include <iostream>

bool InputHandler::isPressed(unsigned char keycode) const noexcept
{
	return _keyStates[keycode];
}

InputHandler::Event InputHandler::readKey() noexcept 
{
	if (_keyBuffer.size() > 0) {
		InputHandler::Event e = _keyBuffer.front();
		_keyBuffer.pop();
		return e;
	}

	return InputHandler::Event();
}

bool InputHandler::isEmpty() const noexcept
{
	return _keyBuffer.empty();
}

char InputHandler::readChar() noexcept
{
	if (_charBuffer.size() > 0) {
		unsigned char charCode = _charBuffer.front();
		_charBuffer.pop();
		return charCode;
	}

	return 0;
}


bool InputHandler::charIdEmpty() const noexcept
{
	return _charBuffer.empty();
}

void InputHandler::flushKey() noexcept
{
	_keyBuffer = std::queue<Event>();
}

void InputHandler::flushChar() noexcept
{
	_charBuffer = std::queue<char>();
}

void InputHandler::flush() noexcept
{
	flushChar();
	flushKey();
}

void InputHandler::enableAutorepeat() noexcept
{
	_autorepeatIsEnabled = true;
}

void InputHandler::disaableAutorepeat() noexcept
{
	_autorepeatIsEnabled = false;
}
bool InputHandler::isEnabledAutorepeat() noexcept
{
	return _autorepeatIsEnabled;
}

void InputHandler::onkeyPressed(unsigned char keycode) noexcept
{

	_keyStates[keycode] = true;
	_keyBuffer.push(InputHandler::Event(InputHandler::Event::Type::Press, keycode));
	trimBuffer(_keyBuffer);
}

void InputHandler::onKeyReleased(unsigned char keycode) noexcept
{
	_keyStates[keycode] = false;
	_keyBuffer.push(InputHandler::Event(InputHandler::Event::Type::Release, keycode));
	trimBuffer(_keyBuffer);
}

void InputHandler::onChar(char cha) noexcept
{
	_charBuffer.push(cha);
	trimBuffer(_charBuffer);
}

void InputHandler::clearState() noexcept
{
	_keyStates.reset();
}
template<typename T>
void InputHandler::trimBuffer(std::queue<T>& buffer) noexcept
{
	while (buffer.size() > _bufferSize)
	{
		buffer.pop();
	}
}
