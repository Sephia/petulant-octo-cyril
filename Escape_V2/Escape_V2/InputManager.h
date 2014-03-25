// InputManager.h

#pragma once

#include "stdafx.h"

enum EMouseButton{
	LEFT,
	RIGHT,
	MB_COUNT
};

class InputManager {
public:
	InputManager();
	~InputManager();

	bool IsDownKeyboard(int key) const;
	bool IsDownOnceKeyboard(int key) const;

	bool IsDownMouse(EMouseButton button) const;
	bool IsDownOnceMouse(EMouseButton button) const;

	void PostUpdate();
	void ClearPresses();

	bool m_keyboard_current[256];
	bool m_keyboard_previous[256];
	bool m_mouse_current[MB_COUNT];
	bool m_mouse_previous[MB_COUNT];


private:


};
