// GuardState.h

#pragma once

#include "stdafx.h"

class Grid2D;
class AnimatedSprite;
class CollisionManager;
class PathFinding;

class GuardState {
public:
	virtual ~GuardState(){};

	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Init(int number, sf::Vector2f* p_position, AnimatedSprite* sprite, Grid2D* p_grid) = 0;
	virtual bool Update() = 0;
	virtual std::string Next() = 0;
	virtual bool IsType(const std::string &c_type) = 0;
	virtual void Cleanup() = 0;
	virtual void UpdateAnimation() = 0;
	virtual void AddWaypointToFront(sf::Vector2f waypoint) = 0;
	virtual bool Detected(sf::Vector2f player_position, CollisionManager* p_collisionManager) = 0;

protected:
	std::string m_nextState;
	bool m_done;
	std::vector<sf::Vector2f> m_waypoints;
	AnimatedSprite* mp_sprite;
	sf::Vector2f* mp_position;
	float m_rotation;
	int m_number;
	float m_timer;
	Grid2D* mp_grid;
	PathFinding* mp_pathfinding;
};
