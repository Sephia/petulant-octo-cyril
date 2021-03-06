// StateMenuState.h

#pragma once
#include <SFML\Graphics.hpp>
#include "SoundManager.h"
#include "state.h"
#include "stdafx.h"
#include "Settings.h"

class StartMenuState :
	public State
{
public:
	StartMenuState();
	~StartMenuState();

	void Enter();
	void Exit();
	bool Update();
	void Draw();
	std::string Next();
	bool IsType(const std::string &c_type);
	void Reset();

	void UpdateEvents();

private:
	//Start Menu
	std::string m_nextState;
	sf::Texture *bg1;
	sf::Texture *headerOff;
	sf::Texture *headerOn;
	sf::Texture *playOff;
	sf::Texture *playOn;
	sf::Texture *optionsOff;
	sf::Texture *optionsOn;
	sf::Texture *creditsOff;
	sf::Texture *creditsOn;
	sf::Texture *quitOff;
	sf::Texture *quitOn;
	sf::Texture *arrowOff;
	sf::Texture *arrowOn;
	sf::Sprite *mp_bg1;
	sf::Sprite *mp_header;
	sf::Sprite *mp_play;
	sf::Sprite *mp_options;
	sf::Sprite *mp_credits;
	sf::Sprite *mp_quit;
	sf::Sprite *mp_arrow_1;
	sf::Sprite *mp_arrow_2;
	sf::Time turnOn;
	sf::Clock *turnOnTimer;
/*	sf::SoundBuffer buttonOn;
	sf::SoundBuffer buttonOff;
*/
	//SoundManager soundButtonOn;
	//int m_soundButtonOn;
	//int m_soundButtonOff;
	SoundEntity soundButtonOn;
	SoundEntity soundButtonOff;
	sf::Music menuMusic;
	sf::Music buzzing;

	//Options
	sf::Texture *soundOn;
	sf::Texture *soundOff;
	sf::Texture *musicOn;
	sf::Texture *musicOff;
	sf::Texture *gamma;
	sf::Texture *resolution;
	sf::Texture *defaultOptions;
	sf::Texture *sliderOn;
	sf::Texture *sliderOff;
	sf::Texture *pointerOn;
	sf::Texture *pointerOff;
	sf::Texture *checkboxOn;
	sf::Texture *checkboxOff;
	sf::Sprite *mp_sound;
	sf::Sprite *mp_sliderSound;
	sf::Sprite *mp_pointerSound;
	sf::Sprite *mp_checkboxSound;
	sf::Sprite *mp_music;
	sf::Sprite *mp_sliderMusic;
	sf::Sprite *mp_pointerMusic;
	sf::Sprite *mp_checkboxMusic;
	sf::Sprite *mp_gamma;
	sf::Sprite *mp_sliderGamma;
	sf::Sprite *mp_pointerGamma;
	sf::Sprite *mp_resoultion;
	sf::Sprite *mp_resoulutionUp;
	sf::Sprite *mp_resoulutionDown;
	sf::Sprite *mp_defaultOptions;
	//
	bool playSelected;
	bool optionsSelected;
	bool creditsSelected;

	bool soundActivation;
	bool musicActivation;

	bool optionsTab;

	bool m_done;
};