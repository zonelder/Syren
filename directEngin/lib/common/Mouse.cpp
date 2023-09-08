#include "Mouse.h"



Mouse::Mouse(){}
Mouse& Mouse::GetInstance()
{
	static Mouse m;

	return m;
}


int Mouse::GetDeltaX() noexcept
{
	return GetInstance()._dx;
}

int Mouse::GetDeltaY() noexcept
{
	return GetInstance()._dy;
}

int Mouse::GetX() noexcept
{
	return GetInstance()._dx;
}

int Mouse::GetY() noexcept
{
	return GetInstance()._dy;
}

bool Mouse::LeftKeyDown()
{


	return GetInstance()._isLeftPressed;
}

bool Mouse::RightKeyDown()
{
	return GetInstance()._isRightPressed;
}


float Mouse::GetNormedX() noexcept
{
	return GetInstance()._clampX;
}

float Mouse::GetNormedY() noexcept
{
	return GetInstance()._clampY;
}