// GuardShootingState.cpp

#include "GuardShootingState.h"
#include "AnimatedSprite.h"
#include "CollisionManager.h"
#include "Settings.h"
#include "PathFinding.h"
#include "Grid2D.h"
#include "stdafx.h"

GuardShootingState::GuardShootingState() {
	m_nextState = "GuardSearchState";
}

GuardShootingState::~GuardShootingState() {
	if(mp_pathfinding != nullptr) {
		delete mp_pathfinding;
		mp_pathfinding = nullptr;
	}
}

void GuardShootingState::Enter() {
	std::cout << "Entering GuardShootingState\n";
	m_done = false;
	m_timer = 0.0f;
	mp_sprite->ChangeAnimation("Guard1Shooting.png");
}

void GuardShootingState::Exit() {

}

void GuardShootingState::Init(int number, sf::Vector2f* p_position, AnimatedSprite* sprite, Grid2D* p_grid) {
	m_waypoints.clear();

	mp_position = p_position;
	
	m_done = false;
	m_nextState = "";

	mp_sprite = sprite;

	mp_grid = p_grid;
	mp_pathfinding = new PathFinding();
	mp_pathfinding->Init(mp_grid);
}

bool GuardShootingState::Update(sf::Vector2f player_position, CollisionManager* p_collisionManager) {
	if(!Detected(player_position, p_collisionManager)) {
		m_nextState = "GuardChaseState";
		m_done = true;
	}
	
	return m_done;
}

std::string GuardShootingState::Next() {
	return m_nextState;
}

bool GuardShootingState::IsType(const std::string &c_type) {
	return c_type.compare("GuardShootingState") == 0;
}


void GuardShootingState::Cleanup() {

}

void GuardShootingState::UpdateAnimation(sf::Vector2f playerPosition) {
	mp_sprite->Update();
	m_rotation = atan2(mp_position->y - playerPosition.y, mp_position->x - playerPosition.x) * 180 / static_cast<float>(M_PI) - 90;
	mp_sprite->getSprite()->setRotation(m_rotation);
}

void GuardShootingState::AddWaypointToFront(sf::Vector2f waypoint) {
	
}

bool GuardShootingState::Detected(sf::Vector2f playerPosition, CollisionManager* p_collisionManager) {
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