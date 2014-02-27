// Guard.h

#pragma once

#include "stdafx.h"

class AnimatedSprite;
class GuardStateManager;
class CollisionManager;

class Guard {
public:
	Guard(int number, AnimatedSprite* p_sprite);
	~Guard();
	
	void UpdateAnimation();
	void Update(sf::Vector2f playerPosition, CollisionManager* p_collisionManager);
	void Draw();
	sf::Vector2f GetPosition();
	void AddWaypointToFront(sf::Vector2f waypoint);
	AnimatedSprite* GetSprite();

private:
	AnimatedSprite* mp_sprite;

	GuardStateManager* mp_guardStateManager;
	sf::Vector2f m_position;
	float m_speed;
	int m_number;
};
