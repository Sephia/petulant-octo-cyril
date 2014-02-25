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
	void Init(int number, sf::Vector2f* p_position, AnimatedSprite* sprite);
	bool Update();
	std::string Next();
	bool IsType(const std::string &c_type);
	void Cleanup();
	void UpdateAnimation();
	void AddWaypointToFront(sf::Vector2f waypoint);
	bool Detected(sf::Vector2f playerDetected, CollisionManager* p_collisionManager);
};

