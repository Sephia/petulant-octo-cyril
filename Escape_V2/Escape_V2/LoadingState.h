// Loading State

#pragma once

#include "state.h"

class LoadingState :
	public State
{
public:
	LoadingState();
	~LoadingState();

	void Enter();
	void Exit();
	bool Update();
	void Draw();
	std::string Next();
	bool IsType(const std::string &c_type);
	void Reset();

private:
	std::string m_nextState;
	float m_timer;

	sf::Texture* mp_texture1;
	sf::Sprite* mp_sprite1;

	sf::Texture* mp_texture2;
	sf::Sprite* mp_sprite2;
};

