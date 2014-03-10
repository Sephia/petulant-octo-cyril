// GuardFootSteps.h

#pragma once

#include "stdafx.h"

class FootStepRipples;
class AnimatedSprite;

class GuardFootSteps
{
public:
	GuardFootSteps();
	~GuardFootSteps();

	void AddRipple(sf::Vector2f position, float rotation, AnimatedSprite* sprite);
	void Cleanup();
	void Update();
	void Draw();

private:
	std::vector<FootStepRipples*> m_ripples;

};

