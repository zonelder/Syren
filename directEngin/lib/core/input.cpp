#include "pch.h"
#include "Input.h"


Input::Input(const InputHandler& ih)
{
	p_ih = &ih;
}


bool Input::IsKeyDown(unsigned char keycode) const noexcept
{
	return p_ih->isPressed(keycode);
}