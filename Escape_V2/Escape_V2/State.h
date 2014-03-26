// State.h

#pragma once

#include "stdafx.h"

class State {
public:
	virtual ~State() {};

    virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual bool Update() = 0;
	virtual void Draw() = 0;
	virtual std::string Next() = 0;
	virtual bool IsType(const std::string &c_type) = 0;
	virtual void Reset() = 0;
};
