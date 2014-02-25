// GuardInvestigateState.cpp

#include "GuardInvestigateState.h"
#include "AnimatedSprite.h"
#include "Settings.h"
#include "CollisionManager.h"

GuardInvestigateState::GuardInvestigateState() {

}

GuardInvestigateState::~GuardInvestigateState() {

}

void GuardInvestigateState::Enter() {
	std::cout << "Entering GuardInvestigateState\n";
	m_done = false;
}

void GuardInvestigateState::Exit() {

}

void GuardInvestigateState::Init(int number, sf::Vector2f* p_position, AnimatedSprite* sprite) {
	m_waypoints.clear();
	
	m_done = false;
	m_nextState = "";

	mp_sprite = sprite;

	m_number = number;

	mp_position = &Settings::ms_guards.at(m_number);

	for(int i = 0; i < Settings::m_allGuardWaypoints[m_number].size(); i++) {
	
		m_waypoints.push_back(Settings::m_allGuardWaypoints[m_number].at(i));

	}

	mp_position = p_position;

	mp_sprite = sprite;
}

bool GuardInvestigateState::Update() {
	Movement();
	
	return m_done;
}

std::string GuardInvestigateState::Next() {
	return m_nextState;
}

bool GuardInvestigateState::IsType(const std::string &c_type) {
	return c_type.compare("GuardInvestigateState") == 0;
}


void GuardInvestigateState::Cleanup() {

}

void GuardInvestigateState::UpdateAnimation() {
	mp_sprite->Update();
	m_rotation = atan2(mp_position->y - m_waypoints.at(0).y, mp_position->x - m_waypoints.at(0).x) * 180 / M_PI - 90;
	mp_sprite->getSprite()->setRotation(m_rotation);
}

void GuardInvestigateState::AddWaypointToFront(sf::Vector2f waypoint) {
	m_waypoints.clear();
	m_waypoints.push_back(waypoint);
}

void GuardInvestigateState::Movement() {
	if(m_waypoints.size() > 0) {
		sf::Vector2f distance = *mp_position - m_waypoints.at(0);
		float speed = 100;
		float dist = sqrtf(distance.x * distance.x + distance.y * distance.y);
		if(dist > 1) {
			distance.x /= dist;
			distance.y /= dist;

			*mp_position = *mp_position - distance * speed * Settings::ms_deltatime;
		}
		else {
			m_done = true;
			m_nextState = "GuardPatrolState";
		}
	}
}

bool GuardInvestigateState::Detected(sf::Vector2f playerPosition, CollisionManager* p_collisionManager) {
	sf::Vector2f direction(*mp_position - playerPosition);
	float distance = sqrtf(direction.x * direction.x + direction.y * direction.y);
	while(distance > 20) {
		if(p_collisionManager->Circle_WallCollision(*mp_position, 46)) {
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