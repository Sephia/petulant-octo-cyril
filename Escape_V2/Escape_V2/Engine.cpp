// Engine.cpp

#include "engine.h"
#include "GameState.h"
#include "LoadingState.h"
#include "StartMenuState.h"
#include "OptionsState.h"
#include "StateManager.h"
#include "CreditState.h"
#include "VictoryState.h"
#include "GameOverState.h"
#include "Settings.h"
#include "stdafx.h"


Engine::Engine()
{
    mp_stateManager = nullptr;
}

Engine::~Engine()
{
    //flyttad från Cleanup()
    if(mp_stateManager != nullptr)
    {
		mp_stateManager->CleanUp();
        delete mp_stateManager;
        mp_stateManager = nullptr;
    }
}

bool Engine::Initialize()
{
    //skapa en ny state manager om det inte finns någon
    if(mp_stateManager==nullptr)
    {
        mp_stateManager = new StateManager();
        if(mp_stateManager==nullptr)
        {
            return false;
        }
        mp_stateManager->Attach(new StartMenuState());
		mp_stateManager->Attach(new GameState());
		mp_stateManager->Attach(new OptionsState());
		mp_stateManager->Attach(new CreditState());
		mp_stateManager->Attach(new LoadingState());
		mp_stateManager->Attach(new VictoryState());
		mp_stateManager->Attach(new GameOverState());
        mp_stateManager->SetState("LoadingState");
        //lägg till fler states med tiden, LoadingState kommer vara den första som körs senare
    }
    return true;
}

void Engine::Run()
{
    //game loopen, alternativt lägg till Draw() i engine
    while(mp_stateManager->IsRunning())
    {
		Settings::UpdateDeltatime();
        if(mp_stateManager->Update()) {
			mp_stateManager->Draw();
		}
		sf::Time time = sf::milliseconds(10);
		sf::sleep(time);
    }
	
}