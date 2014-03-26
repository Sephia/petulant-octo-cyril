// GameOverState.h

#pragma once

#include "state.h"

class GameOverState :
	public State
{
public:
	GameOverState();
	~GameOverState();

	void Enter();
	void Exit();
	bool Update();
	void Draw();
	std::string Next();
	bool IsType(const std::string &c_type);
	void Reset();

	bool UpdateEvents();

private:
	std::string m_nextState;
	bool m_done;

	sf::Texture* mp_texture;
	sf::Sprite* mp_sprite;
};

