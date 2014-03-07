// GuardSearchState.cpp

#include "GuardSearchState.h"
#include "AnimatedSprite.h"
#include "CollisionManager.h"
#include "PathFinding.h"
#include "Grid2D.h"
#include "stdafx.h"
#include "Settings.h"

GuardSearchState::GuardSearchState() {

}

GuardSearchState::~GuardSearchState() {

}

void GuardSearchState::Enter() {
	std::cout << "Entering GuardSearchState\n";
	m_done = false;
}

void GuardSearchState::Exit() {
	m_waypoints.clear();
}

void GuardSearchState::Init(int number, sf::Vector2f* p_position, AnimatedSprite* sprite, Grid2D* p_grid) {
	m_number = number;

	m_waypoints.clear();
	
	m_done = false;
	m_nextState = "";

	mp_position = p_position;

	mp_sprite = sprite;

	mp_grid = p_grid;
	mp_pathfinding = new PathFinding();
	mp_pathfinding->Init(mp_grid);
}

bool GuardSearchState::Update(sf::Vector2f player_position, CollisionManager* p_collisionManager) {
	if(Detected(player_position, p_collisionManager)) {
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
}

void GuardSearchState::AddWaypointToFront(sf::Vector2f waypoint) {
	m_waypoints.clear();
	m_waypoints.push_back(waypoint);
}

bool GuardSearchState::Detected(sf::Vector2f playerPosition, CollisionManager* p_collisionManager) {
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

void GuardSearchState::Movement() {
	if(!mp_pathfinding->m_foundGoal) {
		while(!mp_pathfinding->m_foundGoal) {
			mp_pathfinding->FindPath(sf::Vector2f((*mp_position).x / 40.0f, (*mp_position).y / 40.0f), sf::Vector2f(m_waypoints.at(0).x / 40.0f, m_waypoints.at(0).y / 40.0f));
		}
	}
	if(mp_pathfinding->m_foundGoal) {
		sf::Vector2f vec = mp_pathfinding->NextPathPos(sf::Vector2f((*mp_position).x / 40.0f, (*mp_position).y / 40.0f), 5.0f / 40.0f);
		vec.x *= 40;
		vec.y *= 40;
		
		m_waypoints.at(0) = vec;
	}
	
	if(m_waypoints.size() > 0) {
		sf::Vector2f distance = *mp_position - m_waypoints.at(0);
		float speed = 100;
		float dist = sqrtf(distance.x * distance.x + distance.y * distance.y);
		if(dist > 0.01) {
			distance.x /= dist;
			distance.y /= dist;

			*mp_position = *mp_position - distance * speed * Settings::ms_deltatime;
		}
	}
	//mp_pathfinding->Draw(Settings::ms_window);
}