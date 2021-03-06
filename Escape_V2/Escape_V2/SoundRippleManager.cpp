// SoundRippleManager.cpp

#include "SoundRippleManager.h"
#include "SoundRipple.h"
#include "stdafx.h"

SoundRippleManager::SoundRippleManager() {

}

SoundRippleManager::~SoundRippleManager() {

}

sf::Vector2f SoundRippleManager::GuardNotice(sf::Vector2f guardPosition) {
	sf::Vector2f vdistance;
	float fdistance;
	for(int i = m_soundOrigins.size() - 1; i >= 0; i--) {
		if(m_soundOrigins.at(i)->getPlayerCreated()) {
			vdistance = guardPosition - m_soundOrigins.at(i)->getPosition();
			fdistance = sqrtf(fabs(vdistance.x * vdistance.x + vdistance.y * vdistance.y));

			if(fdistance < m_soundOrigins.at(i)->getVolume() * 250) {
				return m_soundOrigins.at(i)->getPosition();
			}
		}
	}
	return sf::Vector2f(0, 0);
}

void SoundRippleManager::CreateSoundRipple(sf::Vector2f soundOriginPosition, int soundVolume, bool isPlayer, AnimatedSprite* p_sprite, sf::Sound* sound) {
	m_soundOrigins.push_back(new SoundRipple(soundOriginPosition, soundVolume, isPlayer, p_sprite, sound));
}

void SoundRippleManager::UpdateSounds() {
	for(unsigned int i = 0; i < m_soundOrigins.size(); i++) {
		if(m_soundOrigins.at(i)->Update()) {
			auto it = m_soundOrigins.begin() + i;
			delete *it;
			m_soundOrigins.erase(it);
			i--;
		}
	}
}

void SoundRippleManager::Draw() {
	for(unsigned int i = 0; i < m_soundOrigins.size(); i++) {
		m_soundOrigins.at(i)->Draw();
	}
}

void SoundRippleManager::Cleanup() {
	for(unsigned int i = 0; i < m_soundOrigins.size(); i++) {
		if(m_soundOrigins.at(i) != nullptr) {
			delete m_soundOrigins.at(i);
			m_soundOrigins.erase(m_soundOrigins.begin() + i);
			i--;
		}
	}
	m_soundOrigins.clear();
}