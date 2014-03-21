// SoundRipple.h

#pragma once

#include "stdafx.h"

class AnimatedSprite;

class SoundRipple {
public:
	SoundRipple(sf::Vector2f position, int volume, bool isPlayer, AnimatedSprite* p_sprite);
	~SoundRipple();

	
	sf::Vector2f getPosition();
	int getVolume();
	bool Update();
	void Draw();
	bool getPlayerCreated();

private:
	sf::Vector2f m_position;
	int m_soundVolume;
	float m_time;
	AnimatedSprite* mp_sprite;
	std::vector<sf::Vector2f> m_scales;
	bool m_playerCreated;
};

