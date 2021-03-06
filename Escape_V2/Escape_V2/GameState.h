// GameState.h

#pragma once

#include "stdafx.h"
#include "SpriteManager.h"
#include "State.h"
#include "SoundRippleManager.h"

class Grid2D;
class PathFinding;
class PlayerObject;
class Guard;
class WallManager;
class HullManager;
class CollisionManager;
class LightManager;
class FurnitureManager;
class KeyManager;
class DoorManager;
class GUI;
class FoV;
class GuardFootSteps;
class Level;
class MusicEntity;

namespace ltbl {
	class LightSystem;
};

namespace sf {
	class View;
};


class GameState :
	public State
{
public:
	GameState();
	~GameState();

	void Enter();
	void Exit();
	bool Update();
	void Draw();
	std::string Next();
	bool IsType(const std::string &c_type);
	void Reset();

private:
	bool UpdateEvents();


private:
	std::string m_nextState;

	Level* mp_level;

	PlayerObject* mp_player;
	std::vector<Guard*> m_guards;

	GuardFootSteps* mp_guardFootSteps;

	SpriteManager m_spriteManager;
	SoundRippleManager m_soundRippleManager;

	MusicEntity* mp_music_undetected;
	MusicEntity* mp_music_detected;
	sf::Sound* mp_keySound;
	sf::Sound* mp_unlock;
	sf::Sound* mp_death;
	sf::Sound* mp_jokeDeath;

	bool m_deathSoundPlayed;

	float m_timerGuards;
	float m_timerPlayer;
	float m_gameOverTimer;
	float m_bumpTimer;		//Only create a new sound from bumping into furniures after a certain time from the last

	int m_levelToLoad;
	std::string m_pathToLevel;

	sf::View* mp_view;

	LightManager* lm;
	HullManager* hl;
	WallManager* wl;
	DoorManager* dm;
	KeyManager* km;
	FurnitureManager* fm;
	CollisionManager* cl;
	ltbl::LightSystem* ls;

	Grid2D* mp_grid;
	PathFinding* mp_finding;

	GUI* mp_gui;
	FoV* mp_fov;

	sf::Texture loadingScreenTexture;
	sf::Sprite* loadingScreenSprite;
};
