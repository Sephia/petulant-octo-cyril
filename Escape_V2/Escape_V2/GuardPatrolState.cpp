// GuardPatrolState.cpp

#include "GuardPatrolState.h"
#include "Settings.h"
#include "AnimatedSprite.h"
#include "CollisionManager.h"
#include "stdafx.h"

GuardPatrolState::GuardPatrolState() {
	
}

GuardPatrolState::~GuardPatrolState() {
	
}

void GuardPatrolState::Enter() {
	std::cout << "Entering GuardPatrolState\n";
	m_done = false;
	
}

void GuardPatrolState::Exit() {

}

void GuardPatrolState::Init(int number, sf::Vector2f* p_position, AnimatedSprite* sprite) {
	m_number = number;

	m_waypoints.clear();
	
	m_done = false;
	m_nextState = "";

	mp_position = &Settings::ms_guards.at(m_number);

	for(int i = 0; i < Settings::m_allGuardWaypoints[m_number].size(); i++) {
	
		m_waypoints.push_back(Settings::m_allGuardWaypoints[m_number].at(i));

	}

	m_nextWaypoint = 0;

	mp_position = p_position;

	mp_sprite = sprite;
}

bool GuardPatrolState::Update() {
	Movement();

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

void GuardPatrolState::UpdateAnimation() {
	mp_sprite->Update();
	m_rotation = atan2(mp_position->y - m_waypoints.at(m_nextWaypoint).y, mp_position->x - m_waypoints.at(m_nextWaypoint).x) * 180 / M_PI - 90;
	mp_sprite->getSprite()->setRotation(m_rotation);
}

void GuardPatrolState::Movement() {
	if(m_waypoints.size() > 0) {
		sf::Vector2f distance = *mp_position - m_waypoints.at(m_nextWaypoint);
		float speed = 100;
		float dist = sqrtf(distance.x * distance.x + distance.y * distance.y);
		if(dist > 1) {
			distance.x /= dist;
			distance.y /= dist;

			*mp_position = *mp_position - distance * speed * Settings::ms_deltatime;
		}
		else {
			m_nextWaypoint++;
			m_nextWaypoint %= m_waypoints.size();
		}
	}
}

void GuardPatrolState::AddWaypointToFront(sf::Vector2f waypoint) {

	m_nextState = "GuardInvestigateState";
	m_done = true;

	/*auto it = m_waypoints.begin() + m_nextWaypoint;
	m_waypoints.insert(it, waypoint);*/
}

bool GuardPatrolState::Detected(sf::Vector2f playerPosition, CollisionManager* p_collisionManager) {
	sf::Vector2f direction(*mp_position - playerPosition);
	float distance = sqrtf(direction.x * direction.x + direction.y * direction.y);
	while(distance > 20) {
		if(p_collisionManager->Circle_WallCollision(*mp_position - direction, 10)) {
			return false;
		}
		if(direction.x > 11) {
			direction.x -= 5;
		}
		else if(direction.x < -11) {
			direction.x += 5;
		}

		if(direction.y > -11) {
			direction.y -= 5;
		}
		else if(direction.y < -11) {
			direction.y += 5;
		}
		distance = sqrtf(direction.x * direction.x + direction.y * direction.y);
	}
	return true;
}