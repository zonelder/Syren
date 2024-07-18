#pragma once
#include "InputHandler.h"

class Input
{
public:
	Input(const InputHandler& ih);

	bool IsKeyDown(unsigned char) const noexcept;

public:
	
	/// @brief Mouse X - position (along horizontal axis from left to right)
	int x;
	/// @brief Mouse Y - position (along vertical axis from up to down)
	int y;

	/// @brief mouse X-position normed by window widht
	float normedX;
	/// @brief mouse Y-position normed by window height
	float normedY;

	/// @brief  Get mouse X-cordinate changes from last frame
	int deltaX;
	/// @brief Get mouse Y-cordinate changes from last frame
	int deltaY;


	bool isLeftPressed;
	bool isRightPressed;

private:
	const InputHandler* p_ih = nullptr;
};
