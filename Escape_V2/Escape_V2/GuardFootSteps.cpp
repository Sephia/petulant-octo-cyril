// GuardFootSteps.cpp

#include "GuardFootSteps.h"
#include "Settings.h"
#include "FootStepRipples.h"

GuardFootSteps::GuardFootSteps() {
	
}

GuardFootSteps::~GuardFootSteps() {

}

void GuardFootSteps::AddRipple(sf::Vector2f position, float rotation, AnimatedSprite* sprite, sf::Sound* sound) {
	m_ripples.push_back(new FootStepRipples(position, rotation, sprite, sound));
}

void GuardFootSteps::Cleanup() {
	for( unsigned int i = 0; i < m_ripples.size(); i++) {
		if(m_ripples.at(i) != nullptr) {
			m_ripples.at(i)->Cleanup();
			delete m_ripples.at(i);
			m_ripples.erase(m_ripples.begin() + i);
			i--;
		}
	}
}

void GuardFootSteps::Update() {
	for( unsigned int i = 0; i < m_ripples.size(); i++) {
		if(m_ripples.at(i) != nullptr) {
			if(m_ripples.at(i)->Update()) {
				m_ripples.at(i)->Cleanup();
				delete m_ripples.at(i);
				m_ripples.erase(m_ripples.begin() + i);
				i--;
			}
		}
	}
}

void GuardFootSteps::Draw() {
	for(unsigned int i = 0; i < m_ripples.size(); i++) {
		m_ripples.at(i)->Draw();
	}
}