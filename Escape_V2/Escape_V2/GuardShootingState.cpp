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
	m_timer = 0.0f;
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
	mp_sprite->ChangeAnimation("Guard1DrawWeapon.png");
}

void GuardShootingState::Exit() {

}

void GuardShootingState::Init(int number, sf::Vector2f* p_position, float* p_rotation, AnimatedSprite* sprite, Grid2D* p_grid) {
	m_waypoints.clear();

	mp_position = p_position;
	mp_rotation = p_rotation;
	
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
	
	
	if(m_timer > 2.5f) {
		Shoot(p_collisionManager, player_position);
	}
	else if(m_timer > 1.0f) {
		mp_sprite->ChangeAnimation("Guard1Shooting.png");
	}

	m_timer += Settings::ms_deltatime;

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
	mp_sprite->getSprite()->setRotation(*mp_rotation);
}

void GuardShootingState::AddWaypointToFront(sf::Vector2f waypoint) {
	
}

bool GuardShootingState::Detected(sf::Vector2f playerPosition, CollisionManager* p_collisionManager) {
	sf::Vector2f vectorBetween(playerPosition - *mp_position);
	float distance = sqrtf(vectorBetween.x * vectorBetween.x + vectorBetween.y * vectorBetween.y);

	if(distance > 800) {
		return false;
	}

	Rotate(playerPosition);

	float angleToPlayer = static_cast<int>(atan2f(vectorBetween.y, -vectorBetween.x) * 180 / static_cast<float>(M_PI) + 180.0f) % 360;
	float directionLooking = -(*mp_rotation) + 90;
	int diffAngle = static_cast<int>(angleToPlayer - directionLooking + 360) % 360;

	sf::Vector2f direction;
	if(distance > 0.001f) {
		direction = vectorBetween / distance;
	}

	while(distance > 8) {
		if(p_collisionManager->Circle_WallCollision(playerPosition - vectorBetween, 15)) {
			return false;
		}

		vectorBetween.x -= direction.x * 10;
		vectorBetween.y -= direction.y * 10;

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

bool GuardShootingState::Rotate(sf::Vector2f playerPosition) {
	int rotationToGetTo = ( static_cast<int>(atan2(mp_position->y - playerPosition.y, mp_position->x - playerPosition.x) * 180 / static_cast<float>(M_PI) - 90) + 360 ) % 360;
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

void GuardShootingState::Shoot(CollisionManager* p_collisionManager, sf::Vector2f playerPosition) {
	std::cout << "Shooting! Dead!\n";
	Settings::Shoot(*mp_position, playerPosition);
	Settings::ms_gameOver = true;
}
