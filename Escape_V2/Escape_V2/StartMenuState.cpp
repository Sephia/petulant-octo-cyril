// StartMenuState.cpp

#include "StartMenuState.h"
#include "OptionsState.h"
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <memory>
#include <stdlib.h>
#include "Settings.h"
#include "stdafx.h"


StartMenuState::StartMenuState() {
	m_nextState = "";
	m_done = false;
	optionsTab = false;
	creditsTab = false;
	
	//Buttons selected
	playSelected = false;
	optionsSelected = false;
	creditsSelected = false;

	soundActivation = true;
	musicActivation = true;

	checkFade = false;
	fade = 255;
	//toggleMusic = true;
	
}

StartMenuState::~StartMenuState() {

}

void StartMenuState::Enter() {
	m_nextState = "";
	m_done = false;

	//Header Timer
	turnOnHeader = new sf::Clock;
//	turnOn = turnOnHeader->getElapsedTime();

	//Credits Timer
	turnOnCredits = new sf::Clock;
//	creditsRollOn = turnOnCredits->getElapsedTime();
	//Sprite List

	//Start Menu
	mp_bg1 = new sf::Sprite;
	mp_header = new sf::Sprite;
	mp_play = new sf::Sprite;
	mp_options = new sf::Sprite;
	mp_credits = new sf::Sprite;
	mp_quit = new sf::Sprite;
	mp_arrow_1 = new sf::Sprite;
	mp_arrow_2 = new sf::Sprite;
	//Options
	mp_sound = new sf::Sprite;
	mp_music = new sf::Sprite;
	mp_gamma = new sf::Sprite;
	mp_sliderSound = new sf::Sprite;
	mp_pointerSound = new sf::Sprite;
	mp_checkboxSound = new sf::Sprite;
	mp_sliderMusic = new sf::Sprite;
	mp_pointerMusic = new sf::Sprite;
	mp_checkboxMusic = new sf::Sprite;
	mp_sliderGamma = new sf::Sprite;
	mp_pointerGamma = new sf::Sprite;
	mp_resoultion = new sf::Sprite;
	mp_resoulutionUp = new sf::Sprite;
	mp_resoulutionDown = new sf::Sprite;
	mp_defaultOptions = new sf::Sprite;
	//Credits
	mp_creditsRoll = new sf::Sprite;

	//Background Image
	bg1 = new sf::Texture;
	if (!bg1->loadFromFile("../Data/Sprites/MAIN_MENU/Background_Noire_Updated.png"))
	{
		std::cout << "could not load bg1" << std::endl;
	}
	mp_bg1->setTexture(*bg1);
	mp_bg1->setTextureRect(sf::IntRect(0, 0, 1920, 1080));

	//Header (Title)
	headerOff = new sf::Texture;
	if (!headerOff->loadFromFile("../Data/Sprites/MAIN_MENU/Header_2.png"))
	{
		std::cout << "could not load headerOff" << std::endl;
	}
	headerOn = new sf::Texture;
	if (!headerOn->loadFromFile("../Data/Sprites/MAIN_MENU/Header_1.png"))
	{
		std::cout << "could not load headerOn" << std::endl;
	}

	mp_header->setTexture(*headerOff);
	//mp_header->setTextureRect(sf::IntRect(0, 0, 1120, 340));
	mp_header->setPosition(412, 48);

	//"Play" button
	playOff = new sf::Texture;
	if (!playOff->loadFromFile("../Data/Sprites/MAIN_MENU/Play_2.png"))
	{
		std::cout << "could not load playOff" << std::endl;
	}
	playOn = new sf::Texture;
	if (!playOn->loadFromFile("../Data/Sprites/MAIN_MENU/Play_1.png"))
	{
		std::cout << "could not load playOn" << std::endl;
	}
	//mp_play->setTextureRect(sf::IntRect(0, 0, 200, 100));
	mp_play->setPosition(86, 408);

	//"Options" button
	optionsOff = new sf::Texture;
	if (!optionsOff->loadFromFile("../Data/Sprites/MAIN_MENU/Options_2.png"))
	{
		std::cout << "could not load optionsOff" << std::endl;
	}
	optionsOn = new sf::Texture;
	if (!optionsOn->loadFromFile("../Data/Sprites/MAIN_MENU/Options_1.png"))
	{
		std::cout << "could not load optionsOn" << std::endl;
	}
	//mp_options->setTextureRect(sf::IntRect(0, 0, 310, 100));
	mp_options->setPosition(86, 482);

	//"Credits" button
	creditsOff = new sf::Texture;
	if (!creditsOff->loadFromFile("../Data/Sprites/MAIN_MENU/Credits_2.png"))
	{
		std::cout << "could not load creditsOff" << std::endl;
	}
	creditsOn = new sf::Texture;
	if (!creditsOn->loadFromFile("../Data/Sprites/MAIN_MENU/Credits_1.png"))
	{
		std::cout << "could not load creditsOn" << std::endl;
	}
	//mp_credits->setTextureRect(sf::IntRect(0, 0, 290, 100));
	mp_credits->setPosition(86, 556);

	//"Quit" button
	quitOff = new sf::Texture;
	if (!quitOff->loadFromFile("../Data/Sprites/MAIN_MENU/Quit_2.png"))
	{
		std::cout << "could not load quitOff" << std::endl;
	}
	quitOn = new sf::Texture;
	if (!quitOn->loadFromFile("../Data/Sprites/MAIN_MENU/Quit_1.png"))
	{
		std::cout << "could not load quitOn" << std::endl;
	}
	//mp_quit->setTextureRect(sf::IntRect(0, 0, 200, 100));
	mp_quit->setPosition(86, 630);

	//"Arrow" Selection
	arrowOff = new sf::Texture;
	if (!arrowOff->loadFromFile("../Data/Sprites/MAIN_MENU/Selected_2.png"))
	{
		std::cout << "could not load arrowOff" << std::endl;
	}
	mp_arrow_1->setTexture(*arrowOff);
	mp_arrow_1->setPosition(-100, -100);
	arrowOn = new sf::Texture;
	if (!arrowOn->loadFromFile("../Data/Sprites/MAIN_MENU/Selected_1.png"))
	{
		std::cout << "could not load arrowOn" << std::endl;
	}
	mp_arrow_2->setTexture(*arrowOn);
	mp_arrow_2->setPosition(-100, -100);

	//Sound
	soundOn = new sf::Texture;
	if (!soundOn->loadFromFile("../Data/Sprites/MAIN_MENU/Sound_1.png"))
	{
		std::cout << "could not load soundOn" << std::endl;
	}
	soundOff = new sf::Texture;
	if (!soundOff->loadFromFile("../Data/Sprites/MAIN_MENU/Sound_2.png"))
	{
		std::cout << "could not load soundOff" << std::endl;
	}
	mp_sound->setTexture(*soundOn);
	mp_sound->setPosition(525, 535);

	//Music
	musicOn = new sf::Texture;
	if (!musicOn->loadFromFile("../Data/Sprites/MAIN_MENU/Music_1.png"))
	{
		std::cout << "could not load musicOn" << std::endl;
	}
	musicOff = new sf::Texture;
	if (!musicOff->loadFromFile("../Data/Sprites/MAIN_MENU/Music_2.png"))
	{
		std::cout << "could not load musicOff" << std::endl;
	}
	mp_music->setTexture(*musicOn);
	mp_music->setPosition(525, 439);

	//Gamma
	gamma = new sf::Texture;
	if (!gamma->loadFromFile("../Data/Sprites/MAIN_MENU/Gamma_1.png"))
	{
		std::cout << "could not load gamma" << std::endl;
	}
	mp_gamma->setTexture(*gamma);
	mp_gamma->setPosition(525, 631);

	//Pointers, Sliders and Checkboxes
	sliderOn = new sf::Texture;
	if (!sliderOn->loadFromFile("../Data/Sprites/MAIN_MENU/Slider_bar.png"))
	{
		std::cout << "could not load sliderOn" << std::endl;
	}
	sliderOff = new sf::Texture;
	if (!sliderOff->loadFromFile("../Data/Sprites/MAIN_MENU/Slider_bar_Off.png"))
	{
		std::cout << "could not load sliderOff" << std::endl;
	}
	pointerOn = new sf::Texture;
	if (!pointerOn->loadFromFile("../Data/Sprites/MAIN_MENU/Slider_Pointer.png"))
	{
		std::cout << "could not load pointerOn" << std::endl;
	}
	pointerOff = new sf::Texture;
	if (!pointerOff->loadFromFile("../Data/Sprites/MAIN_MENU/Slider_Pointer_2.png"))
	{
		std::cout << "could not load pointerOff" << std::endl;
	}
	checkboxOn = new sf::Texture;
	if (!checkboxOn->loadFromFile("../Data/Sprites/MAIN_MENU/Checkbox_1.png"))
	{
		std::cout << "could not load checkboxOn" << std::endl;
	}
	checkboxOff = new sf::Texture;
	if (!checkboxOff->loadFromFile("../Data/Sprites/MAIN_MENU/Checkbox_2.png"))
	{
		std::cout << "could not load checkboxOff" << std::endl;
	}

	//Slider-, pointer- and checkbox textures and positions
	mp_sliderSound->setTexture(*sliderOn);
	mp_sliderSound->setPosition(745, 575);
	mp_sliderMusic->setTexture(*sliderOn);
	mp_sliderMusic->setPosition(745, 479);
	mp_sliderGamma->setTexture(*sliderOn);
	mp_sliderGamma->setPosition(745, 678);
	mp_pointerSound->setTexture(*pointerOn);
	mp_pointerSound->setPosition(1329, 547);
	mp_pointerMusic->setTexture(*pointerOn);
	mp_pointerMusic->setPosition(1329, 451);
	mp_pointerGamma->setTexture(*pointerOn);
	mp_pointerGamma->setPosition(1329, 643);
	mp_checkboxSound->setTexture(*checkboxOn);
	mp_checkboxSound->setPosition(1359, 498);
	mp_checkboxMusic->setTexture(*checkboxOn);
	mp_checkboxMusic->setPosition(1359, 402);

	//Credits
	creditsNumber = 0;
	fadeTimer = 8;
	sf::Texture* tempTex;
	tempTex = new sf::Texture;
	if (!tempTex->loadFromFile("../Data/Sprites/MAIN_MENU/Credits/1_Lead-Designer.png"))
	{
		std::cout << "could not load credits1" << std::endl;
	}
	Credits.push_back(tempTex);
	tempTex = new sf::Texture;
	if (!tempTex->loadFromFile("../Data/Sprites/MAIN_MENU/Credits/2_Producer.png"))
	{
		std::cout << "could not load credits2" << std::endl;
	}
	Credits.push_back(tempTex);
	tempTex = new sf::Texture;
	if (!tempTex->loadFromFile("../Data/Sprites/MAIN_MENU/Credits/3_Programming.png"))
	{
		std::cout << "could not load credits3" << std::endl;
	}
	Credits.push_back(tempTex);
	tempTex = new sf::Texture;
	if (!tempTex->loadFromFile("../Data/Sprites/MAIN_MENU/Credits/4_Graphics.png"))
	{
		std::cout << "could not load credits4" << std::endl;
	}
	Credits.push_back(tempTex);
	tempTex = new sf::Texture;
	if (!tempTex->loadFromFile("../Data/Sprites/MAIN_MENU/Credits/5_Sound-Effects.png"))
	{
		std::cout << "could not load credits5" << std::endl;
	}
	Credits.push_back(tempTex);
	tempTex = new sf::Texture;
	if (!tempTex->loadFromFile("../Data/Sprites/MAIN_MENU/Credits/5_5_Lead-Animator.png"))
	{
		std::cout << "could not load credits6" << std::endl;
	}
	Credits.push_back(tempTex);
	tempTex = new sf::Texture;
	if (!tempTex->loadFromFile("../Data/Sprites/MAIN_MENU/Credits/6_Level-Design.png"))
	{
		std::cout << "could not load credits7" << std::endl;
	}
	Credits.push_back(tempTex);
	tempTex = new sf::Texture;
	if (!tempTex->loadFromFile("../Data/Sprites/MAIN_MENU/Credits/7_Level-Software-Designer.png"))
	{
		std::cout << "could not load credits8" << std::endl;
	}
	Credits.push_back(tempTex);
	tempTex = new sf::Texture;
	if (!tempTex->loadFromFile("../Data/Sprites/MAIN_MENU/Credits/8_Level-Builder.png"))
	{
		std::cout << "could not load credits9" << std::endl;
	}
	Credits.push_back(tempTex);
	tempTex = new sf::Texture;
	if (!tempTex->loadFromFile("../Data/Sprites/MAIN_MENU/Credits/9_Let-there-be-light.png"))
	{
		std::cout << "could not load credits10" << std::endl;
	}
	Credits.push_back(tempTex);
	tempTex = new sf::Texture;
	if (!tempTex->loadFromFile("../Data/Sprites/MAIN_MENU/Credits/10_Music.png"))
	{
		std::cout << "could not load credits11" << std::endl;
	}
	Credits.push_back(tempTex);

	mp_creditsRoll->setTexture(*Credits.at(0));
	mp_creditsRoll->setPosition(420, 380);

	//Sound: Button On
	soundManager = new SoundManager;
    std::string temp;
	temp = soundManager->newSound("../Data/Sound/MAIN_MENU/KEY_PRESS_DOWN.wav", false, 500, 0.1f);
    soundButtonOn = soundManager->GetSound(temp)->CreateSound(sf::Vector2f(0,0));
	
	//Sound: Button Off
	soundButtonOff = soundManager->newSound("../Data/Sound/MAIN_MENU/KEY-PRESS-RELEASE.wav", false, 500, 0.1f);

	//Sound: Header Buzzing
	if (!buzzing.openFromFile("../Data/Sound/MAIN_MENU/BUZZ3_5.wav")){
		std::cout << "could not load buzzing" << std::endl;
	}
	
	//Menu Music
	menuMusic = soundManager->newSong("../Data/Music/Dances_and_Dames.wav", true);
	soundManager->GetSong(menuMusic)->Play();

	mp_view = new sf::View(sf::FloatRect(0, 0, static_cast<float>(Settings::ms_window->getSize().x), static_cast<float>(Settings::ms_window->getSize().y)));

	Settings::ms_window->setView(*mp_view);
}

void StartMenuState::Exit() {
	//har du något new vid enter måste du ha delete i exit
	buzzing.stop();
	soundManager->GetSong(menuMusic)->Stop();

	if(mp_view != nullptr) {
		delete mp_view;
		mp_view = nullptr;
	}

	optionsTab = false;
	creditsTab = false;
	
	//Buttons selected
	playSelected = false;
	optionsSelected = false;
	creditsSelected = false;

	soundActivation = true;
	musicActivation = true;

	checkFade = false;
	fade = 255;
}

bool StartMenuState::Update() {
	UpdateEvents();

//	Draw();


	if (creditsTab){
		//float opacity = 255;
		//mp_creditsRoll->setColor(sf::Color(255, 255, 255, opacity));
		float creditsChangeSprite = turnOnCredits->getElapsedTime().asSeconds();
		sf::Color opacity = mp_creditsRoll->getColor();
		if (creditsChangeSprite > 5.0f) {
			if (checkFade == false){
				fade -= 0.30f;
			}
			else {
				fade += 0.30f;
			}
			opacity.a = static_cast<sf::Uint8>(fade);
				mp_creditsRoll->setColor(opacity);
				if (fade <= 1){
					checkFade = true;
				}
				if (fade >= 253){
				checkFade = false;
			}
		}
		if (creditsChangeSprite > fadeTimer){
			creditsNumber++;
			fadeTimer += 5.98f;
			if (creditsNumber == Credits.size()){
				creditsNumber = 0;
			}
			mp_creditsRoll->setTexture(*Credits.at(creditsNumber));			
			
		}

	}
	else {
		creditsNumber = 0;
		mp_creditsRoll->setColor(sf::Color(255, 255, 255, 255));
		mp_creditsRoll->setTexture(*Credits.at(creditsNumber));
		turnOnCredits->restart();
	}

	return !m_done;

}

void StartMenuState::Draw() {

	Settings::ms_window->clear(sf::Color(255, 255, 255, 255));
	Settings::ms_window->draw(*mp_bg1);
	Settings::ms_window->draw(*mp_header);
	Settings::ms_window->draw(*mp_play);
	Settings::ms_window->draw(*mp_options);
	Settings::ms_window->draw(*mp_credits);
	Settings::ms_window->draw(*mp_quit);
	Settings::ms_window->draw(*mp_arrow_1);
	Settings::ms_window->draw(*mp_arrow_2);
	//Options
	if (optionsTab){
		Settings::ms_window->draw(*mp_sound);
		Settings::ms_window->draw(*mp_sliderSound);
		Settings::ms_window->draw(*mp_pointerSound);
		Settings::ms_window->draw(*mp_checkboxSound);
		Settings::ms_window->draw(*mp_music);
		Settings::ms_window->draw(*mp_sliderMusic);
		Settings::ms_window->draw(*mp_pointerMusic);
		Settings::ms_window->draw(*mp_checkboxMusic);
		Settings::ms_window->draw(*mp_gamma);
		Settings::ms_window->draw(*mp_sliderGamma);
		Settings::ms_window->draw(*mp_pointerGamma);
	}
	if (creditsTab){
		Settings::ms_window->draw(*mp_creditsRoll);
	}

	Settings::ms_window->display();
}

std::string StartMenuState::Next() {
	return m_nextState;
}

bool StartMenuState::IsType(const std::string &c_type) {
	return c_type.compare("StartMenuState") == 0;
}

void StartMenuState::Reset() {
	m_nextState = "";
}

void StartMenuState::UpdateEvents() {
	sf::Event event;
	while (Settings::ms_window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			Settings::ms_window->close();
			m_done = true;
		}
	}

	//Header blinking
	float restartHeader = turnOnHeader->getElapsedTime().asSeconds();
	int headerChance1 = 0;
	int headerChance2 = 0;
	if (restartHeader > 0.5f) {
		headerChance1 = rand() % 4 + 1;
		if (headerChance1 == 2) {

			mp_header->setTexture(*headerOn);
			if (buzzing.Paused){
				buzzing.play();
			}
			headerChance2 = rand() % 4 + 1;
			if (headerChance2 == 2){
				if (restartHeader > 0.6f) {
					if (buzzing.Playing){
						buzzing.pause();
					}

					mp_header->setTexture(*headerOff);
				}
			}
		}
		else {
			if (buzzing.Playing){
				buzzing.pause();
			}
			mp_header->setTexture(*headerOff);
			if (restartHeader > 0.6f) {
				if (buzzing.Paused){
					buzzing.play();
				}
				mp_header->setTexture(*headerOn);
			}
		}
		turnOnHeader->restart();
	}

	sf::Vector2i mPos = sf::Mouse::getPosition(*Settings::ms_window);
	//Temp: Window Position
	sf::Vector2i winPos = Settings::ms_window->getPosition();


	//Mouse within "Play"
	if (playSelected == false) {
		if (mPos.x > 120 && mPos.x < 251 && mPos.y > 428 && mPos.y < 487)  {

			mp_play->setTexture(*playOn);
			mp_arrow_1->setPosition(50, 408);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				playSelected = true;
				optionsTab = false;
				creditsTab = false;
				m_nextState = "GameState";
				m_done = true;
				if (creditsSelected) {
					creditsSelected = false;
				}
				if (optionsSelected) {
					optionsSelected = false;
				}
				if(!SoundEntity::IsMuted())
                {
                    soundButtonOn->play();
                }
			}

		}

		else {

			mp_play->setTexture(*playOff);
		}
	}
	if (playSelected == true) {
		mp_arrow_2->setPosition(50, 408);
		mp_play->setTexture(*playOn);
	}

	//Mouse within "Options"
	if (optionsSelected == false) {
		if (mPos.x > 121 && mPos.x < 361 && mPos.y > 501 && mPos.y < 562)  {

			mp_options->setTexture(*optionsOn);
			mp_arrow_1->setPosition(50, 482);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){

				optionsTab = true;
				creditsTab = false;
				optionsSelected = true;
				if (playSelected) {
					playSelected = false;
				}
				if (creditsSelected) {
					creditsSelected = false;
				}
                if(!SoundEntity::IsMuted())
                {
                    soundButtonOn->play();
                }
			}
		}

		else {

			mp_options->setTexture(*optionsOff);
		}
	}
	if (optionsSelected == true) {
		mp_arrow_2->setPosition(50, 482);
		mp_options->setTexture(*optionsOn);
	}

	//Mouse within "Credits"
	if (creditsSelected == false){
		if (mPos.x > 161 && mPos.x < 345 && mPos.y > 575 && mPos.y < 636)  {

			mp_credits->setTexture(*creditsOn);
			mp_arrow_1->setPosition(50, 556);

			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				creditsSelected = true;
				optionsTab = false;
				creditsTab = true;
				if (playSelected) {
					playSelected = false;
				}
				if (optionsSelected) {
					optionsSelected = false;
				}
				//soundButtonOn.Sounds[m_soundButtonOn].Play(0);
				if(!SoundEntity::IsMuted())
                {
                    soundButtonOn->play();
                }
			}
		}

		else {

			mp_credits->setTexture(*creditsOff);
		}
	}
	if (creditsSelected == true) {
		mp_arrow_2->setPosition(50, 556);
		mp_credits->setTexture(*creditsOn);
	}

	//Mouse within "Quit"
	if (mPos.x > 122 && mPos.x < 250 && mPos.y > 649 && mPos.y < 711)  {

		mp_quit->setTexture(*quitOn);
		mp_arrow_1->setPosition(50, 630);

		if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
			soundManager->GetSong(menuMusic)->Stop();
			m_done = true;
		}
	}

	else {

		mp_quit->setTexture(*quitOff);
	}
	if (optionsTab){
		
		//Music Activation
		if (mPos.x > mp_checkboxMusic->getPosition().x && mPos.x < mp_checkboxMusic->getLocalBounds().width + mp_checkboxMusic->getPosition().x 
			&& mPos.y > mp_checkboxMusic->getPosition().y && mPos.y < mp_checkboxMusic->getLocalBounds().height + mp_checkboxMusic->getPosition().y){
			if (event.type == sf::Event::MouseButtonPressed){
				if (event.mouseButton.button == sf::Mouse::Left){
					if (musicActivation == true){
						musicActivation = false;
					}
					else {
						musicActivation = true;
					}
                    soundManager->ToggleMusic();
					if(!SoundEntity::IsMuted())
                    {
                        soundButtonOn->play();
                    }
				}
			}

		}

		if (musicActivation == true){
		//	toggleMusic = true;
			mp_checkboxMusic->setTexture(*checkboxOn);
			mp_music->setTexture(*musicOn);
			mp_sliderMusic->setTexture(*sliderOn);
			mp_pointerMusic->setTexture(*pointerOn);
		}

		if (musicActivation == false){
		//	toggleMusic = false;
			soundManager->ToggleMusic();
			mp_checkboxMusic->setTexture(*checkboxOff);
			mp_music->setTexture(*musicOff);
			mp_sliderMusic->setTexture(*sliderOff);
			mp_pointerMusic->setTexture(*pointerOff);
		}

		// Sound Activation
		if (mPos.x > mp_checkboxSound->getPosition().x && mPos.x < mp_checkboxSound->getLocalBounds().width + mp_checkboxSound->getPosition().x 
			&& mPos.y > mp_checkboxSound->getPosition().y && mPos.y < mp_checkboxSound->getLocalBounds().height + mp_checkboxSound->getPosition().y)  {
			if (event.type == sf::Event::MouseButtonPressed){
				if (event.mouseButton.button == sf::Mouse::Left){
					
					if (soundActivation == true){
						soundActivation = false;
					}
					else {
						soundActivation = true;
					}
					if(!SoundEntity::IsMuted())
                    {
                        soundButtonOn->play();
                    }
				}
			}

		}

		if (soundActivation == true){
			soundManager->ToggleSound();
			buzzing.setVolume((mp_pointerSound->getPosition().x - mp_sliderSound->getPosition().x) / ((mp_sliderSound->getLocalBounds().width - mp_pointerSound->getLocalBounds().width) / 100));
			mp_checkboxSound->setTexture(*checkboxOn);
			mp_sound->setTexture(*soundOn);
			mp_sliderSound->setTexture(*sliderOn);
			mp_pointerSound->setTexture(*pointerOn);
		}

		if (soundActivation == false){
			soundManager->ToggleSound();
			buzzing.setVolume(0);
			mp_checkboxSound->setTexture(*checkboxOff);
			mp_sound->setTexture(*soundOff);
			mp_sliderSound->setTexture(*sliderOff);
			mp_pointerSound->setTexture(*pointerOff);
		}

		// Music Slider

		if (mPos.x > mp_sliderMusic->getPosition().x && mPos.x < mp_sliderMusic->getLocalBounds().width + mp_sliderMusic->getPosition().x
			&& mPos.y > mp_pointerMusic->getPosition().y && mPos.y < mp_pointerMusic->getLocalBounds().height + mp_pointerMusic->getPosition().y){
				if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
					mp_pointerMusic->setPosition(mPos.x - mp_pointerMusic->getLocalBounds().width / 2, mp_pointerMusic->getPosition().y);
					if (mp_pointerMusic->getPosition().x < mp_sliderMusic->getPosition().x) {
						mp_pointerMusic->setPosition(mp_sliderMusic->getPosition().x, mp_pointerMusic->getPosition().y);
					}
					if (mp_pointerMusic->getLocalBounds().width + mp_pointerMusic->getPosition().x > mp_sliderMusic->getLocalBounds().width + mp_sliderMusic->getPosition().x){
						mp_pointerMusic->setPosition(mp_sliderMusic->getPosition().x + mp_sliderMusic->getLocalBounds().width - mp_pointerMusic->getLocalBounds().width, mp_pointerMusic->getPosition().y);
					}
					for (unsigned int i = 0; i < soundManager->GetSongCount(); i++){
						soundManager->SetSongVolume((mp_pointerMusic->getPosition().x - mp_sliderMusic->getPosition().x) / ((mp_sliderMusic->getLocalBounds().width - mp_pointerMusic->getLocalBounds().width) / 100));
					}
				}
		}
		
		//Sound Slider

		if (mPos.x > mp_sliderSound->getPosition().x && mPos.x < mp_sliderSound->getLocalBounds().width + mp_sliderSound->getPosition().x
			&& mPos.y > mp_pointerSound->getPosition().y && mPos.y < mp_pointerSound->getLocalBounds().height + mp_pointerSound->getPosition().y){
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				mp_pointerSound->setPosition(mPos.x - mp_pointerSound->getLocalBounds().width / 2, mp_pointerSound->getPosition().y);
				if (mp_pointerSound->getPosition().x < mp_sliderSound->getPosition().x) {
					mp_pointerSound->setPosition(mp_sliderSound->getPosition().x, mp_pointerSound->getPosition().y);
				}
				if (mp_pointerSound->getLocalBounds().width + mp_pointerSound->getPosition().x > mp_sliderSound->getLocalBounds().width + mp_sliderSound->getPosition().x){
					mp_pointerSound->setPosition(mp_sliderSound->getPosition().x + mp_sliderSound->getLocalBounds().width - mp_pointerSound->getLocalBounds().width, mp_pointerSound->getPosition().y);
				}
				for (unsigned int i = 0; i < soundManager->GetSoundCount(); i++){
					soundManager->SetSoundVolume((mp_pointerSound->getPosition().x - mp_sliderSound->getPosition().x) / ((mp_sliderSound->getLocalBounds().width - mp_pointerSound->getLocalBounds().width) / 100));
				}				
			}
		}

		//Gamma Slider

		if (mPos.x > mp_sliderGamma->getPosition().x && mPos.x < mp_sliderGamma->getLocalBounds().width + mp_sliderGamma->getPosition().x
			&& mPos.y > mp_pointerGamma->getPosition().y && mPos.y < mp_pointerGamma->getLocalBounds().height + mp_pointerGamma->getPosition().y){
			if (sf::Mouse::isButtonPressed(sf::Mouse::Left)){
				mp_pointerGamma->setPosition(mPos.x - mp_pointerGamma->getLocalBounds().width / 2, mp_pointerGamma->getPosition().y);
				if (mp_pointerGamma->getPosition().x < mp_sliderGamma->getPosition().x) {
					mp_pointerGamma->setPosition(mp_sliderGamma->getPosition().x, mp_pointerGamma->getPosition().y);
				}
				if (mp_pointerGamma->getLocalBounds().width + mp_pointerGamma->getPosition().x > mp_sliderGamma->getLocalBounds().width + mp_sliderGamma->getPosition().x){
					mp_pointerGamma->setPosition(mp_sliderGamma->getPosition().x + mp_sliderGamma->getLocalBounds().width - mp_pointerGamma->getLocalBounds().width, mp_pointerGamma->getPosition().y);
				}
			}
		}


	}
	
	
}