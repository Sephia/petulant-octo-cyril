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
	void Init(int number, sf::Vector2f* p_position, AnimatedSprite* sprite, Grid2D* p_grid);
	bool Update(sf::Vector2f player_position, CollisionManager* p_collisionManager);
	std::string Next();
	bool IsType(const std::string &c_type);
	void Cleanup();
	void UpdateAnimation(sf::Vector2f playerPosition);
	void AddWaypointToFront(sf::Vector2f waypoint);
	bool Detected(sf::Vector2f player_detected, CollisionManager* p_collisionManager);

private:
	void Movement();

private:
	bool m_justLostSight;

};

