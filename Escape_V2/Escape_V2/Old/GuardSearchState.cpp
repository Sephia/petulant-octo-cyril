// GuardSearchState.cpp

#include "GuardSearchState.h"
#include "AnimatedSprite.h"
#include "CollisionManager.h"

GuardSearchState::GuardSearchState() {

}

GuardSearchState::~GuardSearchState() {

}

void GuardSearchState::Enter() {
	std::cout << "Entering GuardSearchState\n";
	m_done = false;
}

void GuardSearchState::Exit() {

}

void GuardSearchState::Init(int number, sf::Vector2f* p_position, AnimatedSprite* sprite) {
	m_number = number;

	m_waypoints.clear();
	
	m_done = false;
	m_nextState = "";

	mp_sprite = sprite;
}

bool GuardSearchState::Update() {
	

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

void GuardSearchState::UpdateAnimation() {
	mp_sprite->Update();
}

void GuardSearchState::AddWaypointToFront(sf::Vector2f waypoint) {
	
}

bool GuardSearchState::Detected(sf::Vector2f playerPosition, CollisionManager* p_collisionManager) {
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