#include "Input.h"



Input::Input(){}
Input& Input::GetInstance()
{
	static Input m;

	return m;
}


int Input::GetDeltaX() noexcept
{
	return GetInstance()._dx;
}

int Input::GetDeltaY() noexcept
{
	return GetInstance()._dy;
}

int Input::GetX() noexcept
{
	return GetInstance()._dx;
}

int Input::GetY() noexcept
{
	return GetInstance()._dy;
}

bool Input::LeftKeyDown()
{


	return GetInstance()._isLeftPressed;
}

bool Input::RightKeyDown()
{
	return GetInstance()._isRightPressed;
}


float Input::GetNormedX() noexcept
{
	return GetInstance()._clampX;
}

float Input::GetNormedY() noexcept
{
	return GetInstance()._clampY;
}


bool Input::IsKeyDown(unsigned char keycode) noexcept
{
	return GetInstance().p_ih->isPressed(keycode);
}