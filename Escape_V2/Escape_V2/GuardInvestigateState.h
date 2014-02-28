// GuardInvestigateState.h

#pragma once

#include "GuardState.h"

class GuardInvestigateState :
	public GuardState
{
public:
	GuardInvestigateState();
	~GuardInvestigateState();
	
	void Enter();
	void Exit();
	void Init(int number, sf::Vector2f* p_position, AnimatedSprite* sprite, Grid2D* p_grid);
	bool Update();
	std::string Next();
	bool IsType(const std::string &c_type);
	void Cleanup();
	void UpdateAnimation();
	void AddWaypointToFront(sf::Vector2f waypoint);
	bool Detected(sf::Vector2f player_detected, CollisionManager* p_collisionManager);

private:
	void Movement();
};