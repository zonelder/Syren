#include "MouseHandler.h"
#include "../../Window.h"


void MouseHandler::trimBuffer() noexcept {
	while (_buffer.size() > _bufferSize) {
		_buffer.pop();
	}
}

bool MouseHandler::isInWindow() const noexcept {
	return _isInWindow;
}
void MouseHandler::onLeftPressed(int x,int y) noexcept{
	_leftIsPressed = true;
	_buffer.push(MouseHandler::Event(MouseHandler::Event::Type::LPress, *this));
	trimBuffer();
}

void MouseHandler::onLeftReleased(int x, int y) noexcept {
	_leftIsPressed = false;
	_buffer.push(MouseHandler::Event(MouseHandler::Event::Type::LRelease, *this));
	trimBuffer();
}

void MouseHandler::onRightPressed(int x, int y) noexcept {
	_rightIsPressed = true;
	_buffer.push(MouseHandler::Event(MouseHandler::Event::Type::RPress, *this));
	trimBuffer();
}

void MouseHandler::onRightReleased(int x, int y) noexcept {
	_rightIsPressed = false;
	_buffer.push(MouseHandler::Event(MouseHandler::Event::Type::RRelease, *this));
	trimBuffer();
}

void MouseHandler::onWheelDown(int x, int y) noexcept {

	_buffer.push(MouseHandler::Event(MouseHandler::Event::Type::WheelDown, *this));
	trimBuffer();
}

void MouseHandler::onWheelUp(int x, int y) noexcept {

	_buffer.push(MouseHandler::Event(MouseHandler::Event::Type::WheelUp, *this));
	trimBuffer();
}

void MouseHandler::onMouseMove(int x,int y) noexcept{
	_x = x;
	_y = y;
	_buffer.push(MouseHandler::Event(MouseHandler::Event::Type::Move, *this));
	trimBuffer();
}



void MouseHandler::onMouseLeave() noexcept {
	_isInWindow = false;
	_buffer.push(MouseHandler::Event(MouseHandler::Event::Type::Leave, *this));
	trimBuffer();

}
void MouseHandler::onMouseEnter() noexcept {
	_isInWindow = true;
	_buffer.push(MouseHandler::Event(MouseHandler::Event::Type::Enter, *this));
	trimBuffer();

}
 MouseHandler::Event MouseHandler::read() noexcept{
	if (_buffer.size() > 0) {
		MouseHandler::Event e = _buffer.front();
		_buffer.pop();
		return e;
	}

	return  MouseHandler::Event();
}

 void MouseHandler::flush() noexcept {
	 _buffer = std::queue<Event>();
 }

 Vec2 MouseHandler::getPos() const noexcept{

	 return Vec2(_x, _y);
 }


 void MouseHandler::onWheelDelta(int x,int y,int delta) noexcept{
	 _wheelDeltaCarry += delta;
	 //genera event for every WHEEL_DELTA rotation
	 while (_wheelDeltaCarry >= WHEEL_DELTA) {
		 _wheelDeltaCarry -= WHEEL_DELTA;
		 onWheelUp(x, y);
	 }
	 while (_wheelDeltaCarry <= -WHEEL_DELTA) {
		 _wheelDeltaCarry += WHEEL_DELTA;
		 onWheelDown(x, y);
	 }
 }
