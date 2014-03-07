// InputManager.cpp

#include "InputManager.h"
#include "stdafx.h"

InputManager::InputManager() {
	for(int i = 0; i < 256; i++) {
		m_keyboard_current[i] = false;
		m_keyboard_previous[i] = false;
	}
	for(int i = 0; i < MB_COUNT; i++) {
		m_mouse_current[i] = false;
		m_mouse_previous[i] = false;
	}
}

InputManager::~InputManager() {

}

bool InputManager::IsDownKeyboard(int key) const {
	return m_keyboard_current[key];
}

bool InputManager::IsDownOnceKeyboard(int key) const {
	return m_keyboard_current[key] && !m_keyboard_previous[key];
}

void InputManager::PostUpdate() {
	for(int i = 0; i < 256; i++) {
		m_keyboard_previous[i] = m_keyboard_current[i];
	}

	for(int i = 0; i < MB_COUNT; i++) {
		m_mouse_previous[i] = m_mouse_current[i];
	}
}

bool InputManager::IsDownMouse(EMouseButton button) const {
	return m_mouse_current[button];
}

bool InputManager::IsDownOnceMouse(EMouseButton button) const {
	return m_mouse_current[button] && !m_mouse_previous[button];
}
