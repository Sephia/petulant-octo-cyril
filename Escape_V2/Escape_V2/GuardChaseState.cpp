// GuardChaseState.cpp

#include "GuardChaseState.h"
#include "AnimatedSprite.h"
#include "CollisionManager.h"
#include "PathFinding.h"
#include "Grid2D.h"
#include "stdafx.h"
#include "Settings.h"


GuardChaseState::GuardChaseState() {
	m_done = false;
	m_justLostSight = true;
	m_timer = 0.0f;
	m_speed = 350.0f;
}


GuardChaseState::~GuardChaseState() {
}

void GuardChaseState::Enter() {
	m_done = false;
	m_justLostSight = true;
	m_timer = 0.0f;
	mp_sprite->ChangeAnimation("Guard1Running.png");
}

void GuardChaseState::Exit() {
	m_waypoints.clear();
}

void GuardChaseState::Init(int number, sf::Vector2f* p_position, float* p_rotation, AnimatedSprite* sprite, Grid2D* p_grid) {
	m_number = number;

	m_waypoints.clear();

	m_done = false;
	m_nextState = "";

	mp_sprite = sprite;

	mp_position = p_position;
	mp_rotation = p_rotation;

	mp_grid = p_grid;
	mp_pathfinding = new PathFinding();
	mp_pathfinding->Init(mp_grid);
}

bool GuardChaseState::Update(sf::Vector2f player_position, CollisionManager* p_collisionManager) {
	if(!Detected(player_position, p_collisionManager)) {
		if(m_justLostSight) {
			m_waypoints.clear();
			m_waypoints.push_back(player_position);
			mp_pathfinding->m_foundGoal = false;
			mp_pathfinding->m_initializedStartGoal = false;
			m_justLostSight = false;
		}
		Movement();
	}
	else {
		m_nextState = "GuardShootingState";
		m_done = true;
	}

	return m_done;
}

std::string GuardChaseState::Next() {
	return m_nextState;
}

bool GuardChaseState::IsType(const std::string &c_type) {
	return c_type.compare("GuardChaseState") == 0;
}

void GuardChaseState::Cleanup() {

}

void GuardChaseState::UpdateAnimation(sf::Vector2f playerPosition) {
	mp_sprite->Update();
	mp_sprite->getSprite()->setRotation(*mp_rotation);
}

void GuardChaseState::AddWaypointToFront(sf::Vector2f waypoint) {
	m_waypoints.clear();
	m_waypoints.push_back(waypoint);
}

bool GuardChaseState::Detected(sf::Vector2f playerPosition, CollisionManager* p_collisionManager) {
	sf::Vector2f vectorBetween(playerPosition - *mp_position);
	float distance = sqrtf(vectorBetween.x * vectorBetween.x + vectorBetween.y * vectorBetween.y);

	if(distance > 600) {
		return false;
	}

	float angleToPlayer = static_cast<int>(atan2f(vectorBetween.y, -vectorBetween.x) * 180 / static_cast<float>(M_PI) + 180.0f) % 360;
	float directionLooking = -(*mp_rotation) + 90;
	int diffAngle = static_cast<int>(angleToPlayer - directionLooking + 360) % 360;

	if(diffAngle < 300 && diffAngle > 60) {
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

		vectorBetween.x -= direction.x * 10;
		vectorBetween.y -= direction.y * 10;

		distance = sqrtf(vectorBetween.x * vectorBetween.x + vectorBetween.y * vectorBetween.y);

	}

	return true;
}

void GuardChaseState::Movement() {
	if(!mp_pathfinding->m_foundGoal) {
		while(!mp_pathfinding->m_foundGoal) {
			if(!mp_pathfinding->FindPath(sf::Vector2f((*mp_position).x, (*mp_position).y), sf::Vector2f(m_waypoints.at(0).x, m_waypoints.at(0).y))) {
				break;
			}
		}
	}

	if(mp_pathfinding->m_foundGoal) {
		m_waypoints.at(0) = mp_pathfinding->NextPathPos(sf::Vector2f((*mp_position).x, (*mp_position).y), 5.0f);
	}

	if(m_waypoints.size() > 0) {
		if(m_timer > 0.0f) {
			if(m_timer < 1.0f) {
				Rotate(-1);
			}
			else if(m_timer < 3.0f) {
				Rotate(1);
			}
			else {
				//ToDo: Change from Patrol to Search when Search is ready
				m_nextState = "GuardSearchState";
				m_done = true;
			}

			m_timer += Settings::ms_deltatime;
			mp_sprite->ChangeAnimation("Guard1Turning.png");
		}
		else if(Rotate(0)) {
			mp_sprite->ChangeAnimation("Guard1Walking.png");
			sf::Vector2f distance = *mp_position - m_waypoints.at(0);
			float dist = sqrtf(distance.x * distance.x + distance.y * distance.y);
			if(dist > 5.0f) {
				distance.x /= dist;
				distance.y /= dist;

				*mp_position = *mp_position - distance * m_speed * Settings::ms_deltatime;
				m_timer = 0.0f;
			}
			else {
				m_timer += Settings::ms_deltatime;
			}
		}
		else {
			mp_sprite->ChangeAnimation("Guard1Turning.png");
		}
	}
}

bool GuardChaseState::Rotate(int rotationWay) {
	float rotationToGetTo;
	int diffDegrees;
	float rotationSpeed = 4.0f;

	if(rotationWay < 0) {
		diffDegrees = 10;
	}
	else if(rotationWay > 0) {
		diffDegrees = 350;
	}
	else {
		rotationToGetTo = ( static_cast<int>(atan2(mp_position->y - m_waypoints.at(0).y, mp_position->x - m_waypoints.at(0).x) * 180 / static_cast<float>(M_PI) - 90) + 360 ) % 360;
		diffDegrees = ( static_cast<int>(*mp_rotation - rotationToGetTo) + 720 ) % 360;
	}

	if(diffDegrees < 5 || diffDegrees > 355) {
		*mp_rotation = static_cast<int>(*mp_rotation) % 360;
		return true;
	}
	else if(diffDegrees >= 185) {
		*mp_rotation += rotationSpeed;
	}
	else if(diffDegrees < 185) {
		*mp_rotation -= rotationSpeed;
	}

	*mp_rotation = static_cast<int>(*mp_rotation) % 360;

	return false;
}