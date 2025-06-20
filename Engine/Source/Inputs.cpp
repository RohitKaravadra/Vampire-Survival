#include "Engine.h"

using namespace Engine;

// static variable definition

Window* Inputs::win = nullptr;
Vector2 Inputs::mousePos(0, 0);

#pragma region Inputs Methods

void Inputs::Init(Window& _win)
{
	if (win == nullptr)
		Inputs::win = &_win;
}

void Inputs::destroy() { Inputs::win = nullptr; }

void Inputs::refresh()
{
	if (win == nullptr)
		return;

	win->checkInput();
	mousePos.set(win->getMouseX(), win->getMouseY());
}

int Inputs::get_h_axis()
{
	if (win == nullptr)
		return 0;

	int axis = 0;

	if (win->keyPressed('A') || win->keyPressed(VK_LEFT))
		axis--;
	if (win->keyPressed('D') || win->keyPressed(VK_RIGHT))
		axis++;

	return axis;
}

int Inputs::get_v_axis()
{
	if (win == nullptr)
		return 0;

	int axis = 0;

	if (win->keyPressed('W') || win->keyPressed(VK_UP))
		axis--;
	if (win->keyPressed('S') || win->keyPressed(VK_DOWN))
		axis++;

	return axis;
}

Vector2 Inputs::get_axis() { return Vector2(get_h_axis(), get_v_axis()).normalize(); }

Vector2 Inputs::get_mouse_pos() { return mousePos; }

bool Inputs::mouse_button(MouseButton _button) { return win->mouseButtonPressed(_button); }

int Inputs::mouse_wheel() { return win->getMouseWheel(); }

bool Inputs::key_pressed(int key)
{
	if (win == nullptr)
		return false;
	return win->keyPressed(key);
}

bool Inputs::ui_accept()
{
	if (win == nullptr)
		return false;
	return win->keyPressed('F') || win->keyPressed(VK_RETURN);
}

bool Inputs::ui_back()
{
	if (win == nullptr)
		return false;
	return win->keyPressed(VK_ESCAPE) || win->keyPressed(VK_BACK);
}

#pragma endregion
