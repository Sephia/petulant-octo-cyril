// FootStepRipples.cpp

#include "FootStepRipples.h"
#include "AnimatedSprite.h"
#include "Settings.h"

FootStepRipples::FootStepRipples(sf::Vector2f position, float rotation, AnimatedSprite* sprite) {
	m_position = position;
	mp_sprite = sprite;
	mp_sprite->getSprite()->setPosition(m_position);
	mp_sprite->getSprite()->setOrigin(mp_sprite->getSprite()->getLocalBounds().width/2, mp_sprite->getSprite()->getLocalBounds().height/2);
	mp_sprite->getSprite()->setRotation(rotation);
	m_timeAlive = 0.0f;
}

FootStepRipples::~FootStepRipples() {

}

bool FootStepRipples::Update() {
	m_timeAlive += Settings::ms_deltatime;

	if(m_timeAlive > 3.0f) {
		return true;
	}

	if(m_timeAlive > 1.0f) {
		sf::Color color = mp_sprite->getSprite()->getColor();
		color.a *= 0.96;
		mp_sprite->getSprite()->setColor(color);
	}

	return false;
}

void FootStepRipples::Draw() {
	Settings::ms_window->draw(*mp_sprite->getSprite());
}

void FootStepRipples::Cleanup() {
	if(mp_sprite != nullptr) {
		delete mp_sprite;
		mp_sprite = nullptr;
	}
}