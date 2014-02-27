// Settings.h

#pragma once

#include "stdafx.h"
#include "InputManager.h"

class Settings {
public:
	Settings();
	~Settings();

	static void Load();
	static void UpdateDeltatime();
	static void SetFullscreen();
	static void SetWindowed();

	static std::map<int, std::vector<sf::Vector2f>> m_allGuardWaypoints;
	static std::vector<sf::Vector2f> ms_guards;
	static sf::RenderWindow *ms_window;
	static float ms_deltatime;
	static bool ms_gameOver;
	static sf::Vector2f ms_enter;
	static sf::Vector2f ms_exit;
	static InputManager ms_inputManager;

private:
	static clock_t ms_oldTime;
	static clock_t ms_newTime;
};

