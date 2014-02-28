// SoundRippleManager.h

#pragma once

#include "stdafx.h"

class SoundRipple;
class AnimatedSprite;

class SoundRippleManager {
public:
	SoundRippleManager();
	~SoundRippleManager();

	sf::Vector2f GuardNotice(sf::Vector2f guardPosition);
	void CreateSoundRipple(sf::Vector2f soundOriginPosition, int soundVolume, bool isPLayer, AnimatedSprite* p_sprite);
	void UpdateSounds();
	void Draw();

private:
	
	std::vector<SoundRipple*> m_soundOrigins;
};