// FootStepRipples.h

#pragma once

#include "stdafx.h"

class AnimatedSprite;

class FootStepRipples {
public:
	FootStepRipples(sf::Vector2f position, float rotation, AnimatedSprite* sprite, sf::Sound* sound);
	~FootStepRipples();

	void Draw();
	bool Update();
	void Cleanup();

private:
	AnimatedSprite* mp_sprite;
	sf::Vector2f m_position;
	float m_timeAlive;
	sf::Sound* mp_sound;
};

