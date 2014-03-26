// GameOverScreen.cpp

#include "GameOverState.h"
#include "Settings.h"

GameOverState::GameOverState() {
}

GameOverState::~GameOverState() {
}

void GameOverState::Enter() {
	m_nextState = "";
	m_done = false;

	Settings::ms_window->setView(Settings::ms_window->getDefaultView());

	mp_texture = new sf::Texture();
	mp_texture->loadFromFile("../data/sprites/GameOverScreen.png");
	mp_sprite = new sf::Sprite(*mp_texture);

}

void GameOverState::Exit(){
	if(mp_texture != nullptr) {
		delete mp_texture;
		mp_texture = nullptr;
	}
	if(mp_sprite != nullptr) {
		delete mp_sprite;
		mp_sprite = nullptr;
	}
}

bool GameOverState::Update() {
	if(UpdateEvents()) {
		return false;
	}

	return true;
}

void GameOverState::Draw() {
	Settings::ms_window->clear();
	Settings::ms_window->draw(*mp_sprite);
	Settings::ms_window->display();
}

std::string GameOverState::Next() {
	return m_nextState;
}

bool GameOverState::IsType(const std::string &c_type){
	return c_type.compare("GameOverState") == 0;
}

void GameOverState::Reset() {

}

bool GameOverState::UpdateEvents() {
	sf::Event event;
	while (Settings::ms_window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			Settings::ms_window->close();
			m_done = true;
		}
		else if(event.type == sf::Event::KeyPressed) {
			Settings::ms_inputManager.m_keyboard_current[event.key.code] = true;
		}
		else if(event.type == sf::Event::KeyReleased) {
			Settings::ms_inputManager.m_keyboard_current[event.key.code] = false;
		}
	}
	
	if(Settings::ms_inputManager.IsDownOnceKeyboard(sf::Keyboard::Escape) || Settings::ms_inputManager.IsDownOnceKeyboard(sf::Keyboard::Space) || Settings::ms_inputManager.IsDownOnceKeyboard(sf::Keyboard::Return)) {
		m_done = true;
		m_nextState = "StartMenuState";
	}
	return m_done;
}
