// SoundRipple.cpp

#include "SoundRipple.h"
#include "Settings.h"
#include "AnimatedSprite.h"
#include "stdafx.h"

SoundRipple::SoundRipple(sf::Vector2f position, int volume, bool isPlayer, AnimatedSprite* sprite) {
	m_position = position;
	m_soundVolume = volume;

	m_time = 0;

	m_playerCreated = isPlayer;

	mp_sprite = sprite;
	mp_sprite->Init("SoundRipple.png");
	mp_sprite->getSprite()->setPosition(position);
	mp_sprite->getSprite()->setOrigin(mp_sprite->getSprite()->getLocalBounds().width / 2, mp_sprite->getSprite()->getLocalBounds().height / 2);
	float factor = 1.2f;
	sf::Vector2f vect(0.05f, 0.05f);
	for(int i = 0; i < 1; i++) {
		m_scales.push_back(vect);
		vect *= factor;
	}
	
}


SoundRipple::~SoundRipple() {
	if(mp_sprite != nullptr) {
		delete mp_sprite;
		mp_sprite = nullptr;
	}
}

sf::Vector2f SoundRipple::getPosition() {
	return m_position;
}

int SoundRipple::getVolume() {
	return m_soundVolume;
}

bool SoundRipple::Update() {
	m_time += Settings::ms_deltatime;
	if(m_time > 2.0f) {
		return true;
	}
	return false;
}

void SoundRipple::Draw() {
	for(unsigned int i = 0; i < m_scales.size(); i++) {
		mp_sprite->getSprite()->setScale(m_scales.at(i));
		m_scales.at(i).x *= 1.015f;
		m_scales.at(i).y *= 1.015f;
		sf::Color color = mp_sprite->getSprite()->getColor();
		color.a *= 0.992;
		mp_sprite->getSprite()->setColor(color);
		Settings::ms_window->draw(*mp_sprite->getSprite());
	}
}

bool SoundRipple::getPlayerCreated() {
	return m_playerCreated;
}