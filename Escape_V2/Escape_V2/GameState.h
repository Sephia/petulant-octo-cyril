// GameState.h

#pragma once

#include "stdafx.h"
#include "Level.h"
#include "SpriteManager.h"
#include "State.h"
#include "SoundRippleManager.h"

class PlayerObject;
class Guard;
class WallManager;
class HullManager;
class CollisionManager;
class LightManager;

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

	Level m_level;

	PlayerObject* mp_player;
	std::vector<Guard*> m_guards;

	SpriteManager m_spriteManager;
	SoundRippleManager m_soundRippleManager;

	sf::Music m_music;

	float m_timer;
	sf::View* mp_view;

	LightManager* lm;
	HullManager* hl;
	WallManager* wl;
	CollisionManager* cl;
	ltbl::LightSystem* ls;

	/*LightManager* lm2;
	HullManager* hl2;
	WallManager* wl2;
	CollisionManager* cl2;
	ltbl::LightSystem* ls2;*/

	
	//ltbl::Light_Point* testLight3;
	
};
