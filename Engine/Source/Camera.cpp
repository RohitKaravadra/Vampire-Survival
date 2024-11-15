#include "Engine.h"

namespace Engine
{
	// static variables definition
	bool Camera::notNull = false;
	Window Camera::win;
	Rect* Camera::followTarget = nullptr;
	Vector2 Camera::offset;
	Rect Camera::camRect;

	void Camera::create(std::string _name, Vector2 _size, Vector2 _pos)
	{
		if (notNull)// check if camera is already set
			return;
		notNull = true;
		win.create(_size.x, _size.y, _name);// create window
		camRect.set(_size, _pos);
		offset = _size / 2;
	}

	void Camera::destroy()
	{
		followTarget = nullptr;

		// clear screen
		win.clear();
		present();

		// reset inputs for next operations
		win.resetInput();
	}

	Window& Camera::get_window() { return win; }

	Vector2 Camera::world_to_screen(Vector2 _pos) { return offset + _pos - camRect.get_center(); }

	// does not work if windows size is changed
	Vector2 Camera::screen_to_world(Vector2 _pos) { return _pos + camRect.get_center() - offset; }

	bool Camera::has_follow_target() { return followTarget != nullptr; }

	void Camera::set_follow_target(Rect& rect) { followTarget = &rect; }

	void Camera::reset_follow_target() { followTarget = nullptr; }

	void Camera::update(float dt)
	{
		if (followTarget != nullptr && notNull)
			camRect.set_center(followTarget->get_center());
	}

	void Camera::clear()
	{
		if (notNull)
			win.clear();
	}

	void Camera::draw(Rect& _rect, Image& _image)
	{
		if (!notNull) // return if camera is not initialized
			return;

		if (!camRect.collide_as_rect(_rect)) // return if object is not in view
			return;

		Vector2 relPos = world_to_screen(_rect.get_topleft()); // relative position of object to camera

		for (unsigned int y = 0; y < _image.height; y++)
		{
			int posY = relPos.y + y; // y position of image pixel on window

			//bound check for y
			if (posY < 0)
				continue;
			if (posY > camRect.size.y)
				break;

			for (unsigned int x = 0; x < _image.width; x++)
			{
				int posX = relPos.x + x; // x position of image pixel on window

				//x bound check for x
				if (posX < 0)
					continue;
				if (posX > camRect.size.x - 1)
					break;

				//draw pixel if alpha is greater than 0
				if (_image.alphaAt(x, y) > 0)
					win.draw(posX, posY, _image.at(x, y));
			}
		}
	}

	void Camera::draw_ui(Vector2 _pos, Image& _image)
	{
		if (!notNull) // return if camera is not initialized
			return;

		for (unsigned int y = 0; y < _image.height; y++)
		{
			int posY = _pos.y + y; // y position of image pixel on window

			//bound check for y
			if (posY < 0)
				continue;
			if (posY > camRect.size.y)
				break;

			for (unsigned int x = 0; x < _image.width; x++)
			{
				int posX = _pos.x + x; // x position of image pixel on window

				//x bound check for x
				if (posX < 0)
					continue;
				if (posX > camRect.size.x - 1)
					break;

				//draw pixel if alpha is greater than 0
				if (_image.alphaAt(x, y) > 0)
					win.draw(posX, posY, _image.at(x, y));
			}
		}
	}

	void Camera::present() { win.present(); }
}