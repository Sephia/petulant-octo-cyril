//LoadingState.h

#include "LoadingState.h"
#include "Settings.h"
#include "stdafx.h"

LoadingState::LoadingState() {
	srand(time(NULL));
	Settings::ms_window = nullptr;
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
	Settings::ms_window = new sf::RenderWindow(video, "SFML shapes", sf::Style::Default, setting);
	Settings::ms_window->setFramerateLimit(60);
	
}

void LoadingState::Exit() {

}

bool LoadingState::Update() {
	m_nextState = "StartMenuState";
	return false;
}

void LoadingState::Draw() {
//	ms_window->display();
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
