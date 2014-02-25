// Engine.h

#pragma once

#include "stdafx.h"

class StateManager;

class Engine
{
public:
    Engine();
    ~Engine();
    
    bool Initialize();
    void Run();
    //Cleanup flyttad till destructorn
    
private:
    StateManager *mp_stateManager;
};