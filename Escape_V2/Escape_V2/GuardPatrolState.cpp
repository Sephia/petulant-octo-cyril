// GuardPatrolState.cpp

#include "GuardPatrolState.h"
#include "Settings.h"
#include "AnimatedSprite.h"
#include "CollisionManager.h"
#include "FurnitureManager.h"
#include "stdafx.h"
#include "PathFinding.h"


GuardPatrolState::GuardPatrolState() {
	m_speed = 200.0f;
}

GuardPatrolState::~GuardPatrolState() {

}

void GuardPatrolState::Enter() {
	m_done = false;
}

void GuardPatrolState::Exit() {
	mp_pathfinding->m_foundGoal = false;
	mp_pathfinding->m_initializedStartGoal = false;
}

void GuardPatrolState::Init(int number, sf::Vector2f* p_position, float* p_rotation, AnimatedSprite* sprite, Grid2D* p_grid) {
	m_number = number;

	m_waypoints.clear();

	m_done = false;
	m_nextState = "";

	mp_position = &Settings::ms_guards.at(m_number);
	mp_position = p_position;
	mp_rotation = p_rotation;

	m_nextWaypoint = 0;
	
	for(unsigned int i = 0; i < Settings::m_allGuardWaypoints[m_number].size(); i++) {
		m_waypoints.push_back(Settings::m_allGuardWaypoints[m_number].at(i));
	}
	if(m_waypoints.size() > 1) {
		*mp_rotation = atan2(mp_position->y - m_waypoints.at(m_nextWaypoint).y, mp_position->x - m_waypoints.at(m_nextWaypoint).x) * 180 / static_cast<float>(M_PI) - 90;
	}
	

	mp_sprite = sprite;

	mp_grid = p_grid;
	mp_pathfinding = new PathFinding();
	mp_pathfinding->Init(mp_grid);
}

bool GuardPatrolState::Update(sf::Vector2f playerPosition, CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager) {
	Movement();
	if(Detected(playerPosition, p_collisionManager, p_furnitureManager)) {
		m_nextState = "GuardShootingState";
		m_done = true;
	}

	return m_done;
}

std::string GuardPatrolState::Next() {
	return m_nextState;
}

bool GuardPatrolState::IsType(const std::string &c_type) {
	return c_type.compare("GuardPatrolState") == 0;
}

void GuardPatrolState::Cleanup() {

}

void GuardPatrolState::UpdateAnimation(sf::Vector2f playerPosition) {
	mp_sprite->Update();
	mp_sprite->getSprite()->setRotation(*mp_rotation);
}

void GuardPatrolState::Movement() {
	if(m_waypoints.size() > 0) {
		if(!mp_pathfinding->m_foundGoal) {
			while(!mp_pathfinding->m_foundGoal) {
				if(!mp_pathfinding->FindPath(*mp_position, m_waypoints.at(m_nextWaypoint))) {
					break;
				}
			}
		}

		if(mp_pathfinding->m_foundGoal) {
			m_nextPosition = mp_pathfinding->NextPathPos(*mp_position, 12.0f);
		}

		if(Rotate()) {
			mp_sprite->ChangeAnimation("Guard1Walking.png");
			sf::Vector2f distance = *mp_position - m_nextPosition;
			float dist = sqrtf(distance.x * distance.x + distance.y * distance.y);
			if(dist > 0.0f) {
				distance.x /= dist;
				distance.y /= dist;

				*mp_position = *mp_position - distance * m_speed * Settings::ms_deltatime;
			}

			distance = *mp_position - m_waypoints.at(m_nextWaypoint);
			dist = sqrtf(distance.x * distance.x + distance.y * distance.y);
			if(dist < 50.0f) {
				m_nextWaypoint++;
				m_nextWaypoint %= m_waypoints.size();
				mp_pathfinding->m_foundGoal = false;
				mp_pathfinding->m_initializedStartGoal = false;
			}
		}
		else {
			mp_sprite->ChangeAnimation("Guard1Turning.png");
		}
	}
}

void GuardPatrolState::AddWaypointToFront(sf::Vector2f waypoint) {

	m_nextState = "GuardInvestigateState";
	m_done = true;
}

bool GuardPatrolState::Detected(sf::Vector2f playerPosition, CollisionManager* p_collisionManager, FurnitureManager* p_furnitureManager) {
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
	if(distance != 0.0f) {
		direction = vectorBetween / distance;
	}

	while(distance > 8.0f) {
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

bool GuardPatrolState::Rotate() {
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