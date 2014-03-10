//GuardPatrolState

#pragma once

#include "GuardState.h"

class GuardPatrolState :
	public GuardState
{
public:
	GuardPatrolState();
	~GuardPatrolState();

	void Enter();
	void Exit();
	void Init(int number, sf::Vector2f* p_position, float* p_rotation, AnimatedSprite* sprite, Grid2D* p_grid);
	bool Update(sf::Vector2f player_position, CollisionManager* p_collisionManager);
	std::string Next();
	bool IsType(const std::string &c_type);
	void Cleanup();
	void UpdateAnimation(sf::Vector2f playerPosition);
	void AddWaypointToFront(sf::Vector2f waypoint);
	bool Detected(sf::Vector2f player_detected, CollisionManager* p_collisionManager);
	

private:
	void Movement();
	bool Rotate();

private:
	sf::Vector2f m_nextPosition;
	int m_nextWaypoint;
};

