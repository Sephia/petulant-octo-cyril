// OptionsState.h

#pragma once

#include "state.h"

class OptionsState :
	public State
{
public:
	OptionsState();
	~OptionsState();

	void Enter();
	void Exit();
	bool Update();
	void Draw();
	std::string Next();
	bool IsType(const std::string &c_type);
	void Reset();

private:
	std::string m_nextState;
};

