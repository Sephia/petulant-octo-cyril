// VictoryState.h"

#include "VictoryState.h"
#include "Settings.h"

VictoryState::VictoryState() {
	m_nextState = "";
	m_done = false;

	mp_sprite = nullptr;
	mp_texture = nullptr;
}

VictoryState::~VictoryState() {

}

void VictoryState::Enter() {
	m_nextState = "";
	m_done = false;

	Settings::ms_window->setView(Settings::ms_window->getDefaultView());

	mp_texture = new sf::Texture();
	mp_texture->loadFromFile("../data/sprites/VictoryScreen.png");
	mp_sprite = new sf::Sprite(*mp_texture);

}

void VictoryState::Exit(){
	if(mp_texture != nullptr) {
		delete mp_texture;
		mp_texture = nullptr;
	}
	if(mp_sprite != nullptr) {
		delete mp_sprite;
		mp_sprite = nullptr;
	}
}

bool VictoryState::Update() {
	if(UpdateEvents()) {
		return false;
	}

	return true;
}

void VictoryState::Draw() {
	Settings::ms_window->clear();
	Settings::ms_window->draw(*mp_sprite);
	Settings::ms_window->display();
}

std::string VictoryState::Next() {
	return m_nextState;
}

bool VictoryState::IsType(const std::string &c_type){
	return c_type.compare("VictoryState") == 0;
}

void VictoryState::Reset() {

}

bool VictoryState::UpdateEvents() {
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
