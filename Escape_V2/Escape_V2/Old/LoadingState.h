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
};

