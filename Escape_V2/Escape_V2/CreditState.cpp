// CreditState.cpp

#include "CreditState.h"
#include "Settings.h"
#include "Background.h"
#include "stdafx.h"

CreditState::CreditState() {
	m_nextState = "";
	mp_view = nullptr;
	mp_background = nullptr;
}

CreditState::~CreditState() {

}

void CreditState::Enter() {
	m_nextState = "";
	mp_view = new sf::View(sf::FloatRect(0, 0, static_cast<float>(Settings::ms_window->getSize().x), static_cast<float>(Settings::ms_window->getSize().y)));

	Settings::ms_window->setView(*mp_view);
	
	m_spriteManager.Init("../data/");
	mp_background = new Background(m_spriteManager.Load("Credits.txt"));
}

void CreditState::Exit() {
	if(mp_view != nullptr) {
		delete mp_view;
		mp_view = nullptr;
	}
}

bool CreditState::Update() {
	if(UpdateEvents()) {
		return false;
	}

	return true;
}

void CreditState::Draw() {
	Settings::ms_window->clear(sf::Color(0, 0, 0, 0));
	
	mp_background->Draw();

	

	Settings::ms_window->display();
}

std::string CreditState::Next() {
	return m_nextState;
}

bool CreditState::IsType(const std::string &c_type) {
	return c_type.compare("CreditState") == 0;
}

void CreditState::Reset() {
	m_nextState = "";
}

bool CreditState::UpdateEvents() {
	bool quit = false;
	sf::Event event;
	while (Settings::ms_window->pollEvent(event)) {
		if (event.type == sf::Event::Closed) {
			Settings::ms_window->close();
			quit = true;
		}
	}
	
	if(sf::Keyboard::isKeyPressed(sf::Keyboard::Escape)) {
		Settings::ms_window->close();
		quit = true;
	}
	return quit;
}