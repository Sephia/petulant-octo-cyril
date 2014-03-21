// Settings.h

#pragma once

#include "stdafx.h"
#include "InputManager.h"
#include "SoundManager.h"

class Settings {
public:
	Settings();
	~Settings();

	static void Load(const std::string& c_path);
	static void UpdateDeltatime();
	static void SetFullscreen();
	static void SetWindowed();
	static void Shoot(sf::Vector2f from, sf::Vector2f to);
	static void DrawShot();
	static void ResetShot();

	static std::map<int, std::vector<sf::Vector2f>> m_allGuardWaypoints;
	static std::vector<sf::Vector2f> ms_guards;
	static sf::RenderWindow *ms_window;
	static float ms_deltatime;
	static bool ms_gameOver;
	static sf::Vector2f ms_enter;
	static sf::Vector2f ms_exit;
	static InputManager ms_inputManager;
	static SoundManager ms_soundManager;

private:
	static clock_t ms_oldTime;
	static clock_t ms_newTime;

	static bool ms_shoot;
	static sf::Vector2f ms_shootFrom;
	static sf::Vector2f ms_shootTo;
};

