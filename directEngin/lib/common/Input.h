#pragma once
#include "App.h"

class Input
{
	friend class App;
public:

	/// @brief  Get mouse x-cordinate changes from last frame
	static int GetDeltaX() noexcept;

	/// @brief Get mouse Y-cordinate changes from last frame
	static int GetDeltaY() noexcept;

	/// @brief Get Mouse position along horizontal axis(from left to right) 
	static int GetX() noexcept;

	/// @brief Get Mouse position along vertical axis(from up to down) 
	static int GetY() noexcept;


	static float GetNormedX() noexcept;

	static float GetNormedY() noexcept;

	static bool LeftKeyDown();
	static bool RightKeyDown();

private:
	static Input& GetInstance();
	Input();
	int _x, _y;
	float _clampX, _clampY;
	int _dx, _dy;
	bool _isLeftPressed;
	bool _isRightPressed;

};
