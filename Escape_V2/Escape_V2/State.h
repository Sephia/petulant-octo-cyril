// State.h

#pragma once

#include "stdafx.h"

namespace sf{
    class RenderWindow;
};

class State {
public:
	virtual ~State() {};

	//virtual bool Init() = 0; förslagsvis
    virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual bool Update() = 0;
	virtual void Draw() = 0;
	virtual std::string Next() = 0;
	virtual bool IsType(const std::string &c_type) = 0;
	virtual void Reset() = 0;

protected:
	//static sf::RenderWindow* ms_window;
    //deltatime är en global variabel alla states kommer använda, och bör uppdateras när skärmen uppdateras
	//bör vara public om du inte har en funktion som uppdaterar den (inte rekommenderat)//
    static float ms_deltatime;
};
