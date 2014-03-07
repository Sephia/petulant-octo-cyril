// GuardPatrolState.cpp

#include "GuardPatrolState.h"
#include "Settings.h"
#include "AnimatedSprite.h"
#include "CollisionManager.h"
#include "stdafx.h"
#include "PathFinding.h"


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

void GuardPatrolState::Init(int number, sf::Vector2f* p_position, AnimatedSprite* sprite, Grid2D* p_grid) {
	m_number = number;

	m_waypoints.clear();
	
	m_done = false;
	m_nextState = "";

	mp_position = &Settings::ms_guards.at(m_number);

	for(unsigned int i = 0; i < Settings::m_allGuardWaypoints[m_number].size(); i++) {
	
		m_waypoints.push_back(Settings::m_allGuardWaypoints[m_number].at(i));

	}

	m_nextWaypoint = 0;

	mp_position = p_position;

	mp_sprite = sprite;

	mp_grid = p_grid;
	mp_pathfinding = new PathFinding();
	mp_pathfinding->Init(mp_grid);
}

bool GuardPatrolState::Update(sf::Vector2f playerPosition, CollisionManager* p_collisionManager) {
	Movement();
	if(Detected(playerPosition, p_collisionManager)) {
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
	m_rotation = atan2(mp_position->y - m_waypoints.at(m_nextWaypoint).y, mp_position->x - m_waypoints.at(m_nextWaypoint).x) * 180 / static_cast<float>(M_PI) - 90;
	mp_sprite->getSprite()->setRotation(m_rotation);
}

void GuardPatrolState::Movement() {
	if(m_waypoints.size() > 0) {
		sf::Vector2f distance = *mp_position - m_waypoints.at(m_nextWaypoint);
		float speed = 100;
		float dist = sqrtf(distance.x * distance.x + distance.y * distance.y);
		if(dist > 5) {
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
	sf::Vector2f vectorBetween(playerPosition - *mp_position);
	float distance = sqrtf(vectorBetween.x * vectorBetween.x + vectorBetween.y * vectorBetween.y);

	if(distance > 600) {
		return false;
	}

	float angleToPlayer = static_cast<int>(atan2f(vectorBetween.y, -vectorBetween.x) * 180 / static_cast<float>(M_PI) + 180.0f) % 360;
	float directionLooking = -m_rotation + 90;
	int diffAngle = static_cast<int>(angleToPlayer - directionLooking + 360) % 360;

	if(diffAngle < 300 && diffAngle > 60) {
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

		vectorBetween.x -= direction.x * 10;
		vectorBetween.y -= direction.y * 10;

		/*sf::CircleShape circ(15);
		circ.setPosition(playerPosition - vectorBetween);
		circ.setFillColor(sf::Color(100, 100, 100));
		circ.setOrigin(15.0f, 15.0f);
		Settings::ms_window->draw(circ);
		Settings::ms_window->display();*/

		float sqr = sqrtf(vectorBetween.x * vectorBetween.x - vectorBetween.y * vectorBetween.y);
		
		if(sqr > 0) {
			distance = sqrtf(vectorBetween.x * vectorBetween.x - vectorBetween.y * vectorBetween.y);
		}
		else {
			return false;
		}
	}

	return true;
}