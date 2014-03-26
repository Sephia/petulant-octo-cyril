// GuardState.h

#pragma once

#include "stdafx.h"

class Grid2D;
class AnimatedSprite;
class CollisionManager;
class PathFinding;
class FurnitureManager;

class GuardState {
public:
	virtual ~GuardState(){};

	virtual void Enter() = 0;
	virtual void Exit() = 0;
	virtual void Init(int number, sf::Vector2f* p_position, float* p_rotation, AnimatedSprite* sprite, Grid2D* p_grid) = 0;
	virtual bool Update(sf::Vector2f playerPosition, CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager) = 0;
	virtual std::string Next() = 0;
	virtual bool IsType(const std::string &c_type) = 0;
	virtual void Cleanup() = 0;
	virtual void UpdateAnimation(sf::Vector2f playerPosition) = 0;
	virtual void AddWaypointToFront(sf::Vector2f waypoint) = 0;
	virtual bool Detected(sf::Vector2f playerPosition, CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager) = 0;

protected:
	std::string m_nextState;
	bool m_done;
	std::vector<sf::Vector2f> m_waypoints;
	AnimatedSprite* mp_sprite;
	sf::Vector2f* mp_position;
	float* mp_rotation;
	int m_number;
	float m_timer;
	Grid2D* mp_grid;
	PathFinding* mp_pathfinding;
	float m_speed;
};
