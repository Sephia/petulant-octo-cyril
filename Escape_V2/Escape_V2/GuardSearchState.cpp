// GuardSearchState.cpp

#include "GuardSearchState.h"
#include "AnimatedSprite.h"
#include "CollisionManager.h"
#include "FurnitureManager.h"
#include "PathFinding.h"
#include "Grid2D.h"
#include "stdafx.h"
#include "Settings.h"

GuardSearchState::GuardSearchState() {

}

GuardSearchState::~GuardSearchState() {

}

void GuardSearchState::Enter() {
	m_done = false;
	m_nextPosition = sf::Vector2f(0.0f, 0.0f);
	if(m_waypoints.size() > 0) {
		m_currentWaypoint = rand() % m_waypoints.size();
	}
	else {
		m_currentWaypoint = 0;
	}
	m_speed = 250;
}

void GuardSearchState::Exit() {
	m_waypoints.clear();
}

void GuardSearchState::Init(int number, sf::Vector2f* p_position, float* p_rotation, AnimatedSprite* sprite, Grid2D* p_grid) {
	m_number = number;

	m_waypoints.clear();
	m_waypoints = Settings::ms_roomWaypoints;

	m_done = false;
	m_nextState = "";

	mp_position = p_position;
	mp_rotation = p_rotation;

	mp_sprite = sprite;

	mp_grid = p_grid;
	mp_pathfinding = new PathFinding();
	mp_pathfinding->Init(mp_grid);
}

bool GuardSearchState::Update(sf::Vector2f playerPosition, CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager) {
	Movement();
	if(Detected(playerPosition, p_collisionManager, p_furnitureManager)) {
		m_nextState = "GuardShootingState";
		m_done = true;
	}

	return m_done;
}

std::string GuardSearchState::Next() {
	return m_nextState;
}

bool GuardSearchState::IsType(const std::string &c_type) {
	return c_type.compare("GuardSearchState") == 0;
}

void GuardSearchState::Cleanup() {

}

void GuardSearchState::UpdateAnimation(sf::Vector2f playerPosition) {
	mp_sprite->Update();
	mp_sprite->getSprite()->setRotation(*mp_rotation);
}

void GuardSearchState::AddWaypointToFront(sf::Vector2f waypoint) {

}

bool GuardSearchState::Detected(sf::Vector2f playerPosition, CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager) {
	sf::Vector2f vectorBetween(playerPosition - *mp_position);
	float distance = sqrtf(vectorBetween.x * vectorBetween.x + vectorBetween.y * vectorBetween.y);

	if(distance > 600) {
		return false;
	}

	float angleToPlayer = static_cast<int>(atan2f(vectorBetween.y, -vectorBetween.x) * 180 / static_cast<float>(M_PI) + 180.0f) % 360;
	float directionLooking = -(*mp_rotation) + 90;
	int diffAngle = static_cast<int>(angleToPlayer - directionLooking + 360) % 360;

	if(diffAngle < 330 && diffAngle > 30) {
		return false;
	}
	sf::Vector2f direction;
	if(distance != 0) {
		direction = vectorBetween / distance;
	}

	while(distance > 8) {
		if(p_collisionManager->Circle_WallCollision(playerPosition - vectorBetween, 15)) {
			return false;
		}
		sf::Sprite tempSprite = *mp_sprite->getSprite();
		tempSprite.setScale(0.1, 0.1);
		tempSprite.setPosition(playerPosition - vectorBetween);

		if(p_collisionManager->Circle_DoorCollision(tempSprite) != nullptr) {
			return false;
		}
		for(int i = 0; i < p_furnitureManager->GetCount(); i++) {	
			if(p_collisionManager->Circle_FurnitureCollision(tempSprite, *p_furnitureManager->GetFurniture(i))) {
				return false;
			}
		}

		vectorBetween.x -= direction.x * 10;
		vectorBetween.y -= direction.y * 10;

		distance = sqrtf(vectorBetween.x * vectorBetween.x + vectorBetween.y * vectorBetween.y);

	}

	return true;
}

void GuardSearchState::Movement() {
	if(m_waypoints.size() > 0) {
		if(!mp_pathfinding->m_foundGoal) {
			while(!mp_pathfinding->m_foundGoal) {
				if(!mp_pathfinding->FindPath(*mp_position, m_waypoints.at(m_currentWaypoint))) {
					break;
				}
			}
		}

		if(mp_pathfinding->m_foundGoal) {
			m_nextPosition = mp_pathfinding->NextPathPos(*mp_position, 5.0f);
		}

		if(Rotate()) {
			mp_sprite->ChangeAnimation("Guard1Walking.png");
			sf::Vector2f distance = *mp_position - m_nextPosition;
			float dist = sqrtf(distance.x * distance.x + distance.y * distance.y);
			if(dist > 1.0f) {
				distance.x /= dist;
				distance.y /= dist;

				*mp_position = *mp_position - distance * m_speed * Settings::ms_deltatime;
			}
			else {

				m_currentWaypoint = rand() % m_waypoints.size();
				mp_pathfinding->m_foundGoal = false;
				mp_pathfinding->m_initializedStartGoal = false;

				if(!mp_pathfinding->m_foundGoal) {
					while(!mp_pathfinding->m_foundGoal) {
						if(!mp_pathfinding->FindPath(*mp_position, m_waypoints.at(m_currentWaypoint))) {
							break;
						}
					}
				}
			}
		}
		else {
			mp_sprite->ChangeAnimation("Guard1Turning.png");
		}
	}
}

bool GuardSearchState::Rotate() {
	float rotationToGetTo = ( static_cast<int>(atan2(mp_position->y - m_nextPosition.y, mp_position->x - m_nextPosition.x) * 180 / static_cast<float>(M_PI) - 90) + 360 ) % 360;
	int diffDegrees = ( static_cast<int>(*mp_rotation - rotationToGetTo) + 720 ) % 360;

	if(diffDegrees < 5 || diffDegrees > 355) {
		*mp_rotation = rotationToGetTo;
		return true;
	}
	else if(diffDegrees >= 185) {
		*mp_rotation += 3.0f;
	}
	else if(diffDegrees < 185) {
		*mp_rotation -= 3.0f;
	}
	*mp_rotation = static_cast<int>(*mp_rotation) % 360;

	return false;
}