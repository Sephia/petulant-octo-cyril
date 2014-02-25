// CreditState.h

#pragma once

#include "state.h"
#include "SpriteManager.h"

class Background;

class CreditState :
	public State
{
public:
	CreditState();
	~CreditState();

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
	sf::View* mp_view;
	SpriteManager m_spriteManager;
	Background* mp_background;
	
};

