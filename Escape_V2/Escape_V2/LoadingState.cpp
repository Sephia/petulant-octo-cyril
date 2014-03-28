//LoadingState.h

#include "LoadingState.h"
#include "Settings.h"
#include "stdafx.h"

LoadingState::LoadingState() {
	srand(time(NULL));
	Settings::ms_window = nullptr;
	m_timer = 0.0f;

	mp_sprite1 = nullptr;
	mp_sprite2 = nullptr;
	mp_texture1 = nullptr;
	mp_texture2 = nullptr;
}


LoadingState::~LoadingState() {

}

void LoadingState::Enter() {
	sf::VideoMode video(1280, 720);
    std::vector<sf::VideoMode> supportedModes;
    supportedModes = video.getFullscreenModes();
    auto it = supportedModes.begin();
    while (it!=supportedModes.end())
    {
        if(it->width/16 == it->height/9)
        {
            video = *it;
            break;
        }
        it++;
    }

	sf::ContextSettings setting;
	setting.antialiasingLevel = 8;
	Settings::ms_window = new sf::RenderWindow(video, "With Intent", sf::Style::Fullscreen, setting);
	Settings::ms_window->setFramerateLimit(60);
	
	mp_texture1 = new sf::Texture();
	mp_texture2 = new sf::Texture();
	mp_texture1->loadFromFile("../data/sprites/BoringWarrior.png");
	mp_texture2->loadFromFile("../data/sprites/PizzaPower2.png");
	mp_sprite1 = new sf::Sprite(*mp_texture1);
	mp_sprite2 = new sf::Sprite(*mp_texture2);
}

void LoadingState::Exit() {
	if(mp_texture1 != nullptr) {
		delete mp_texture1;
		mp_texture1 = nullptr;
	}
	if(mp_texture2 != nullptr) {
		delete mp_texture2;
		mp_texture2 = nullptr;
	}
	if(mp_sprite1 != nullptr) {
		delete mp_sprite1;
		mp_sprite1 = nullptr;
	}
	if(mp_sprite2 != nullptr) {
		delete mp_sprite2;
		mp_sprite2 = nullptr;
	}
}

bool LoadingState::Update() {
	m_timer += Settings::ms_deltatime;

	if(m_timer > 5.0f) {
		m_nextState = "StartMenuState";
		return false;
	}
	

	return true;
}

void LoadingState::Draw() {
	if(m_timer > 2.5f) {
		Settings::ms_window->draw(*mp_sprite2);
	}
	else {
		Settings::ms_window->draw(*mp_sprite1);
	}
	Settings::ms_window->display();
	Settings::ms_window->clear();
	
}

std::string LoadingState::Next() {
	return m_nextState;
}

bool LoadingState::IsType(const std::string &c_type) {
	return c_type.compare("LoadingState") == 0;
}

void LoadingState::Reset() {
	m_nextState = "";
}
