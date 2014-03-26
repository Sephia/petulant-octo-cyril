// GuardShootingState.h

#pragma once

#include "GuardState.h"

class GuardShootingState :
	public GuardState
{
public:
	GuardShootingState();
	~GuardShootingState();

	void Enter();
	void Exit();
	void Init(int number, sf::Vector2f* p_position, float* p_rotation, AnimatedSprite* sprite, Grid2D* p_grid);
	bool Update(sf::Vector2f playerPosition, CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager);
	std::string Next();
	bool IsType(const std::string &c_type);
	void Cleanup();
	void UpdateAnimation(sf::Vector2f playerPosition);
	void AddWaypointToFront(sf::Vector2f waypoint);
	bool Detected(sf::Vector2f playerPosition, CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager);

private:
	bool Rotate(sf::Vector2f playerPosition);
	void Shoot(CollisionManager* p_collisionManager, sf::Vector2f playerPosition);

	bool m_soundShootingPlayed;
	sf::Sound* mp_shooting;
};

