// GuardStateManager.h

#pragma once

#include "stdafx.h"

class GuardState;
class AnimatedSprite;
class CollisionManager;
class Grid2D;

class GuardStateManager
{
public:
	GuardStateManager();
	~GuardStateManager();

	void Attach(GuardState *p_state);
	bool Update();
	void SetState(const std::string &c_type);
	void ChangeState();
	void Init(int number, sf::Vector2f* p_position, AnimatedSprite* sprite, Grid2D* p_grid);
	void Cleanup();
	void UpdateAnimation();
	void AddWaypointToFront(sf::Vector2f waypoint);
	bool Detected(sf::Vector2f player_position, CollisionManager* p_collisionManager);
	bool IsCurrent(std::string& type);

private:
	std::vector<GuardState*> m_states;
	GuardState* mp_currentState;
};

