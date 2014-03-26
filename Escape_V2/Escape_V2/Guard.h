// Guard.h

#pragma once

#include "stdafx.h"

class AnimatedSprite;
class GuardStateManager;
class CollisionManager;
class FurnitureManager;
class Grid2D;

class Guard {
public:
	Guard(int number, AnimatedSprite* p_sprite, Grid2D* mp_grid);
	~Guard();
	
	void UpdateAnimation(sf::Vector2f playerPosition);
	void Update(sf::Vector2f playerPosition, CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager);
	void Draw();
	sf::Vector2f GetPosition();
	void AddWaypointToFront(sf::Vector2f waypoint);
	AnimatedSprite* GetSprite();
	
	bool IsWalking();
	int GetFoot();

private:
	AnimatedSprite* mp_sprite;

	Grid2D* mp_grid;

	GuardStateManager* mp_guardStateManager;
	sf::Vector2f m_position;
	sf::Vector2f m_prevPosition;
	float m_speed;
	int m_number;
	float m_rotation;

	int m_foot;
};
