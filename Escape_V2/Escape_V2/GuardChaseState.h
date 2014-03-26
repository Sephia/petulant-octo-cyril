// GuardChaseState.h

#pragma once

#include "guardstate.h"

class GuardChaseState :
	public GuardState
{
public:
	GuardChaseState();
	~GuardChaseState();

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
	void Movement();
	bool Rotate(int rotationWay);

private:
	bool m_justLostSight;

};

