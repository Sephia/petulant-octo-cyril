// StateManager.h

#pragma once

#include "stdafx.h"

class State;

class StateManager
{
public:
	StateManager();
	~StateManager();

	void Attach(State *p_state);
	bool Update();
	void Draw();
	void SetState(const std::string &c_type);
	void ChangeState();
	bool IsRunning();
	void Init();
	void CleanUp();
	

private:
	std::vector<State*> m_states;
	State *mp_current;

	bool m_quit;

};